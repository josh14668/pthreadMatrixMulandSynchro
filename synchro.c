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
... please fill in.


void * customer_routine(void *);
void * attendant_routine(void *);

//declare global mutex and condition variables
... please fill in


int main(int argc, char ** argv)
{
	//any variables
	... please fill in.


	// ask user to provide the total number of seats & total number of terminals.
	... please fill in,


	// ask user to provide the total number of customers, customers arrival rate & terminal usage time.
	... please fill in.


	//Initialize mutexes and condition variable objects
	... please fill in.


	// anything you want to add
	... please fill in.


	//create the attendant thread.
	... please fill in.


	//create consumer threads according to the arrival rate (in the range between 0 and arrival rate) and
	//pass user-defined id and terminal usage (in the range between 0 and terminal usage)
	... please fill in.


	//anything you want to add
	... please fill in.


    pthread_exit(NULL);
}

void * attendant_routine(void * noargs)
{
    while (1) //Continue to serve customers.
    {
		//The attendant thread must print the following status messages wherever appropriate:
		//"Attendant: The number of free seats is %d. No customers and I'm waiting. \n"
		//"Attendant: The number of free seats now is %d. try to find a free terminal.\n"
		//"Attendant: The number of free terminal s is %d. All terminals are occupied. \n"
		//"Attendant: The number of free terminals is %d. There are free terminals now. \n"
		//"Attendant: Call one customer. The number of free seats is now %d.\n"
		//"Attendant: Assign one terminal to the customer. The number of free terminals is now %d.\n"

        ... please fill in.


    }
}

void * customer_routine(void * args)
{
	//customer thread must print the following status messages wherever appropriate where id is the identifying number of the thread:
	//"Customer %d arrives.\n"
	//"Customer %d: oh no! all seats have been taken and I'll leave now!\n"
	//"Customer %d: I'm lucky to get a free seat from %d.\n"
	//"Customer %d: I'm to be served.\n"
	//"Customer %d: I'm getting a terminal now.\n"
	//"Customer %d: I'm finished using the terminal and leaving.\n"

	... please fill in.


}
