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
} PriorityQueue;


PriorityQueue* initQueue();
task enQ(PriorityQueue* pq, int tid, int p);
task deQ(PriorityQueue* pq, int tid);
task deQtop(PriorityQueue* pq);
task deQpop(PriorityQueue* pq);


#endif