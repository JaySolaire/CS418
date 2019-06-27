#include <time.h>
#include <stdio.h>
int main(void)
{
	double time_used;
	clock_t start = clock();
	
	printf("hello world!\n");
	
	clock_t end = clock();
	
	time_used = (double) (end - start) / CLOCKS_PER_SEC;
	printf("took about %f seconds long \n", time_used);
	
	return 0;
}
