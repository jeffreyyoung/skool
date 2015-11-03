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
extern DeltaClock* deltaClock;							//DeltaClock for random timing
extern Semaphore* DClockMutex;					//Blocking for Clock / UnBlocking - this is really just for lines

//given mutexes
extern JPARK myPark;
extern Semaphore* parkMutex;					//for critical Data
extern Semaphore* fillSeat[NUM_CARS];			//signal empty seats in the car
extern Semaphore* seatFilled[NUM_CARS];			//signal seat taken in the car
extern Semaphore* rideOver[NUM_CARS];			//signal car and driver done

//Mutexes
Semaphore* driverMutex;
Semaphore* visitorMutex;


//Tour Car Semaphores
Semaphore* sigPassenger;						//signal to get a Passenger for the ride
Semaphore* seatTaken;							//signal that one seat is taken
Semaphore* rideDone[MAX_TASKS];					//signal a ride is done

Semaphore* deltaItemExpired[MAX_TASKS];				//signal timing event

Semaphore* passengerSeated;

Semaphore* spotInPark;
Semaphore* spotInGiftShop;
Semaphore* spotInMuseum;


//driver semaphores
Semaphore* driverDoneSemaphore;
Semaphore* driverReady;

//drivers and tickets
Semaphore* carTicket;
Semaphore* driver;
Semaphore* needDriverMutex;
Semaphore* needTicket;
Semaphore* takeTicket;

//Need Variables for visitorID, currCarID, driverID
//code could be improved by using helper functions in your visitorFunction, but meh.


int visitorID;
int currCarID;
int driverID = 0;
int visitor_count = 45;
//everything's 10 basically.
#define MAX_ENTRANCE_TIME 10			// in seconds
#define MAX_LINE_TIME 3			// in seconds
#define MAX_GIFT_SHOP_TIME 10			// in seconds
#define MAX_MUSEUM_TIME 10			// in seconds
#define MAX_TIME 10

// ***********************************************************************
// project 3 functions and tasks
int P3_carTask(int argc, char* argv[]);
int P3_visitorTask(int argc, char* argv[]);
int P3_driverTask(int argc, char* argv[]);

//Note: This lab isn't too bad. make sure you understand semaphores and that they're implemented correctly
// ~ 10-12 hours
// for testing, relied heavily on sem to see when things were blocked and they shouldn't have been
// compare sem states frequently for starvation and deadlock cases


int P3_project3(int argc, char* argv[])
{
    char buf[32];SWAP;
    char buf2[32];SWAP;
    char* newArgv[2];																SWAP;
    sprintf(buf, "jurassicPark");													SWAP;
    newArgv[0] = buf;																SWAP;
    
    //init jurassicPark task
    createTask(buf,	jurassicTask, MED_PRIORITY,	1, newArgv);						SWAP;												SWAP;		// task argument
    
    //INIT SEMAPHORES
    //init mutexes to 1
    driverMutex = createSemaphore("Driver Mutex", BINARY, 1);						SWAP;	//unblocked to start
    passengerSeated = createSemaphore("passengerSeated", BINARY, 1);				SWAP;	//none seated.
    visitorMutex = createSemaphore("visitorMutex", BINARY, 1);						SWAP;	//unblocked to start
    
    sigPassenger = createSemaphore("sigPassenger", BINARY, 0);						SWAP;
    seatTaken = createSemaphore("seatTaken", BINARY, 0);							SWAP;
    spotInPark = createSemaphore("spotInPark", COUNTING, MAX_IN_PARK);				SWAP;
    spotInGiftShop = createSemaphore("spotInGiftShop", COUNTING, MAX_IN_GIFTSHOP);	SWAP;
    spotInMuseum = createSemaphore("spotInMuseum", COUNTING, MAX_IN_MUSEUM);		SWAP;
    carTicket = createSemaphore("museumTicket", COUNTING, MAX_TICKETS);				SWAP;
    
    //driver semaphores
    driverReady = createSemaphore("Driver Ready", BINARY, 0);						SWAP;
    driver = createSemaphore("Driver", BINARY, 0);									SWAP;
    needDriverMutex = createSemaphore("Need Driver", BINARY, 0);							SWAP;
    needTicket = createSemaphore("Need Ticket", BINARY, 0);							SWAP;
    takeTicket = createSemaphore("Take Ticket", BINARY, 0);							SWAP;
    
    // wait for park to get initialized...
    while (!parkMutex)																SWAP;
    printf("\nStart Jurassic Park...");												SWAP;
    
    // create car, driver, and visitor tasks here
    int id;																			SWAP;
    
    id = 0;																			SWAP;
    //Name the tasks and create the
    
    //Drivers
    for (id = 1; id <= NUM_DRIVERS; id++)
    {
        sprintf(buf, "Driver %d", id);												SWAP; //create task for each driver
        sprintf(buf2, "%d", id);
        newArgv[0] = buf;																SWAP;
        newArgv[1] = buf2;														SWAP;
        createTask(buf, P3_driverTask, MED_PRIORITY, 0, 0);							SWAP;
        driverID++;																	SWAP;
    }
    
    //visitor tasks
    for (id = 0; id < visitor_count; id++)
    {
        sprintf(buf, "visitor %d", id);												SWAP; //create task for each driver
        sprintf(buf2, "%d", id);
        newArgv[0] = buf;																SWAP;
        newArgv[1] = buf2;														SWAP;
        createTask(buf, P3_visitorTask, MED_PRIORITY, 2, newArgv);					SWAP; //create visitor tasks for everyone
    }
    
    //create TourCar tasks
    sprintf(buf, "carTask");														SWAP;
    for (id = 0; id < NUM_CARS; id++)
    {
        sprintf(buf, "car %d", id);												SWAP; //create task for each driver
        sprintf(buf2, "%d", id);
        newArgv[0] = buf;																SWAP;
        newArgv[1] = buf2;														SWAP;
        createTask(buf, P3_carTask, MED_PRIORITY, 2, newArgv);						SWAP; //create tasks for each car
    }
    
    return 0;
} // end project3


//testing your DC. Roper gives you a test function already. it's nice.
int P3_dc(int argc, char* argv[]) //testing for DC - prints out the clock
{
    printf("\nDelta Clock\n");														SWAP;
    printDeltaClock(deltaClock);												SWAP;
    return 0;
}

//fillSeat[NUM_CARS]
//seatFilled[NUM_CARS]
//rideOver[NUM_CARS]
int P3_carTask(int argc, char* argv[])
{
    int i;																SWAP;
    Semaphore* carRideDone[NUM_SEATS + 1]; // is this right? NUM_SEATS or NUM_SEATS + 1
    SWAP;
    int carId = atoi(argv[1]);														SWAP;
    printf("carId %d\n", carId);													SWAP;
    
    while (1)
    {
        //from the slides
        //for each car seat:
        //sem_wait(fillSeat[carId])
        //save passenger rideDone[] semaphore
        //get driver (if last passenger)
        //save driver driverDone semaphore
        //semsignal(seatFilled[carID)])
        
        
        for (i = 0; i < NUM_SEATS; i++) //for each passenger in the car
        {
            printf("\n\n\nHERE!!!!!\n\n\n\n");
            semWait(fillSeat[carId]);												SWAP;
            printf("\n\n\nHERE!!!!!\n\n\n");
            semSignal(sigPassenger);												SWAP;	//signal passenger got dec num
            semWait(seatTaken);														SWAP;	//block
            
            // save passenger ride over semaphore using temporary ride sem
            carRideDone[i] = rideDone[visitorID];									SWAP;
            
            semSignal(passengerSeated);												SWAP;	// visitor seated
            
            if (i == NUM_SEATS - 1) //if you're the last passenger in the car
            {
                SEM_WAIT(driverMutex);												SWAP;
                SEM_SIGNAL(needDriverMutex);												SWAP;
                currCarID = carId;													SWAP;
                SEM_SIGNAL(driver);													SWAP;
                SEM_WAIT(driverReady);												SWAP;
                carRideDone[3] = driverDoneSemaphore;								SWAP;
                SEM_SIGNAL(driverMutex);											SWAP;
            }
            
            //sem seat filled
            semSignal(seatFilled[carId]);											SWAP;
            
            //block and update park vars
            semWait(parkMutex);														SWAP;
            myPark.numInCarLine--;													SWAP;
            myPark.numInCars++;														SWAP;
            semSignal(parkMutex);													SWAP;
        }
        
        //Ride Complete
        semWait(rideOver[carId]);													SWAP;
        
        //once ride is over
        //	sem_wait(rideOver)
        
        // Release passengers
        for (i = 0; i < NUM_SEATS; i++)
        {
            //Block and update park vars
            semWait(parkMutex);														SWAP;
            myPark.numInCars--;														SWAP;
            myPark.numInGiftLine++;													SWAP;
            semSignal(parkMutex);													SWAP;
            
            //Specifiy Car Ride
            semSignal(carRideDone[i]);												SWAP;
        }
        //seat 3 is the driver, pass 4
        //Release driver
        semSignal(carRideDone[3]);													SWAP;
    }
}


//from slides
//	1. make sure that everything is created with the same priority - DONE
//
//this function is on the slides. I really just tweaked what he had there.
//hopefully naming isn't confusing.
int P3_driverTask(int argc, char* argv[])
{
    char name[32];
    int id = driverID;
    sprintf(name, "DriverFinished %d", id);
    Semaphore* driverDone = createSemaphore(name, BINARY, 0);
    
    while (1)
    {
        
        SEM_WAIT(parkMutex);														SWAP;
        myPark.drivers[id] = 0;														SWAP;
        SEM_SIGNAL(parkMutex);														SWAP;
        SEM_WAIT(driver);															SWAP;
        
        //when a driver is awakened, use the semTryLockFunction to determine if a driver or a ticket seller is needed
        if (semTryLock(needDriverMutex))
        {
            driverDoneSemaphore = driverDone;										SWAP;
            
            //block while update
            SEM_WAIT(parkMutex);													SWAP;
            myPark.drivers[id] = currCarID + 1;										SWAP;
            //update your cur drivers ID
            SEM_SIGNAL(parkMutex);													SWAP;
            
            //sig driver ready and wait for done
            SEM_SIGNAL(driverReady);												SWAP; //driver ready - go on ride
            SEM_WAIT(driverDone);													SWAP; // driver done - finish loop
        }
        else if (semTryLock(needTicket))
        {
            //if you need a ticket, you don't need a driver, change driver to -1
            SEM_WAIT(parkMutex);													SWAP;
            myPark.drivers[id] = -1;												SWAP;
            SEM_SIGNAL(parkMutex);													SWAP;
            SEM_SIGNAL(takeTicket);													SWAP;
            //sig ticket done
        }
        else break;
    }
    return 0;
}


int P3_visitorTask(int argc, char* argv[])
{
    int visitorId = atoi(argv[1]);														SWAP;	//get visitor ID
    char buf[32];
    
    sprintf(buf, "deltaItemExpired%d", visitorId);											SWAP;
    deltaItemExpired[visitorId] = createSemaphore(buf, BINARY, 0);							SWAP;	//print out at time events
    
    
    sprintf(buf, "RideComplete%d", visitorId);										SWAP;
    rideDone[visitorId] = createSemaphore(buf, BINARY, 0);							SWAP;	//print out when ride is over
    
    //block and update park
    semWait(parkMutex);																SWAP;
    myPark.numOutsidePark++;														SWAP;
    semSignal(parkMutex);															SWAP;
    
    //waiting outside clock
    int waitTime = rand() % (MAX_TIME * 10) + 1;									SWAP;
    insertDeltaClock(waitTime, deltaItemExpired[visitorId]);							SWAP;
    semWait(deltaItemExpired[visitorId]);													SWAP;
    semWait(spotInPark);															SWAP;	//block spot in park, -1
    
    
    //block park and update stats
    semWait(parkMutex);																SWAP;
    myPark.numOutsidePark--;														SWAP;
    myPark.numInPark++;																SWAP;
    myPark.numInTicketLine++;														SWAP;
    semSignal(parkMutex);															SWAP;
    
    //call visitor functions
    
    //*******************************************************************
    //ticketBooth
    //wait for ticket
    //Tickets
    waitTime = rand() % (MAX_LINE_TIME * 10) + 1;								SWAP;
    insertDeltaClock(waitTime, deltaItemExpired[visitorId]);							SWAP;
    semWait(deltaItemExpired[visitorId]);													SWAP;
    SEM_WAIT(carTicket);															SWAP;
    
    //Wake up the driver and ask for a ticket, then wait for the ticket
    SEM_WAIT(driverMutex);															SWAP;	//block driver
    SEM_SIGNAL(needTicket);															SWAP;	//signal you need ticket
    SEM_SIGNAL(driver);																SWAP;	//need driver
    SEM_WAIT(takeTicket);															SWAP;	//wait for take ticket
    SEM_SIGNAL(driverMutex);														SWAP;	//unblock driver
    
    //Update park stats
    SEM_WAIT(parkMutex);															SWAP;
    myPark.numInTicketLine--;														SWAP;	//go from ticketline
    myPark.numTicketsAvailable--;													SWAP;	//take from avail
    myPark.numInMuseumLine++;														SWAP;	//put them in line at museum
    SEM_SIGNAL(parkMutex);															SWAP;	//unblock park
    
    //end ticketbooth
    
    //******************************************************************
    //museum
    // random time waiting
    waitTime = rand() % (MAX_LINE_TIME * 10) + 1;								SWAP;
    insertDeltaClock(waitTime, deltaItemExpired[visitorId]);							SWAP;
    semWait(deltaItemExpired[visitorId]);													SWAP;
    semWait(spotInMuseum);															SWAP;
    
    //block and update park vars
    semWait(parkMutex);																SWAP;
    myPark.numInMuseumLine--;														SWAP;
    myPark.numInMuseum++;															SWAP;
    semSignal(parkMutex);															SWAP;
    
    // Browse museum for random time
    waitTime = rand() % (MAX_TIME * 10) + 1;										SWAP;
    insertDeltaClock(waitTime, deltaItemExpired[visitorId]);							SWAP;
    
    semWait(deltaItemExpired[visitorId]);													SWAP;
    
    semWait(parkMutex);																SWAP;	//block park
    myPark.numInMuseum--;															SWAP;	//goes from museum
    myPark.numInCarLine++;															SWAP;	//to line in car
    semSignal(parkMutex);															SWAP;	//unblock park
    semSignal(spotInMuseum);														SWAP;	//signal opening in museum
    //end museum
    
    //******************************************************************
    //tourCar
    // Wait random time before attempting to ride tour car
    int random = rand() % (MAX_LINE_TIME * 10) + 1;									SWAP;
    insertDeltaClock(random, deltaItemExpired[visitorId]);								SWAP;
    semWait(deltaItemExpired[visitorId]);													SWAP;
    semWait(sigPassenger);															SWAP; //get resource
    
    semWait(visitorMutex);															SWAP;	//block visitor Mutex until the end
    visitorID = visitorId;															SWAP;	//cur Ride Visitor is your visitorID passed in
    semSignal(seatTaken);															SWAP;	//signal seat taken
    semWait(passengerSeated);														SWAP;	//wait for passenger to be seated
    
    //Block and update park
    SEM_WAIT(parkMutex);															SWAP;
    myPark.numTicketsAvailable++;													SWAP;
    SEM_SIGNAL(parkMutex);															SWAP;
    
    //pass car tick release global
    SEM_SIGNAL(carTicket);															SWAP;
    semSignal(visitorMutex);														SWAP;	//unblock visitor Mutex
    
    //signal ride done
    semWait(rideDone[visitorId]);
    //end tourcar
    
    
    //******************************************************************
    //giftShop
    //GiftShop waiting time outside
    waitTime = rand() % (MAX_LINE_TIME * 10) + 1;								SWAP;
    insertDeltaClock(waitTime, deltaItemExpired[visitorId]);							SWAP;
    semWait(deltaItemExpired[visitorId]);													SWAP;
    semWait(spotInGiftShop);														SWAP;
    
    //Block and update park vars
    semWait(parkMutex);																SWAP;
    myPark.numInGiftLine--;															SWAP;
    myPark.numInGiftShop++;															SWAP;
    semSignal(parkMutex);															SWAP;
    
    // Time for in gift shop before leaving
    waitTime = rand() % (MAX_TIME * 10) + 1;										SWAP;
    insertDeltaClock(waitTime, deltaItemExpired[visitorId]);							SWAP;
    semWait(deltaItemExpired[visitorId]);													SWAP;
    
    //Block and update park vars
    semWait(parkMutex);																SWAP;
    myPark.numInGiftShop--;															SWAP;
    myPark.numInPark--;																SWAP;
    myPark.numExitedPark++;															SWAP;
    semSignal(parkMutex);															SWAP;
    
    //Signal an opening in the park and the giftshop
    semSignal(spotInGiftShop);														SWAP;
    semSignal(spotInPark);															SWAP;
    return 0;
}

//DeltaClock Testing
int P3_dcTest(int argc, char*argv[])		//Do i need to swap here?
{
    int i, flg;
    char buf[32];
    // create some test times for event[0-9]
    
    if(deltaClock->size == 0)
    {
        int ttime[10] = {
            90, 300, 50, 170, 340, 300, 50, 300, 40, 110	};
        
        //	int ttime[1] = {10};		//testing
        for (i=0; i<10; i++)
        {
            sprintf(buf, "event[%d]", i);
            //event[i] = createSemaphore(buf, BINARY, 0);
            insertDeltaClock(ttime[i],createSemaphore(buf,BINARY,0));
        }
    }
    printDeltaClock(deltaClock);
    return 0;
}
