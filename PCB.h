/*
 * PCB.h
 *
 *  Created on: Sep 15, 2021
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#define MAX_STACK_SIZE 65534 //64KB - 1
#define PCBList LinkedList<PCB*>

#include "list.h"
#include "thread.h"
#include "semaphor.h"



typedef enum {CREATED, READY, BLOCKED, RUNNING, FINISHED} Status; //mozda dodati i status idle


class PCB {
public:
	static ID max_id;

	PCB(Thread*, StackSize, Time);

	virtual ~PCB();
	Status my_status;
	StackSize stack_size;
	static PCB* getPCBById(int);

	static int getRunningId();

	unsigned *stack_pointer;
	unsigned int sp, ss, bp;

	Time processorTime;
	Time time_left;
	Time blocked_time;

	int getID()volatile;

	static PCBList* allPCB;

	static void wrapper();

	void start();

	void waitToComplete();
	void setBlocked(Time) volatile;
	void resetBlocked() volatile;
	void run()volatile;
	void setFinished()volatile;
	void unblockWaitList()volatile;
	Status getStatus()volatile;

	static int compareIDWrapper(PCB*);
	static int resetBlockedWrapper(PCB*);

	Thread* getThread();

	int decProcessorTime()volatile;
	int decBlockedTime()volatile;

	void resetMyTime()volatile;

	PCBList* getWaitList();

	static void mainInstance();

	friend class Kernel;
	volatile int unblocked_by_PCB;

	void setReady();

	int clone(PCB*); //treba vratiti 1 ako uspesno klonira, 0 ako ne
	int childreen_no;
	Semaphore* getSemaphore() volatile;
	void deleteSem() volatile;
	void signalParent() volatile;
private:
	Semaphore* sem;
	PCB* myParent;
	PCB();
	static volatile PCB* retPCB;
	static volatile int findId;
	int myID;
	Thread* myT;
	PCBList* waitList;
};

#endif /* PCB_H_ */
