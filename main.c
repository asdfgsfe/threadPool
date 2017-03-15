#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>

#include"h_threadpool.h"

void *mytask_run(void *arg)
{
	printf("threads 0x%x working on task %d\n", (int)pthread_self(), *(int*)arg);
	sleep(1);
	free(arg);

	return NULL;
}

int main(void)
{
	threadpool_t pool;
	threadpool_init(&pool, 3);

	int i;
	for(i=0; i<10; i++)
	{
		int *arg = (int *)malloc(sizeof(int));
		*arg = i;

		threadpool_add_task(&pool, mytask_run, (void*)arg);
	}
	//sleep(15);
	threadpool_destroy(&pool);
	return 0;
}















