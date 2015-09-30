// os345semaphores.c - OS Semaphores
// ***********************************************************************
// **   DISCLAMER ** DISCLAMER ** DISCLAMER ** DISCLAMER ** DISCLAMER   **
// **                                                                   **
// ** The code given here is the basis for the BYU CS345 projects.      **
// ** It comes "as is" and "unwarranted."  As such, when you use part   **
// ** or all of the code, it becomes "yours" and you are responsible to **
// ** understand any algorithm or method presented.  Likewise, any      **
// ** errors or problems become your responsibility to fix.             **
// **                                                                   **
// ** NOTES:                                                            **
// ** -Comments beginning with "// ??" may require some implementation. **
// ** -Tab stops are set at every 3 spaces.                             **
// ** -The function API's in "OS345.h" should not be altered.           **
// **                                                                   **
// **   DISCLAMER ** DISCLAMER ** DISCLAMER ** DISCLAMER ** DISCLAMER   **
// ***********************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <time.h>
#include <assert.h>
#include "os345.h"


extern TCB tcb[];							// task control block
extern int curTask;							// current task #

extern int superMode;						// system mode
extern Semaphore* semaphoreList;			// linked list of active semaphores
extern PQueue* rq;

// **********************************************************************
// **********************************************************************
// signal semaphore
//
//	if task blocked by semaphore, then clear semaphore and wakeup task
//	else signal semaphore
//

void semSignal(Semaphore* s)
{
    int i;
    // assert there is a semaphore and it is a legal type
    assert("semSignal Error" && s);
    assert("semSignal Error" && ((s->type == 0) || (s->type == 1)));
    // check semaphore type
    if (s->type == 0)
    {
        // binary semaphore
        // look through tasks for one suspended on this semaphore
        task top;
        top = deQpop(s->q); // take off the top of the blocked queue
        
        if(top.tid != -1)
        {
            // ?? move task from blocked to ready queue
            //printf("\n*Added to RQ %ld", top.tid);
            enQ(rq, top.tid, top.priority);
            s->state = 0;				// clear semaphore
            tcb[top.tid].event = 0;			// clear event pointer
            tcb[top.tid].state = S_READY;	// unblock task
        }
        else
        {
            s->state = 1;	//if the top task is invalid, change it to 1
        }
        if (!superMode) swapTask();
        return;
        
        // nothing waiting on semaphore, go ahead and just signal
        s->state = 1;						// nothing waiting, signal
        
    }
    else
    {
        // counting semaphore
        // ?? implement counting semaphore
        if(curTask == -1 && s->state < -1) return; //if your curTask isn't assig, and things are in the blocked queue
        
        s->state++;
        task top = deQpop(s->q);
        
        if(top.tid != -1)
        {
            tcb[top.tid].event = 0; //clear event pointer
            tcb[top.tid].state = S_READY; //unblock task
            enQ(rq, top.tid, top.priority); //
        }
        
        if (!superMode) swapTask();
        return;
    }
} // end semSignal



// **********************************************************************
// **********************************************************************
// wait on semaphore
//
//	if semaphore is signaled, return immediately
//	else block task
//
int semWait(Semaphore* s)
{
    //assert("semWait Error" && s);												// assert semaphore
    assert("semWait Error" && ((s->type == 0) || (s->type == 1)));	// assert legal type
    assert("semWait Error" && !superMode);								// assert user mode
    
    // check semaphore type
    //printf("SEM : %s  %d %d\n", s->name,s->state, s->type);
    
    if (s->type == 0)
    {
        // binary semaphore
        // if state is zero, then block task
        
        if (s->state == 0) // block task
        {
            tcb[curTask].event = s;		// block task
            tcb[curTask].state = S_BLOCKED;
            
            // ?? move task from ready queue to blocked queue
            task top;
            top = deQ(rq, curTask); // take curTask from ready queue to blocked queue
            
            if(top.tid != -1) //if the task is valid, enQ it
            {
                //printf("$$Adding %d to Blocked Q\n", curTask);
                enQ(s->q, curTask, top.priority);
                //	s->state = 0; 					//reset state, and don't block
            }
            
            swapTask();					// reschedule the tasks
            return 1;
            
        }
        // state is non-zero (semaphore already signaled)
        s->state = 0;						// reset state, and don't block
        return 0;
    }
    
    
    else
    {
        // counting semaphore
        // ?? implement counting semaphore
        
        if((s->state) < 1) //if there are things in the blocked queue
        {
            task top;
            top = deQpop(rq);
            tcb[top.tid].event = s;
            tcb[top.tid].state = S_BLOCKED; //blocking of task
            if(top.tid != -1)
            {
                enQ(s->q, curTask, top.priority);
                s->state--;

            }
            swapTask();
            return 1;
        }
        s->state--;
        return 0;
        
        //goto temp;
    }
} // end semWait



// **********************************************************************
// **********************************************************************
// try to wait on semaphore
//
//	if semaphore is signaled, return 1
//	else return 0
//
int semTryLock(Semaphore* s)
{
    assert("semTryLock Error" && s);												// assert semaphore
    assert("semTryLock Error" && ((s->type == 0) || (s->type == 1)));	// assert legal type
    assert("semTryLock Error" && !superMode);									// assert user mode
    
    // check semaphore type
    if (s->type == 0)
    {
        // binary semaphore
        // if state is zero, then block task
        
        //temp:	// ?? temporary label
        if (s->state == 0)
        {
            return 0;
        }
        // state is non-zero (semaphore already signaled)
        s->state = 0;						// reset state, and don't block
        return 1;
    }
    else
    {
        // counting semaphore
        // ?? implement counting semaphore
        if(s->state < 0)
        {
            return 0;
        }
        s->state--;
        return 1;
        //goto temp;
    }
} // end semTryLock


// **********************************************************************
// **********************************************************************
// Create a new semaphore.
// Use heap memory (malloc) and link into semaphore list (Semaphores)
// 	name = semaphore name
//		type = binary (0), counting (1)
//		state = initial semaphore state
// Note: memory must be released when the OS exits.
//
Semaphore* createSemaphore(char* name, int type, int state)
{
    Semaphore* sem = semaphoreList;
    Semaphore** semLink = &semaphoreList;
    
    // assert semaphore is binary or counting
    assert("createSemaphore Error" && ((type == 0) || (type == 1)));	// assert type is validate
    
    // look for duplicate name
    while (sem)
    {
        if (!strcmp(sem->name, name))
        {
            printf("\nSemaphore %s already defined", sem->name);
            
            // ?? What should be done about duplicate semaphores ??
            // semaphore found - change to new state
            sem->type = type;					// 0=binary, 1=counting
            sem->state = state;				// initial semaphore state
            sem->taskNum = curTask;			// set parent task #
            return sem;
        }
        // move to next semaphore
        semLink = (Semaphore**)&sem->semLink;
        sem = (Semaphore*)sem->semLink;
    }
    
    // allocate memory for new semaphore
    sem = (Semaphore*)malloc(sizeof(Semaphore));
    
    // set semaphore values
    sem->name = (char*)malloc(strlen(name)+1);
    strcpy(sem->name, name);				// semaphore name
    sem->type = type;							// 0=binary, 1=counting
    sem->state = state;						// initial semaphore state
    sem->taskNum = curTask;					// set parent task #
    sem->q = initQueue(sem->q);				//****Mallocing the semaphore priority queue
    
    // prepend to semaphore list
    sem->semLink = (struct semaphore*)semaphoreList;
    semaphoreList = sem;						// link into semaphore list
    return sem;									// return semaphore pointer
} // end createSemaphore



// **********************************************************************
// **********************************************************************
// Delete semaphore and free its resources
//
bool deleteSemaphore(Semaphore** semaphore)
{
    Semaphore* sem = semaphoreList;
    Semaphore** semLink = &semaphoreList;
    
    // assert there is a semaphore
    assert("deleteSemaphore Error" && *semaphore);
    
    // look for semaphore
    while(sem)
    {
        if (sem == *semaphore)
        {
            // semaphore found, delete from list, release memory
            *semLink = (Semaphore*)sem->semLink;
            
            // free the name array before freeing semaphore
            printf("\ndeleteSemaphore(%s)", sem->name);
            
            // ?? free all semaphore memory
            // ?? What should you do if there are tasks in this
            //    semaphores blocked queue????
            free(sem->name);
            free(sem);
            
            return TRUE;
        }
        // move to next semaphore
        semLink = (Semaphore**)&sem->semLink;
        sem = (Semaphore*)sem->semLink;
    }
    
    // could not delete
    return FALSE;
} // end deleteSemaphore
