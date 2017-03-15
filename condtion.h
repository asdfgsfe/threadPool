#ifndef _CONDTION_H_
#define _CONDTION_H_

#include<pthread.h>

typedef struct condtion
{
	pthread_mutex_t pmutex;
	pthread_cond_t pcond;
} condtion_t;

int condtion_init(condtion_t *cond);
int condtion_lock(condtion_t *cond);
int condtion_unlock(condtion_t *cond);
int condtion_wait(condtion_t *cond);
int condtion_timedwait(condtion_t *cond, const struct timespec *abstime);
int condtion_signal(condtion_t *cond);
int condtion_broadcast(condtion_t *cond);
int condtion_destroy(condtion_t *cond);

#endif

