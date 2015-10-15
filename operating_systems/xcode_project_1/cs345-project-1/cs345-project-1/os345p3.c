// os345p3.c - Jurassic Park
// ***********************************************************************
// **   DISCLAMER ** DISCLAMER ** DISCLAMER ** DISCLAMER ** DISCLAMER   **
// **                                                                   **
// ** The code given here is the basis for the CS345 projects.          **
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
#include "os345park.h"
#include "DeltaClock.h"

// ***********************************************************************
// project 3 variables

// Jurassic Park
extern JPARK myPark;
extern Semaphore* parkMutex;						// protect park access
extern Semaphore* fillSeat[NUM_CARS];			// (signal) seat ready to fill
extern Semaphore* seatFilled[NUM_CARS];		// (wait) passenger seated
extern Semaphore* rideOver[NUM_CARS];			// (signal) ride over
extern DeltaClock* deltaClock;
Semaphore* tickets;
Semaphore* spotsInGiftShop;
Semaphore* spotsInPark;
Semaphore* spotsInMuseum;
Semaphore* carSpotAvailable;
Semaphore* seatTaken;
Semaphore* vMailboxMutex; 	//mutex to keep mailbox in check
Semaphore* dMailboxMutex;
Semaphore* visitorTaken;	//mutex to signal when process has taken the visitor from the mailbox
Semaphore* wakeupDriver;
Semaphore* needDriver;
Semaphore* driverReady;
Semaphore* driverTaken;
Semaphore* needTicket;
Semaphore* ticketReady;

Semaphore* carNumMutex;
Semaphore* carNumReady;
Semaphore* carNumTaken;

Semaphore* interactingWithDriverMutex;

Semaphore* ticketTakerMutex;




Semaphore* loadMail;
Semaphore* driverMail;
int carNum;


// ***********************************************************************
// project 3 functions and tasks
void CL3_project3(int, char**);
void CL3_dc(int, char**);


// ***********************************************************************
// ***********************************************************************
// project3 command
int P3_project3(int argc, char* argv[])
{
    
    
    char buf[32];
    char* newArgv[2];
    int i; SWAP;    
    // start park
    sprintf(buf, "jurassicPark");
    newArgv[0] = buf;
    createTask( buf,				// task name
               jurassicTask,				// task
               MED_PRIORITY,				// task priority
               1,								// task count
               newArgv);					// task argument
    
    // wait for park to get initialized...
    while (!parkMutex) SWAP;
    printf("\nStart Jurassic Park...");
    
    //?? create car, driver, and visitor tasks here
    
    spotsInGiftShop = createSemaphore("spotsInGiftShop", COUNTING, MAX_IN_GIFTSHOP);	SWAP;
    spotsInMuseum = createSemaphore("spotsInMuseum", COUNTING, MAX_IN_MUSEUM);			SWAP;
    spotsInPark = createSemaphore("spotsInPark", COUNTING, MAX_IN_PARK);				SWAP;
    tickets = createSemaphore("tickets", COUNTING, MAX_TICKETS);						SWAP;
    
    vMailboxMutex = createSemaphore("vMailboxMutex", BINARY, 1); SWAP;
    visitorTaken = createSemaphore("visitorTaken", BINARY, 0); SWAP;
    
    carSpotAvailable = createSemaphore("carSpotAvailable", BINARY, 0); SWAP;
    seatTaken = createSemaphore("seatTaken", BINARY, 0);SWAP;
    
    needTicket = createSemaphore("needTicket", BINARY, 0); SWAP;
    
    wakeupDriver = createSemaphore("wakeupDriver", BINARY, 0); SWAP;
    needDriver = createSemaphore("needDriver", BINARY, 0); SWAP;
    
    
    
    ticketReady = createSemaphore("ticketReady", BINARY, 0); SWAP;
    
    driverTaken = createSemaphore("driverTaken", BINARY, 0); SWAP;
    driverReady = createSemaphore("driverReady", BINARY, 0); SWAP;
    
    dMailboxMutex = createSemaphore("dMailboxSemaphore", BINARY, 1);	SWAP;
    
    carNumMutex = createSemaphore("carNumMutex", BINARY, 1);	SWAP;
    carNumReady = createSemaphore("carNumReady", BINARY, 0);	SWAP;
    carNumTaken = createSemaphore("carNumTaken", BINARY, 0);	SWAP;
    
    ticketTakerMutex = createSemaphore("ticketTakerMutex", BINARY, 1); SWAP;
    interactingWithDriverMutex = createSemaphore("interactingWithDriverMutex", BINARY, 1); SWAP;
    
    int numVisitors = NUM_VISITORS; SWAP;
    if(argv[1] != 0){ SWAP;
        numVisitors = atoi(argv[1]); SWAP;
    }
    
    //create visitor tasks
    for(i = 0; i < numVisitors; i++){ SWAP;
        sprintf(buf, "visitor%d", i); SWAP;
        newArgv[0] = buf; SWAP;
        char str[15]; SWAP;
        sprintf(str, "%d", i); SWAP;
        newArgv[1] = str; SWAP;
        createTask(buf, P3_visitor, MED_PRIORITY, 2, newArgv); SWAP;
    }
    
    //create driver tasks
    for(i = 0; i < NUM_DRIVERS; i++){ SWAP;
        sprintf(buf, "driver%d", i); SWAP;
        newArgv[0] = buf; SWAP;
        char str[15]; SWAP;
        sprintf(str, "%d", i); SWAP;
        newArgv[1] = str; SWAP;
        createTask(buf, P3_driver, MED_PRIORITY, 2, newArgv);	SWAP;
    }
    
    //create car tasks
    for(i = 0; i < NUM_CARS; i++){ SWAP;
        sprintf(buf, "car%d", i); SWAP;
        newArgv[0] = buf; SWAP;
        char str[15]; SWAP;
        sprintf(str, "%d", i); SWAP;
        newArgv[1] = str; SWAP;
        createTask(buf, P3_car, MED_PRIORITY, 2, newArgv);	SWAP;
    }
    
    
    return 0;
} // end project3

int P3_car(int argc, char* argv[]){ SWAP;
    
    int carID= atoi(argv[1]); SWAP;
    carID +=1;
    Semaphore* rideDone[NUM_SEATS]; SWAP;
    Semaphore* driverDone; SWAP;

    int i;
    while(1){
        //clear the semaphores
//        for(i = 0; i < NUM_SEATS; i++){
//            rideDone[i] = 0;	SWAP;
//        }
//        driverDone = 0;	SWAP;
        
        
        for(i = 0; i < NUM_SEATS; i++){ SWAP;
            //semWait(fillSeat[carID]); SWAP;
            semSignal(carSpotAvailable);	SWAP;
            semWait(seatTaken);	SWAP;
            
            //take visitor's semaphore from the mailbox
            rideDone[i] = loadMail;	SWAP;
            semSignal(visitorTaken);	SWAP;
            
            if(i == 2){ SWAP;
                semSignal(needDriver);	SWAP;
                semSignal(wakeupDriver);	SWAP;
                
                semWait(driverReady);	SWAP;
                driverDone = driverMail;	SWAP;
                semSignal(driverTaken);	SWAP;
                
                semWait(carNumMutex);	SWAP;
                carNum = carID;	SWAP;
                semSignal(carNumReady);	SWAP;
                semWait(carNumTaken);	SWAP;
                semSignal(carNumMutex);	SWAP;
            }
            semSignal(seatFilled[carID]); SWAP;
        }
        semWait(rideOver[carID]);	SWAP;
        
        //release the visitors and driver
        for(i = 0; i < 3; i++){ SWAP;
            semSignal(rideDone[i]);	SWAP;
        }
        semSignal(driverDone);	SWAP;
    }
    
    return 0;
}

int P3_driver(int argc, char* argv[]){	SWAP;
    char buf[32];	SWAP;
    Semaphore* driverDone;	SWAP;
    int myID = atoi(argv[1]);	SWAP;
    printf(buf, "Starting driverTask %d", myID);	SWAP;
    sprintf(buf, "driverDone%d", myID);	SWAP;
    driverDone = createSemaphore(buf, BINARY, 0);	SWAP;
    
    while(1){
        semWait(wakeupDriver);	SWAP;
        
        
        
        
        if(semTryLock(needDriver)){ SWAP;
            
            semWait(dMailboxMutex);	SWAP;
            
            driverMail = driverDone;	SWAP;
            semSignal(driverReady);	SWAP;
            semWait(driverTaken); SWAP;
            driverMail = 0;	SWAP;
            
            //take the car number
            semWait(carNumReady); SWAP;
            semWait(parkMutex);	SWAP;
            myPark.drivers[myID] = carNum + 1;	SWAP;
            semSignal(parkMutex);	SWAP;
            semSignal(carNumTaken);	SWAP;
            semSignal(dMailboxMutex); SWAP;
            
            semWait(driverDone); SWAP;
        }
        else if(semTryLock(needTicket)){	SWAP;
            semWait(ticketTakerMutex);	SWAP;
            myPark.drivers[myID] = -1;	SWAP;
            semWait(parkMutex);	SWAP;
            
            semSignal(parkMutex);	SWAP;
            semWait(tickets);	SWAP;
            semSignal(ticketReady);	SWAP;
            semSignal(ticketTakerMutex);	SWAP;
            
        }
        
        semWait(parkMutex); SWAP;
        myPark.drivers[myID] = 0; SWAP;
        semSignal(parkMutex); SWAP;
        
    }
    
    return 0;
    
}



// ***********************************************************************
// ***********************************************************************
// delta clock command
int P3_dc(int argc, char* argv[])
{
    printf("\nDelta Clock"); SWAP;
    printDeltaClock(deltaClock);
    return 0;
} // end CL3_dc

int P3_visitor(int argc, char* argv[]){
    char buf[32];	SWAP;
    sprintf(buf, "visitor%d", atoi(argv[1]));	SWAP;
    Semaphore* visitorSem = createSemaphore(buf, BINARY, 0);	SWAP;
    
    //initialize random timing!
    int ticsB4Arrive = rand() % 100;	SWAP;
    int ticsB4EntranceRqTicket = rand() % 30; SWAP;
    int ticsB4MuseumRqEntry = rand() % 30;	SWAP;
    int ticsB4GSRqEntry = rand() % 30;	SWAP;
    int ticsInMuseum = rand() % 30; SWAP;
    int ticsInGS = rand() % 30; SWAP;
    int ticsB4CarRqEntry = rand() % 30; SWAP;
    
    //WAIT TO ARRIVE AT THE PARK
    insertDeltaClock(ticsB4Arrive, visitorSem);	SWAP;
    semWait(visitorSem);	SWAP;
    
    semWait(parkMutex);	SWAP;
    myPark.numOutsidePark++; SWAP;
    semSignal(parkMutex);		SWAP;
    
    semWait(spotsInPark);
    
    semWait(parkMutex); SWAP;
    myPark.numOutsidePark--;	SWAP;
    myPark.numInPark++; SWAP;
    myPark.numInTicketLine++;	SWAP;
    semSignal(parkMutex);	SWAP;
    
    //WAIT TO GET A TICKET
    insertDeltaClock(ticsB4EntranceRqTicket, visitorSem);	SWAP;
    semWait(visitorSem);	SWAP;
    
    semWait(interactingWithDriverMutex); SWAP;
    semSignal(needTicket);	SWAP;
    semSignal(wakeupDriver); SWAP;
    semWait(ticketReady);	SWAP;
    semWait(parkMutex); SWAP;
    myPark.numTicketsAvailable--; SWAP;
    semSignal(parkMutex); SWAP;
    semSignal(interactingWithDriverMutex); SWAP;
    
    
    semWait(parkMutex);	SWAP;
    myPark.numInTicketLine--;	SWAP;
    myPark.numInMuseumLine++;	SWAP;
    semSignal(parkMutex); SWAP;
    
    //WAIT IN LINE NEAR THE MUSEUM
    insertDeltaClock(ticsB4MuseumRqEntry, visitorSem);	SWAP;
    semWait(visitorSem); SWAP;
    
    //WAIT UNTIL THERE ARE SPOTS IN THE MUSEUM
    semWait(spotsInMuseum); SWAP;
    
    semWait(parkMutex);	SWAP;
    myPark.numInMuseumLine--;	SWAP;
    myPark.numInMuseum++;	SWAP;
    semSignal(parkMutex); SWAP;
    
    //VISIT THE MUSEUM FOR A WHILE
    insertDeltaClock(ticsInMuseum, visitorSem); SWAP;
    semWait(visitorSem); SWAP;
    
    semWait(parkMutex); SWAP;
    myPark.numInMuseum--;	SWAP;
    myPark.numInCarLine++;	SWAP;
    semSignal(parkMutex); SWAP;
    
    semSignal(spotsInMuseum); SWAP;
    
    insertDeltaClock(ticsB4CarRqEntry, visitorSem); SWAP;
    semWait(visitorSem);	SWAP;
    
    
    //wait until there is a spot available in a car
    semWait(carSpotAvailable); SWAP;
    
    //consume the key for the mailbox mutex
    semWait(vMailboxMutex);	SWAP;
    sprintf(buf, "rideDoneVisitor%d", atoi(argv[1]));	SWAP;
    Semaphore* rideDone = createSemaphore(buf, BINARY, 0);	SWAP;
    loadMail = rideDone;	SWAP;
    semSignal(seatTaken); SWAP;
    semWait(visitorTaken); SWAP;
    loadMail = 0; SWAP;
    semSignal(vMailboxMutex); SWAP;
    
    //return ticket
    semSignal(tickets); SWAP;
    semWait(parkMutex); SWAP;
    myPark.numTicketsAvailable++; SWAP;
    semSignal(parkMutex); SWAP;
    
    
    //update the dcount
    semWait(parkMutex); SWAP;
    myPark.numInCarLine--; SWAP;
    myPark.numInCars++; SWAP;
    semSignal(parkMutex); SWAP;
    
    //wait until the ride is done
    semWait(rideDone); SWAP;
    
    semWait(parkMutex); SWAP;
    myPark.numInCars--; SWAP;
    myPark.numInGiftLine++; SWAP;
    semSignal(parkMutex); SWAP;
    
    //wait in line at gift shop
    insertDeltaClock(ticsB4GSRqEntry, visitorSem); SWAP;
    semWait(visitorSem);	SWAP;
    
    semWait(spotsInGiftShop); SWAP;
    
    semWait(parkMutex); SWAP;
    myPark.numInGiftLine--; SWAP;
    myPark.numInGiftShop++; SWAP;
    semSignal(parkMutex); SWAP;
    
    insertDeltaClock(ticsInGS, visitorSem); SWAP;
    semWait(visitorSem); SWAP;
    
    semSignal(spotsInGiftShop); SWAP;
    
    semWait(parkMutex); SWAP;
    myPark.numInGiftShop--;	SWAP;
    myPark.numInPark--; SWAP;
    myPark.numExitedPark++;
    semSignal(parkMutex); SWAP;
    
    semSignal(spotsInPark); SWAP;
    semSignal(tickets); SWAP;
    
    
    
    return 0;
    
}


/*
 // ***********************************************************************
 // ***********************************************************************
 // ***********************************************************************
 // ***********************************************************************
 // ***********************************************************************
 // ***********************************************************************
 // delta clock command
 int P3_dc(int argc, char* argv[])
 {
	printf("\nDelta Clock");
	// ?? Implement a routine to display the current delta clock contents
	//printf("\nTo Be Implemented!");
	int i;
	for (i=0; i<numDeltaClock; i++)
	{
 printf("\n%4d%4d  %-20s", i, deltaClock[i].time, deltaClock[i].sem->name);
	}
	return 0;
 } // end CL3_dc
 
 
 // ***********************************************************************
 // display all pending events in the delta clock list
 void printDeltaClock(void)
 {
	int i;
	for (i=0; i<numDeltaClock; i++)
	{
 printf("\n%4d%4d  %-20s", i, deltaClock[i].time, deltaClock[i].sem->name);
	}
	return;
 }
 
 
 // ***********************************************************************
 // test delta clock
 int P3_tdc(int argc, char* argv[])
 {
	createTask( "DC Test",			// task name
 dcMonitorTask,		// task
 10,					// task priority
 argc,					// task arguments
 argv);
 
	timeTaskID = createTask( "Time",		// task name
 timeTask,	// task
 10,			// task priority
 argc,			// task arguments
 argv);
	return 0;
 } // end P3_tdc
 
 
 
 // ***********************************************************************
 // monitor the delta clock task
 int dcMonitorTask(int argc, char* argv[])
 {
	int i, flg;
	char buf[32];
	// create some test times for event[0-9]
	int ttime[10] = {
 90, 300, 50, 170, 340, 300, 50, 300, 40, 110	};
 
	for (i=0; i<10; i++)
	{
 sprintf(buf, "event[%d]", i);
 event[i] = createSemaphore(buf, BINARY, 0);
 insertDeltaClock(ttime[i], event[i]);
	}
	printDeltaClock();
 
	while (numDeltaClock > 0)
	{
 SEM_WAIT(dcChange)
 flg = 0;
 for (i=0; i<10; i++)
 {
 if (event[i]->state ==1)			{
 printf("\n  event[%d] signaled", i);
 event[i]->state = 0;
 flg = 1;
 }
 }
 if (flg) printDeltaClock();
	}
	printf("\nNo more events in Delta Clock");
 
	// kill dcMonitorTask
	tcb[timeTaskID].state = S_EXIT;
	return 0;
 } // end dcMonitorTask
 
 
 extern Semaphore* tics1sec;
 
 // ********************************************************************************************
 // display time every tics1sec
 int timeTask(int argc, char* argv[])
 {
	char svtime[64];						// ascii current time
	while (1)
	{
 SEM_WAIT(tics1sec)
 printf("\nTime = %s", myTime(svtime));
	}
	return 0;
 } // end timeTask
 */

