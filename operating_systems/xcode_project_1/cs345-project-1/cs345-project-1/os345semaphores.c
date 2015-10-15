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
#include "PQueue.h"

extern TCB tcb[];							// task control block
extern int curTask;							// current task #
extern PQueue* rq;

extern int superMode;						// system mode
extern Semaphore* semaphoreList;			// linked list of active semaphores


// **********************************************************************
// **********************************************************************
// signal semaphore
//
//	if task blocked by semaphore, then clear semaphore and wakeup task
//	else signal semaphore
//
void semSignal(Semaphore* s)
{
    // assert there is a semaphore and it is a legal type
    assert("semSignal Error" && s && ((s->type == 0) || (s->type == 1)));
    
    // check semaphore type
    if (s->type == 0)
    {
        // binary semaphore
        // look through tasks for one suspended on this semaphore
        
        task tempT = deQ(s->q, -1);
        if(tempT.tid != -1){
            s->state = 0;
            tcb[tempT.tid].event = 0;			// clear event pointer
            tcb[tempT.tid].state = S_READY;	// unblock task
            enQ(rq, tempT.tid, tcb[tempT.tid].priority);
            if (!superMode) swapTask();
            return;
            
        }
        // nothing waiting on semaphore, go ahead and just signal
        s->state = 1;						// nothing waiting, signal
        if (!superMode) swapTask();
        return;
    }
    else
    {
        // counting semaphore
        // ?? implement counting semaphore
        //for (i=0; i<MAX_TASKS; i++)	// look for suspended task
        //{
        //CHANGED TO RESPECT QUEUE ON THE SEMAPHORE instead of TCB
        //TODO: ask Dr. Roper if this is OK
        
        
        task top = deQ(s->q, -1);
        int tempT = top.tid;
        if(tempT != -1){
            tcb[tempT].event = 0;			// clear event pointer
            tcb[tempT].state = S_READY;	// unblock task
            enQ(rq, tempT, tcb[tempT].priority);
            
            
            // ?? move task from blocked to ready queue
            
            s -> state++;
            if (!superMode) swapTask();
            return;
        }
        //nothing waiting on semaphore, increment the signal
        s-> state++;
        if(!superMode) swapTask();
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
    assert("semWait Error" && s);												// assert semaphore
    assert("semWait Error" && ((s->type == 0) || (s->type == 1)));	// assert legal type
    assert("semWait Error" && !superMode);								// assert user mode
    
    
    
    // check semaphore type
    if (s->type == 0)
    {
        // binary semaphore
        // if state is zero, then block task
        
        if (s->state == 0)
        {
            tcb[curTask].event = s;		// block task
            tcb[curTask].state = S_BLOCKED;
            
            // ?? move task from ready queue to blocked queue
            deQ(rq, curTask);
            enQ(s->q, curTask, tcb[curTask].priority);
            swapTask();						// reschedule the tasks
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
        if (s->state <= 0)
        {
            tcb[curTask].event = s;		// block task
            tcb[curTask].state = S_BLOCKED;
            
            //decrement the state to make sure we know there are tasks waiting on the semaphore
            s->state --;
            
            // ?? move task from ready queue to blocked queue
            deQ(rq, curTask);
            enQ(s->q, curTask, tcb[curTask].priority);
            swapTask();						// reschedule the tasks
            return 1;
        }
        
        s->state --;
        return 0;
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
        if(s->state <= 0){
            return 0;
        }
        //state is non-zero
        s->state --;
        return 1;
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
    sem->q = initQueue();						//initialize the task queue in this semaphore
    
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
