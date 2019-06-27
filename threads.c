#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define ARRAYSIZE 10000
/* Basically this program works by separating the array into sections based on how many threads there are. EAch thread gets its own section.
100 threads means 100 sections. The section is passed into the thread through the st_section struct, which holds the location of the first value of the array
and the size of the section. 
This program never did end up working, in part at least because converting the structure to a void pointer and back into a struct lost some data. Kinda ruins 
the whole program. 

I tried running this with global variables first, with each thread comparing its local max to a global max. Then I tried running with a return value, which 
is also converted to a null pointer and back. I just got the return value to work when I realized that the struct was a problem (or something), discovered 
through the printsection function.
*/
int min;
//int max;

struct st_section
{
	int *loc;
	int end;
};
/*
void *findMinMax(void *secPTR)
{
	int locmax = 0;				//this represents local min and local max (meaning local to the thread), unrelated to the sec->loc, which is the location of the section
	int locmin = 50000;			//the local min and max exist to reduce the chance of threads accessing the global variables at the same time. Rather than access it for every comparison, each thread only needs to access it once (which its finished)
	struct st_section *sec;
	sec = (struct st_section*) secPTR;
	while (sec->end > 0)
	{
		if (*sec->loc > locmax)
			locmax = *sec->loc;
		if (*sec->loc < min)
			locmin = *sec->loc;
		sec->loc++;
		sec->end--;
	}
	if (locmin < min)
		min = locmin;
	if (locmax > max)
		max = locmax;
}
*/

void *findMax(void *secPTR)
{
	int max = 0;
	struct st_section *sec;
	sec = (struct st_section*) secPTR;
	while (sec->end > 0)
	{
		if (*sec->loc > max)
			max = *sec->loc;
		sec->loc++;
		sec->end--;
	}
	return (void *) max;
}
	
void *printsection(void *secPTR)
{
	struct st_section *sec;
	sec = (struct st_section*) secPTR;
	printf("Seclength: %d\n", sec->end);
	while (sec->end > 0)
	{printf("%d\t", *sec->loc);
		sec->loc++;}
}

int main(int argv, char **argc)
{
	//Resets the maximum and minimum from previous runs
	//void *retmax;
	//retmax = malloc(sizeof(int)); //because it will be holding integers.
	min = 50000;
	int max = 0;
	
	//Generates and populates array
	int  arr[ARRAYSIZE];
	int i;
	for (i = 0; i < ARRAYSIZE; i++)
		arr[i] = i;
		//arr[i] = rand()%50000;
		
	//Gathers number of thread information from user
	int numthreads;											//the number of threads
	int seclength;											//the size of the section of the array for each thread
	if (argv != 2)											//gets valid input from the user, else fails
	{
		printf("Usage: ./program [# of threads]\nNo threads specified, defaulting to 100.\n");
		numthreads = 100;
	}
	else
		numthreads = atoi(argc[1]);
	if (numthreads == 0 || ARRAYSIZE%numthreads != 0)		//it has to divide evenly for splitting among threads to work
	{	
		printf("Invalid number of threads.\n");
		return 0;
	}
	seclength = ARRAYSIZE/numthreads;
	
	//Prints the first and last ten values of the array
	printf("The first 10 values of the array, from the front, are:\n");
	for (i = 0; i < 10; i++)
		printf("%d, ", arr[i]);
	printf("\nThe last values of the array, from the back, are:\n");
	for (i = 1; i < 11; i++)
		printf("%d, ", arr[ARRAYSIZE - i]);
	printf("\n");

	
	//Generates struct to hold section of array for each thread
	struct st_section *secPTR;
	secPTR = malloc(sizeof(struct st_section*));
	secPTR->loc= arr;
	secPTR->end= seclength;

	//Generates and runs threads
	pthread_t thread_id[numthreads];
	for (i = 0; i < numthreads; i++)
	{
	//	printf("\nnewthread\n");
		pthread_create(&thread_id[i], NULL, &printsection, (void*) secPTR);
		secPTR->loc+=seclength;	
	}
	
	//Waits for all the threads to finish, and compares there return values
	void *retval;
	for (i = 0; i < numthreads; i++)
	{	
	
	
		if (max < (int)retval)
			max = (int)retval;
	}	
	printf("Max: %d\nMin: %d\n", max, min);
	return 0;	

}
