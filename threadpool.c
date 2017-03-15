#include"h_threadpool.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<time.h>

void * pthread_routine(void *arg)
{
	struct timespec abstime;
	int timeout;
	printf("thread 0x%x is starting..... \n", (int)pthread_self());
	threadpool_t *pool =  (threadpool_t *)arg;	

	while(1)
	{
		timeout = 0;
		condtion_lock(&pool->ready);
		pool->idle++;
		//wait queue have task to or threadpool destroy inform
		while(pool->first == NULL && !pool->quit)	
		{
			printf("thread 0x%x is waiting..... \n", (int)pthread_self());
			//condtion_wait(&pool->ready);
			clock_gettime(CLOCK_REALTIME, &abstime);
			abstime.tv_sec +=2;
			int status = condtion_timedwait(&pool->ready, &abstime);
			if(status == ETIMEDOUT)
			{
				printf("thread 0x%x is wait timedwait..... \n", (int)pthread_self());
				timeout = 1;
				break;
			}
		}

		//weited to task, working
		pool->idle--;
		
		if(pool->first)
		{
			//from queue head hold out
			task_t * t = pool->first;
			pool->first = t->next;
			condtion_unlock(&pool->ready);

			t->run(t->arg);
			free(t);
		}
		if(pool->quit && pool->first == NULL)
		{
			pool->counter--;	
			if(pool->counter == 0)
				condtion_signal(&pool->ready);

			condtion_unlock(&pool->ready);
			break;
		}		
		if(timeout && pool->first == NULL)
		{
			pool->counter--;
                        condtion_unlock(&pool->ready);
                        break;
		}				
		condtion_unlock(&pool->ready);
	}	
	printf("thread 0x%x is exiting....\n", (int)pthread_self());
	return NULL;
}

void threadpool_init(threadpool_t *pool, int threads)
{
	condtion_init(&pool->ready);
	pool->first = NULL;
	pool->last = NULL;
	pool->counter = 0;
	pool->idle = 0;
	pool->max_threads = threads;
	pool->quit = 0;
}

void threadpool_add_task(threadpool_t *pool, void *(*run)(void *arg), void *arg)
{
	task_t *newtask = (task_t *)malloc(sizeof(task_t));
	newtask->run = run;
	newtask->arg = arg;
	newtask->next = NULL;

	condtion_lock(&pool->ready);
	
	//add task to queue
	if(pool->first == NULL)
		pool->first = newtask;
	else
		pool->last->next = newtask;   //problem if pool->last == NULL
	pool->last = newtask;

	// if have wait threads,wake up one of thread
	if(pool->idle > 0)
	{
		condtion_signal(&pool->ready);
	}
	else if(pool->counter < pool->max_threads)
	{
		// if not have wait thread and current thread number not beyond max_threads,end create thread
		pthread_t tid;
		pthread_create(&tid, NULL, pthread_routine, (void*) pool);

		pool->counter++;
	}
	condtion_unlock(&pool->ready);
}

void threadpool_destroy(threadpool_t *pool)
{
	if(pool->quit)
		return ;

	condtion_lock(&pool->ready);
	pool->quit = 1;
	if(pool->counter > 0)
	{
		if(pool->idle > 0)
			condtion_broadcast(&pool->ready);
		while(pool->counter > 0)
			condtion_wait(&pool->ready);
	}

	condtion_unlock(&pool->ready);
	condtion_destroy(&pool->ready);

	return;
}





