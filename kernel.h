/*
 * kernel.h
 *
 *  Created on: Sep 15, 2021
 *      Author: OS1
 */

#ifndef KERNEL_H_
#define KERNEL_H_

//#include "PCB.h"
class PCB;
class Thread;

#include <dos.h>
#include "SCHEDULE.H"

#define NULL 0

/*Hardversko maskiranje prekoida*/
#define lock() asm{pushf; cli;}
#define unlock() asm popf


/*Softversko maskiranje prekida*/
#define systemLock() {Kernel::int_locked = 1;}
/*Ako se u medjuvremenu dogodi zahtev za obradu, obraditi ga, a ne odbaciti*/
#define systemUnlock() {Kernel::int_locked = 0;\
if(Kernel::switch_context_req_timer||Kernel::switch_context_req_disp) Kernel::dispatch();}

//
void tick();


class Kernel {
public:
	static volatile int preemptive;
	static volatile PCB *running, *mainPCB, *idle;
	static Thread* idleT; //Videti zasto ne moze idleThread
	static volatile int int_locked, switch_context_req_disp, switch_context_req_timer;
	static void dispatch();
	static void boot(Sched, int);
	static void restore();

	Kernel();
	virtual ~Kernel();
private:

	static void interrupt myTimerPR(...);
	static void interrupt myTimerNP(...);
	static volatile void interrupt (*oldTimer)(...);
};
#endif /* KERNEL_H_ */
