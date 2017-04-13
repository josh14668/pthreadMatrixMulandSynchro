#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef enum { false, true } bool;
#define SIZE 256


float **A,**B, **C; //global variables for two-dimensional arrays
int M, K, N; //matrix dimensions
int num_thrds; //number of threads to be created
pthread_attr_t attr;

struct matrixSubSet{
	int id;
	int start_col;
	int end_col;


};

void populateMat(float** Mat,int row,int column,bool setZero){
	srand48(time(NULL));

	for(int i=0;i<row;i++){
		for(int j =0;j<column;j++){
			if(setZero){
				Mat[i][j]=0;
			}else{
				//Mat[i][j]=1;
				Mat[i][j]=drand48();
			}
		}
	}
}

float **matMul(int r1, int c1, int c2, float **A, float**B){

  float **matC=(float**)malloc(sizeof(float)*SIZE);

	for (int i=0;i<r1;i++){
    matC[i]=(float*)calloc(c2,sizeof(float));
  }

	for(int i=0; i<r1; ++i)
        for(int j=0; j<c2; ++j)
            for(int k=0; k<c1; ++k)
            {
                matC[i][j]+=A[i][k]*B[k][j];
            }

return matC;
}



//thread routine for matrix multiplication.
void *mat_mult_thrds(void *threadarg)
{
	struct matrixSubSet *matLims =(struct matrixSubSet*) threadarg;
	int id = matLims->id;
	int a = matLims->start_col;
	int b =matLims->end_col;
	int num_columns_temp=(b-a+1);

	//Allocates space for subset
	float **temp_ans = (float**)malloc(SIZE*K);
	for(int i=0;i<K;i++){
		temp_ans[i]=(float*)calloc(sizeof(float), num_columns_temp);
	}
//Fills Subset with column values from B
	for(int i=0;i<K;i++){
		for(int j=a;j<=b;j++){
			temp_ans[i][j-a]=B[i][j];
		}
	}
	float **temp_sub = (float**)malloc(SIZE*M);
	for(int i=0;i<M;i++){
		temp_sub[i]=calloc(sizeof(float), N);
	}
	temp_sub=matMul(M, K, num_columns_temp, A, temp_ans);

	for(int i=0;i<M;i++){
		for(int j=a;j<=b;j++){
			C[i][j]=temp_sub[i][j-a];
		}
	}

	for(int i=0;i<K;i++){
		free(temp_ans[i]);
	}
	free(temp_ans);

	for(int i=0;i<M;i++){
		free(temp_sub[M]);
	}
	free(temp_sub);


	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t *threads;
	int *t; //user defined thread ids
	int rc,m,n,k;
	void *status;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


	// ask for matrices dimensions M, K, N for the user.
	printf("Please insert matrix dimensions m(int):\n");
	scanf("%d",&m);
	printf("Please insert matrix dimensions k(int):\n");
	scanf("%d",&k);
	printf("Please insert matrix dimensions n(int):\n");
	scanf("%d",&n);
	M=m;
	K=k;
	N=n;
	//ask for the number of threads num_thrds to be created from the user.
	printf("How many threads:\n");
	scanf("%d",&num_thrds);
	printf("\n");
	printf("A:%dx%d and B:%dx%d\n",m,k,k,n );

	int colPerThread = 0;
  int lastThreadExtraCol = 0;

  if(num_thrds>=n){
    num_thrds=n;
  }else{
    colPerThread = n/num_thrds;
    lastThreadExtraCol = n%num_thrds;

  }



	struct matrixSubSet subset[num_thrds];

	//create matrices A, B and C using malloc().

	A = (float**)malloc(SIZE*m);
	for(int i=0;i<m;i++){
		A[i]=(float*)calloc(k,sizeof(float));
	}

	B = (float**)malloc(SIZE*k);
	for(int i=0;i<k;i++){
		B[i]=(float*)calloc(n,sizeof(float));
	}

	C = (float**)malloc(SIZE*m);
	for(int i=0;i<m;i++){
		C[i]=(float*)calloc(n,sizeof(float));
	}




	//initialize matrices A and B using srand() and rand()
	//the range of float values should be between 0 and 1.0.
	//initialize matrix C.

	populateMat(A, m, k, false);
	populateMat(B, k, n,false);

	//allocate memory for thread ids.
	threads = (pthread_t*)malloc(sizeof(pthread_t)*num_thrds);
	t=(int*)malloc(sizeof(int)*num_thrds);



	int i;

  if(num_thrds==n){
    for(int i=0;i<num_thrds;i++){
      subset[i].id=i;
      subset[i].start_col=i;
      subset[i].end_col=i;
      pthread_create((threads+i),&attr,mat_mult_thrds,(void*)(subset+i));
    }
  }else{
    for(i = 0; i<num_thrds-1;i++){
      if(i==0){
        subset[i].start_col=0;
        subset[i].end_col =subset[i].start_col+(colPerThread-1);

      }else{
        subset[i].start_col=subset[i-1].start_col+(colPerThread);
        subset[i].end_col =subset[i].start_col+(colPerThread-1);

      }
      subset[i].id=i;

      pthread_create((threads+i),&attr,mat_mult_thrds,(void*)(subset+i));
    }

    subset[i].id=i;
    subset[i].start_col = subset[i-1].end_col+1;
    subset[i].end_col = subset[i].start_col+(colPerThread-1)+lastThreadExtraCol;
    pthread_create((threads+i),&attr,mat_mult_thrds,(void*)(subset+i));

}



	//join threads.
	for(int i=0;i<num_thrds;i++) {
		pthread_join(threads[i], NULL);
	}


	//perform a sequential computation using the same A and B and
	//compare the results.


	printf("\nMatrix A(%dx%d):\n",m,k);
	for(int i=0; i<m; i++){
		for(int j=0; j<k; j++){
			printf("%f  ", A[i][j]);

		}
		printf("\n");
	}

	printf("\nMatrix B(%dx%d):\n",k,n);
	for(int i=0; i<k; i++){
	for(int j=0; j<n; j++){
		printf("%f  ", B[i][j]);

	}
	printf("\n");
	}

	//C=matMul(m, k, n, A, B);


	printf("\nMatrix C Threaded(%dx%d):\n",m,n);
	for(int i=0; i<m; i++){
	for(int j=0; j<n; j++){
		printf("%f  ", C[i][j]);

	}
	printf("\n");
	}

	printf("\n");

	printf("\nMatrix C Serial(%dx%d):\n",m,n);
	for(int i=0; i<m; i++){
	for(int j=0; j<n; j++){
		printf("%f  ", matMul(m, k, n, A, B)[i][j]);

	}
	printf("\n");
	}

	for(int i=0;i<m;i++){
		free(A[i]);
	}
	free(A);

	for(int i=0;i<k;i++){
		free(B[i]);
	}
	free(B);

	for(int i=0;i<m;i++){
		free(C[i]);
	}
	free(C);

	pthread_exit(NULL);
	free(threads);
}
