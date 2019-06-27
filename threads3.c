#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define ARRAYSIZE 10000
#define NUMTHREADS 2

//Global arrays holding the values, the mins and maxes of the threads.
int arr[ARRAYSIZE];
int	locMax[NUMTHREADS];
int	locMin[NUMTHREADS];
int threadnum;


void *findMinMax(void* arg)
{
	
	
	int i;
	int section = ARRAYSIZE / NUMTHREADS;				//creates a section size for each thread, based on the number of threads. So if the array is 1000, and there are 50 threads, then the section size for each thread is 20
	int num = threadnum++;								
	//^ this one line of code is the same as saying: 
	//num = threadnum;
	//threadnum++;
	//because the threadnum is global, it will be used in the next thread process too.
	for (i = 0; i < section; i++)						//this is a normal high water mark. If the currect array number is bigger than the local max, then its the new max. If its smaller than the min, its the new min
	{
		if (arr[i + (section*num)] > locMax[num])		// arr[i + section * num] simply determines where one is in the array. Assuming that the array is 100, there are 50 threads, and the size is 20, as above. If this is thread # 3, then it should start at 60 and go to 80. (section * num) equals sixty, and i just counts to 20. So it should go 60 to 79 or 61 to 80 (i dont remember exactly)
			locMax[num] = arr[i + (section*num)];
		if (arr[i + (section*num)] < locMin[num])
			locMin[num] = arr[i + (section*num)];
	}
}

int main(int argv, char **argc)
{
	//Starts Clock
	double time_used;
	clock_t start = clock();
	
	//Populates number array with random values and resets localMax/localMin arrays
	int i;
	for (i = 0; i < ARRAYSIZE; i++)
		arr[i] = rand()%50000;
	for (i = 0; i < NUMTHREADS; i++)	
		locMax[i] = 0;
	for (i = 0; i < NUMTHREADS; i++)	
		locMin[i] = 50000;
	
	//Prints the first and last ten values of the array
	printf("The first 10 values of the array, from the front, are:\n");
	for (i = 0; i < 10; i++)
		printf("%d, ", arr[i]);
	printf("\nThe last values of the array, from the back, are:\n");
	for (i = 1; i < 11; i++)
		printf("%d, ", arr[ARRAYSIZE - i]);
	printf("\n");


	//Creates threads
	threadnum = 0;
	pthread_t thread_id[NUMTHREADS];
	for (i = 0; i < NUMTHREADS; i++)
		pthread_create(&thread_id[i], NULL, findMinMax, (void*)NULL);		//this passes the findMinMax FUNCTION into each thread. so each thread should run it. It also passes a null pointer in. I think in order to pass a function to a thread, it was required to pass arguments as well, which is why the null pointer in FindMinMax( void *voidpointer) exists but isn't used.
		
	//Joins threads
	//aka waits for each thread to end before moving on in Main()
	for (i = 0; i < NUMTHREADS; i++)	
		pthread_join(thread_id[i], NULL);

	//Gets Min and Max
	
	//so after each thread finds the local min/max, this is second high watermark that finds the final min/max from each of the local ones. 
	//Basically, the program runs in two steps. First, the array is broken into sections using threads, and a min/max is found for each section. Then in main, the min/max of these local min/maxes is found to get the min/max of the whole array.
	int min = 50000;
	int max = 0;
	for (i = 0; i < NUMTHREADS; i++)
	{
		if (locMax[i] > max)
			max = locMax[i];
		if (locMin[i] < min)
			min = locMin[i];
	}
	printf("The max is %d and the min is %d.\n", max, min);
	
	//Gets Total time, code courtesy of Kenny
	//yeah this bitch just times it.
	clock_t end = clock();
	time_used = (double) (end - start) / CLOCKS_PER_SEC;
	printf("%d threads took about %f seconds long \n", NUMTHREADS, time_used);
	
	return 0;
}



