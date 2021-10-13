/*
 * kernel.cpp
 *
 *  Created on: Sep 15, 2021
 *      Author: OS1
 */

#include "kernel.h"
#include "SCHEDULE.H"
#include "hlpThr.h"
#include "KerSem.h"
#include "PCB.h"

#include "debug.h"

volatile void interrupt (*Kernel::oldTimer)(...) = NULL;

volatile int Kernel::switch_context_req_timer = 0;
volatile int Kernel::switch_context_req_disp = 0;
volatile PCB* Kernel::idle = NULL;
volatile PCB* Kernel::running = NULL;
volatile PCB* Kernel::mainPCB = NULL;
volatile int Kernel::int_locked = 0;

unsigned int tsp, tss, tbp;

Thread* Kernel::idleT = NULL;

void Kernel::dispatch(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	switch_context_req_disp = 1;
	if(!int_locked){//da se ne bi zvao tajmer ako se pozove ispatch tokom locka
		myTimer();
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

}

void interrupt Kernel::myTimer(...){
	int time_left;
	if(!switch_context_req_disp || (int_locked && switch_context_req_disp)){
		tick();
		(*oldTimer)();
		switch_context_req_timer = running->decProcessorTime();
		KernelSem::decAllSem();
	}

	if(!int_locked && (switch_context_req_timer || switch_context_req_disp)){
		switch_context_req_disp = 0;
		switch_context_req_timer = 0;
		PCB *next_thread = NULL;
		#ifdef KERNELDEBUG
		synchronizedPrintf("_%d->",running->getID());
		#endif
		if((running->getStatus() == READY || running->getStatus() == RUNNING) && running != idle)
			Scheduler::put((PCB*)running);

		if((next_thread = Scheduler::get())== NULL)
			next_thread = (PCB*) idle;

		if(running->getStatus() == FINISHED){
			delete running;
		}else{
		#ifndef BCC_BLOCK_IGNORE
		asm{
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
			}
		#endif

		running->sp = tsp;
		running->ss = tss;
		running->bp = tbp;
		}


		running = next_thread;
		running->resetMyTime();

		tsp = running->sp;
		tss = running->ss;
		tbp = running->bp;

		#ifdef KERNELDEBUG
		synchronizedPrintf("%d\n",running->getID());
		#endif

		#ifndef BCC_BLOCK_IGNORE
		asm{
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
		#endif
	}
}

void Kernel::boot(){
	#ifndef BCC_BLOCK_IGNORE
	lock();
	oldTimer = getvect(0x08);
	setvect(0x08, myTimer);
	#endif

	PCB::mainInstance();
	running = mainPCB;

	idleT = new idleThread();
	idle = idleT->myPCB;

	//dodati semafore i ostalo

	#ifndef BCC_BLOCK_IGNORE
	unlock();
	#endif
}

void Kernel::restore(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
		setvect(0x08, oldTimer);
	#endif
		delete idleT;
		delete mainPCB;
	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}


