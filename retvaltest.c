#include <stdio.h>
#include <stdlib.h>

void *retpointer()
{
	return (void *) 42;
}

int main()
{
	void *retval = retpointer();
	printf("%d\n", (int)retval);
	return 0; 
}
