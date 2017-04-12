#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int no_of_customers;
int no_of_seats;
int no_of_free_seats;
int no_of_terminals;
int no_of_free_terms;

//anything you want to add


void * customer_routine(void *);
void * attendant_routine(void *);

//declare global mutex and condition variables
pthread_mutex_t no_seat_mutex;
pthread_mutex_t no_terminals_mutex;

pthread_cond_t attendent_cond;
pthread_cond_t customer_arrives_cond;
pthread_cond_t terminal_use_finished_cond;
pthread_cond_t terminal_available_cond;



int main(int argc, char ** argv)
{
	//any variables
	int customer_pace, terminal_usage_time;


	// ask user to provide the total number of seats & total number of terminals.
	printf("Total number of seats in waiting room(int):\n");
	scanf("%d",&no_of_seats);
	printf("Total number of Terminals(int):\n");
	scanf("%d",&no_of_terminals);

	//Initally all seats and terminals are free
	no_of_free_seats=no_of_seats;
	no_of_free_terms=no_of_terminals;


	// ask user to provide the total number of customers, customers arrival rate & terminal usage time.
	printf("Total Number of customers(int):\n");
	scanf("%d",&no_of_customers);
	printf("Customers arrival rate(int):\n");
	scanf("%d",&customer_pace);
	printf("Terminal usage time (int):\n");
	scanf("%d",&terminal_usage_time);



	//Initialize mutexes and condition variable objects
	pthread_mutex_init(&no_seat_mutex, NULL);
	pthread_mutex_init(&no_terminals_mutex, NULL);

	pthread_cond_init(&customer_arrives_cond, NULL);
	pthread_cond_init(&attendent_cond, NULL);
	pthread_cond_init(&terminal_use_finished_cond, NULL);
	pthread_cond_init(&terminal_available_cond, NULL);



	// anything you want to add

	pthread_t *thread = (pthread_t*)malloc((no_of_customers+1) * sizeof(pthread_t)); //total is No_Of_Consuers + 1 to include farmer

	if(thread == NULL){
		fprintf(stderr, "threads out of memory\n");
		exit(1);
	}
	int * t_ids = malloc((no_of_customers+1) * sizeof(int)); //total is No_Of_Consuers + 1 to include farmer
	if(t_ids == NULL){
		fprintf(stderr, "t out of memory\n");
		exit(1);
	}

	//create the attendant thread.
	int rc = pthread_create(&thread[0],NULL,attendant_routine,NULL);
	if (rc) {
		printf("ERROR; return code from pthread_create() (attendant) is %d\n", rc);
		exit(-1);
	}

	//create consumer threads according to the arrival rate (in the range between 0 and arrival rate) and
	//pass user-defined id and terminal usage (in the range between 0 and terminal usage)
	srand(time(0));

	for (int k = 1; k<=no_of_customers; k++)
	{
		sleep((int)rand() % customer_pace); //sleep a few second before creating a thread
		t_ids[k] = k;
		int rc = pthread_create(&thread[k], NULL, customer_routine, (void *)&t_ids[k]); //consumer routine takes thread id as the arg
		if (rc) {
			printf("ERROR; return code from pthread_create() (consumer) is %d\n", rc);
			exit(-1);
		}
	}

	//anything you want to add

	for(int i=1;i<=no_of_customers;i++){
		pthread_join(thread[i],NULL);
	}

	pthread_cancel(thread[0]);


	free(thread);
	free(t_ids);

  pthread_exit(EXIT_SUCCESS);



}

void * attendant_routine(void * noargs)
{
	//TODO: What if all seats are filled?
    while (1) //Continue to serve customers.
    {
			pthread_mutex_lock(&no_seat_mutex);
			if(no_of_free_seats==no_of_seats){
				//Attendant is waiting on customers, as there are no customer currently in the waiting room
				printf("Attendant: The number of free seats is %d. No customers and I'm waiting. \n",no_of_free_seats);
				pthread_cond_wait(&customer_arrives_cond, &no_seat_mutex);
				pthread_mutex_unlock(&no_seat_mutex);
			}else{

				printf("Attendant: The number of free seats now is %d. trying to find a free terminal\n",no_of_free_seats);

				//No Terminals available
				pthread_mutex_lock(&no_terminals_mutex);
				if(no_of_free_terms==0){
					printf("Attendant: The number of free terminals is %d. All terminals are occupied. ", no_of_free_terms);
					//Waiting for terminals to become free
					pthread_cond_wait(&terminal_use_finished_cond,&no_terminals_mutex);
					pthread_mutex_unlock(&no_terminals_mutex);
				}
				printf("Attendant: The number of free terminals is %d. There are free terminals now. \n", no_of_free_terms);

				no_of_free_seats++;
				no_of_free_terms--;
				pthread_mutex_unlock(&no_seat_mutex);
				pthread_mutex_unlock(&no_terminals_mutex);
				//Signal Customer that a terminal is available
				pthread_cond_signal(&terminal_available_cond);

				printf("Attendant: Call one customer. The number of free seats is now %d\n",no_of_free_seats);
				//pthread_mutex_unlock(&no_seat_mutex);
				printf("Attendant: Assign one terminal to the customer. The number of free terminals is now %d\n",no_of_free_terms);

			//The attendant thread must print the following status messages wherever appropriate:
			//"Attendant: The number of free seats is %d. No customers and I'm waiting. \n"
			//"Attendant: The number of free seats now is %d. try to find a free terminal.\n"
			//"Attendant: The number of free terminal s is %d. All terminals are occupied. \n"
			//"Attendant: The number of free terminals is %d. There are free terminals now. \n"
			//"Attendant: Call one customer. The number of free seats is now %d.\n"
			//"Attendant: Assign one terminal to the customer. The number of free terminals is now %d.\n"

			//pthread_exit(NULL);

			}
    }
}

void * customer_routine(void * args)
{
	int id =*(int*)args;
	printf("Customer %d arrives.\n",id);
	pthread_mutex_lock(&no_seat_mutex);
	if(no_of_free_seats==0){
		//If there are no free seats in waiting room customer leaves
		printf("Customer %d oh no! all seats have been taken and I'll leave now!\n",id);
		pthread_mutex_unlock(&no_seat_mutex);
		pthread_exit(NULL);
	}else{

//Customer has arrived in waiting room and fills a single seat
////////////////////////////////////////////////////////////////////////
		//Customer takes a seat must signal to attendant that he has arrived
		printf("Customer %d: I'm lucky to get a free seat from %d.\n",id,no_of_free_seats);
		no_of_free_seats--;
		pthread_mutex_unlock(&no_seat_mutex);
		//Customer Signals to attendant that he has arrived
		pthread_cond_signal(&customer_arrives_cond);
////////////////////////////////////////////////////////////////////////


//Customer is waiting to be served by the attendant
////////////////////////////////////////////////////////////////////////
		pthread_mutex_lock(&no_terminals_mutex);
		//Customer must wait for signal from attendant
		printf("Customer %d: I'm waiting to be served\n",id);
		//pthread_mutex_lock(&no_terminals_mutex);
		pthread_cond_wait(&terminal_available_cond,&no_terminals_mutex);
		printf("Customer %d: I'm getting a terminal now\n",id);
		//Customer vacates seat in waiting area and fills one terminal spot
		//no_of_free_terms--;
		//pthread_mutex_unlock(&no_terminals_mutex);
		//Customer is using the terminal waits for specefied amount of time
		sleep((int)rand() % 5);
		printf("Customer %d: I'm finished using the terminal and leaving\n",id);
		//pthread_mutex_lock(&no_terminals_mutex);

		no_of_free_terms++;

		printf("thread %d num_terms:%d\n",id,no_of_free_terms);
		pthread_mutex_unlock(&no_terminals_mutex);

		pthread_cond_signal(&terminal_use_finished_cond);
		//Signal to attendent that customer has finished using terminal


		//TODO: This pthread_exit statment is an issue
		//pthread_exit(NULL);

	}

	//customer thread must print the following status messages wherever appropriate where id is the identifying number of the thread:
	//"Customer %d: oh no! all seats have been taken and I'll leave now!\n"
	//"Customer %d: I'm lucky to get a free seat from %d.\n"
	//"Customer %d: I'm to be served.\n"
	//"Customer %d: I'm getting a terminal now.\n"
	//"Customer %d: I'm finished using the terminal and leaving.\n"



	pthread_exit(NULL);
}
