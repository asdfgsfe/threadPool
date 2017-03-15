#include"condtion.h"

int condtion_init(condtion_t *cond)
{
	int status;
	if((status = pthread_mutex_init(&cond->pmutex, NULL)))
		return status;

	if((status = pthread_cond_init(&cond->pcond, NULL)))
                return status;

	return 0;
}

int condtion_lock(condtion_t *cond)
{
	return pthread_mutex_lock(&cond->pmutex);
}

int condtion_unlock(condtion_t *cond)
{
        return pthread_mutex_unlock(&cond->pmutex);
}

int condtion_wait(condtion_t *cond)
{
        return pthread_cond_wait(&cond->pcond, &cond->pmutex);
}

int condtion_timedwait(condtion_t *cond, const struct timespec *abstime)
{
        return pthread_cond_timedwait(&cond->pcond, &cond->pmutex, abstime);
}

int condtion_signal(condtion_t *cond)
{
        return pthread_cond_signal(&cond->pcond);
}

int condtion_broadcast(condtion_t *cond)
{
        return pthread_cond_broadcast(&cond->pcond);
}

int condtion_destroy(condtion_t *cond)
{
        int status;
        if((status = pthread_mutex_destroy(&cond->pmutex)))
                return status;

        if((status = pthread_cond_destroy(&cond->pcond)))
                return status;

        return 0;
}

























