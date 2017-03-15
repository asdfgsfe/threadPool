#include"condtion.h"

typedef struct task
{
	void *(*run)(void *arg);
	void *arg;
	struct task *next;
} task_t;

typedef struct threadpool
{
	condtion_t ready;
	task_t *first;
	task_t *last;
	int counter;
	int idle;
	int max_threads;
	int quit;
} threadpool_t;

void threadpool_init(threadpool_t *pool, int threads);
void threadpool_add_task(threadpool_t *pool, void *(*run)(void *arg), void *arg);
void threadpool_destroy(threadpool_t *pool);



