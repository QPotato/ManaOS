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

int fruta;

#define MOLIN 100000
// Molinete rima con partes posteriores.
void molinete(void* name)
{
    for(int i = 0; i < MOLIN; i++)
    {
        fruta++;
        currentThread->Yield();
    }
}


#ifdef SEMAPHORE_TEST
Semaphore s("semaforo", 3);
#endif

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

void ThreadTest()
{
    DEBUG('t', "Entering SimpleTest");
    fruta = 0;
    int threads = 8;
    srand(time(NULL));
    Thread* test[8];
    for(int i = 0; i < threads; i++)
    {
        char *threadname = new char[128];
        sprintf(threadname,"hilo %d", i + 1);
        test[i] = new Thread (threadname, true);
        test[i]->Fork (molinete, (void*)threadname);
    }
    
    for(int i = 0; i < threads; i++)
        test[i]->Join();
    
    printf("\n$$$420$$$ - fruta = %d - $$$420$$$ \n\n", fruta);
}

