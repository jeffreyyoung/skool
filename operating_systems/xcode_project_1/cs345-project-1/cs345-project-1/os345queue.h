#ifndef os345queue_h
#define os345queue_h

typedef struct 
{
	int tid;
	int priority;
} task;

typedef struct 
{
	task queue[512]; //array of 64 tasks
	int queue_tasks;
} PQueue;


PQueue* initQueue();
task enQ(PQueue* pq, int tid, int p);
task deQ(PQueue* pq, int tid);
task deQtop(PQueue* pq);
task deQpop(PQueue* pq);


#endif