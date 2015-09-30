#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "os345queue.h"
#include "os345.h"
extern TCB tcb[MAX_TASKS];					// task control block


PQueue* initQueue(PQueue* pq)
{
	pq = (PQueue*)malloc(sizeof(PQueue));	//Changed after causing immense pain in lab 5 
	//not enough memory :()
	pq->queue_tasks = 0;
    //enQ(pq, 0, 0);
	return pq;
}


task enQ(PQueue* pq, int t_id, int prior)
{
	task to_add;
	to_add.tid = t_id;
	to_add.priority = prior;

	//insert at the given slot
	//printf("\nqueue_tasks %ld", pq->queue_tasks);


	pq->queue[pq->queue_tasks++] = to_add; //increment the index number

	//start the loop at the end of the queue 
	int i = pq->queue_tasks - 1;

	for(i = pq->queue_tasks -1; i >= 0; i--)
	{
		if(to_add.priority >= pq->queue[i].priority) //if the task to be added is lower prio than what to be added to
		{
			task marker = pq->queue[i]; //save what's in the slot
			
			if(pq->queue_tasks > 1)
			{
				pq->queue[i] = to_add; //replace it with what you're adding in
				pq->queue[i+1] = marker; //swap down the lower priority task through the queue.
			}
		}
	}
	return to_add;
}


task deQ(PQueue* pq, int tid)
{
	task to_remove;
	int index_to_remove = -1;

	int i = pq->queue_tasks - 1;
	for(i; i > -1; i--)
	{
		if(tid == pq->queue[i].tid) //if the task id is what i'm looking for...
		{
			to_remove = pq->queue[i];
			index_to_remove = i;
			//now from this index you need to copy upwards.

			int remover = i;
			for(remover; remover < pq->queue_tasks - 1; remover++)
			{
				pq->queue[remover] = pq->queue[remover + 1]; //copy from down lower on the list
			}

			pq->queue_tasks--;
			return to_remove;
		}
	}

	//if(index_to_remove == -1) //task not found
	//{
		to_remove.tid = -1;
		return to_remove;
	//}
}

//pop it off the top. in his slides he used the deQ(-1). this is less messy
task deQpop(PQueue* pq)
{
	task top;
	int i = 0;

	if(pq->queue_tasks == 0)
	{
		top.tid = -1;
		return top;
	}

	top = pq->queue[0];
	

	for(i; i < pq->queue_tasks - 1; i++)
	{
		pq->queue[i] = pq->queue[i+1];
	}

	pq->queue_tasks--;
	return top;
}


void printPQ(PQueue* pq)
{
	int i = 0;
	for(i; i < pq->queue_tasks; i++)
	{
		printf("\nPQ[%d] : tid: %d    priority: %d      time : %d", i,pq->queue[i].tid, pq->queue[i].priority, tcb[pq->queue[i].tid].time);
	}
}