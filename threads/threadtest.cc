// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create several threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustrate the inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.
//
// Parts from Copyright (c) 2007-2009 Universidad de Las Palmas de Gran Canaria
//

#include "copyright.h"
#include "system.h"
#include "synch.h"
#include <time.h>
//----------------------------------------------------------------------
// SimpleThread
// 	Loop 10 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"name" points to a string with a thread name, just for
//      debugging purposes.
//----------------------------------------------------------------------

#ifdef SEMAPHORE_TEST
Semaphore s("semaforo", 3);
#endif

Puerto *p;

void emisor(void *name)
{
    char *nombre = (char*)name;
    p->Send((int)(nombre[7]));
    printf("%s mandó %d\n", nombre, (int)(nombre[7]));
}

void receptor(void *name)
{
    int msg;
    for(int i = 0; i < 3; i++)
    {
        p->Receive(&msg);
        printf("%s recibió %d\n", (char*)name, msg);
    }
}

void
SimpleThread(void* name)
{
    // Reinterpret arg "name" as a string
    char* threadName = (char*)name;
    
    // If the lines dealing with interrupts are commented,
    // the code will behave incorrectly, because
    // printf execution may cause race conditions.
#ifdef SEMAPHORE_TEST
    s.P();
    DEBUG('s', "hilo %s hizo P\n", threadName);
#endif
    for (int num = 0; num < 10; num++) {
        //IntStatus oldLevel = interrupt->SetLevel(IntOff);
	    printf("*** thread %s looped %d times\n", threadName, num);
	    //interrupt->SetLevel(oldLevel);
        currentThread->Yield();
    }
    //IntStatus oldLevel = interrupt->SetLevel(IntOff);
    printf(">>> Thread %s has finished\n", threadName);
#ifdef SEMAPHORE_TEST
    s.V();
    DEBUG('s', "hilo %d hizo V\n", threadName);
#endif
    //interrupt->SetLevel(oldLevel);
}

//----------------------------------------------------------------------
// ThreadTest
// 	Set up a ping-pong between several threads, by launching
//	ten threads which call SimpleThread, and finally calling 
//	SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest()
{
    DEBUG('t', "Entering SimpleTest");
    
    int emisores = 6;
    int receptores = 2;
    srand(time(NULL));
    p = new Puerto();
    for(int i = 0; i < emisores; i++)
    {
        char *threadname = new char[128];
        sprintf(threadname,"emisor %d", i + 1);
        Thread* newThread = new Thread (threadname);
        newThread->Fork (emisor, (void*)threadname);
    }
    
    for(int i = 0; i < receptores; i++)
    {
        char *threadname = new char[128];
        sprintf(threadname,"receptor %d", i + 1);
        Thread* newThread = new Thread (threadname);
        newThread->Fork (receptor, (void*)threadname);
    }
    
    for(int i = 0; i < 1000; i++)
        currentThread->Yield();

    delete p;
}

