/*
 * PCB.cpp
 *
 *  Created on: Sep 15, 2021
 *      Author: OS1
 */

#include <iostream.h>
#include "kernel.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "debug.h"
#include <dos.h>


int PCB::max_id = 0;
PCBList* PCB::allPCB = new PCBList();
volatile PCB* PCB::retPCB = NULL;
volatile int PCB::findId = -1;

PCB::PCB(Thread *myThread, StackSize stack_size, Time time_slice): myT(myThread),
processorTime(time_slice), my_status(CREATED), blocked_time(0), myID(max_id++), myParent(NULL), stack_size(stack_size) {

	waitList = new PCBList();
	allPCB->push_back(this);
	time_left = time_slice;

	priority = defaultPriority;

	myParent = NULL;

	sem =  new Semaphore(0);

	#ifdef PCBCREATEDEBUG
	synchronizedPrintf("T: %d\n", myID);
	#endif


	if(stack_size > MAX_STACK_SIZE) stack_size = MAX_STACK_SIZE; //dodaj uslov i za min stack
	int sSize = stack_size / sizeof(unsigned);

	stack_pointer = new unsigned[sSize];

	if(stack_pointer != NULL){
		stack_pointer[--sSize] = 0x200; //PSW
			#ifndef BCC_BLOCK_IGNORE

			stack_pointer[--sSize] = FP_SEG(&(PCB::wrapper));
			stack_pointer[--sSize] = FP_OFF(&(PCB::wrapper));

			/*pri izlasku iz rutine se skidaju jos 9 registara*/
			// stack : PSWI, cs, ip, ax , bx , cx , dx , es ,ds , si , di , bp

			sSize -= 9;

			sp = FP_OFF(stack_pointer + sSize);
			ss = FP_SEG(stack_pointer + sSize);
			#endif
			bp = sp;
			//fork
			childreen_no = 0;
	}else{
		synchronizedPrintf("MEMFULL: stack"); //ovo doraditi i istestirati
	}
}

//dodati static konstruktor za main

PCB::PCB() : myT(NULL), processorTime(0), my_status(CREATED), blocked_time(0), myID(max_id++), myParent(NULL) {
	bp = 0;
	ss = 0;
	sp = 0;
	//allPCB->push_back(this); videti da li ovo ima smisla
	resetMyTime();
	time_left = 0;
	stack_pointer = NULL;
}

void PCB::mainInstance(){
	if(Kernel::mainPCB == NULL){
		Kernel::mainPCB = new PCB();
	}
}

PCB::~PCB() {
	waitToComplete();
	systemLock();
	deleteSem();
	allPCB->remove(this);
	delete[] stack_pointer;
	myT->myPCB = NULL;
	systemUnlock();
}

void PCB::waitToComplete(){
	systemLock();
	if(this == Kernel::running || this == Kernel::idle){
		systemUnlock();
		return;
	}

	if(my_status == CREATED || my_status == FINISHED){
		systemUnlock();
		return;
	}

	Kernel::running->setBlocked(0);
	waitList->push((PCB*)Kernel::running);
	systemUnlock();
	dispatch();
}

void PCB::setFinished() volatile {
	my_status = FINISHED;
	unblockWaitList();
	signalParent();
}

void PCB::start(){
	if(my_status == CREATED){
		setReady();
	}
}

void PCB::setReady(){
	my_status = READY;
	Scheduler::put(this);
}
void PCB::run()volatile{
	myT->run();
}

PCBList* PCB::getWaitList(){
	return waitList;
}

void PCB::unblockWaitList() volatile {
	waitList->applyAll(resetBlockedWrapper);
}


int PCB::resetBlockedWrapper(PCB* pcb){
	#ifdef UNBLOCKEDDEBUG
	synchronizedPrintf("PCB fin: ");
	#endif
	pcb->resetBlocked();
	return 0;
}

int PCB::compareIDWrapper(PCB* pcb){
	if(pcb->getID() == findId){
		retPCB = pcb;
		return 1;
	}
	return 0;
}
void PCB::wrapper(){
	Kernel::running->run();
	systemLock();
	Kernel::running->setFinished();
	systemUnlock();
	Kernel::dispatch();
}


ID PCB::getID()volatile{
	return myID;
}


PCB* PCB::getPCBById(ID _id){
	systemLock();
	findId = _id;
	retPCB = NULL;
	allPCB->applyAll(compareIDWrapper);
	PCB* tmp = (PCB*)retPCB;
	systemUnlock(); //Ako se ne zakljuca, moze neka druga nit izmeniti retPCB
	return tmp;
}

ID PCB::getRunningId(){
	return ((PCB*)Kernel::running)->getID();
}

void PCB::resetBlocked() volatile {
	#ifdef UNBLOCKEDDEBUG
	synchronizedPrintf("RB %d, by %d\n ", myID, Kernel::running->getID());
	#endif
	my_status = READY;
	Scheduler::put((PCB*)this);
}

int PCB::decProcessorTime()volatile{
	if(processorTime == 0) return 0;
	if(--time_left <= 0){
		my_status = READY; // ovo nije dobro, ja mislim
		return 1;
	}else{
		return 0;
	}
}

int PCB::decBlockedTime()volatile{
	if(blocked_time == 0) return 0;
	if(--blocked_time <= 0){
		unblocked_by_PCB = 0;
		#ifdef UNBLOCKEDDEBUG
		synchronizedPrintf("Sem t: ");
		#endif
		resetBlocked();
		return 1;
	}else{
		return 0;
	}
}

void PCB::setBlocked(Time t = 0)volatile{
	my_status = BLOCKED;
	blocked_time = t;
}


Status PCB::getStatus()volatile{
	return my_status;
}

void PCB::resetMyTime()volatile{
	time_took = 0;
	time_left = processorTime;
	my_status = RUNNING;
}

Thread* PCB::getThread(){
	return myT;
}

unsigned int parentSS, parentSP, parentBP, childSeg, childOff, childSP, childSS, childBP;
char *parent_stack_top, *child_stack_top;
unsigned sSize, index;

int PCB::clone(PCB* parentPCB){
	parentSS = _SS;
	parentSP = _SP;
	parentBP = _BP;
	parent_stack_top = (char*) MK_FP(parentSS, parentBP); //gadjamo bp, sve posle nas ne zanima
	//sSize = parentPCB->stack_size/sizeof(unsigned); //ovo mozda cuvati
	//if((int)stack_top - (int)parentPCB->stack_pointer + 12 > stack_size){
		//return -1;
	//}

	myParent = parentPCB;
	//(parentPCB->stack_size - stack_size)/2
	memcpy(stack_pointer, parentPCB->stack_pointer, stack_size); //dodati slucaj kada velicine steka nisu jednake

	child_stack_top = (char*)stack_pointer + (parent_stack_top - (char*)parentPCB->stack_pointer);

	_SS = FP_SEG(child_stack_top);

	_SP = FP_OFF(child_stack_top);


	asm{
		pop bp
		pop ax
		mov childOff, ax
		pop ax
		mov childSeg, ax
	}

	asm{
			pushf
			mov ax,childSeg
			push ax
			mov ax,childOff
			push ax
			mov ax, 0 //ret value

			push ax
			push bx
			push cx
			push dx
			push es
			push ds
			push si
			push di
			push bp
			}
	childSP = _SP;
	childBP = _SP;
	childSS = _SS;

	_SS = parentSS;
	_SP = parentSP;
	_BP = parentBP;

	ss = childSS;
	sp = childSP;
	bp = childBP;

	return 0;
}

Semaphore* PCB::getSemaphore() volatile {
	return sem;
}

void PCB::deleteSem() volatile {
	if(sem){
		delete sem;
		sem = NULL;
	}
}

void PCB::signalParent() volatile {
	if(myParent){
		myParent->sem->signal();
	}
}

Time PCB::getProcessorTime(){
	return processorTime;
}

int PCB::getPriority(){
	return priority;
}

void PCB::setPriority(int pr){
	priority = pr;
}

void PCB::incRunningTime(){
	time_took++;
}

void PCB::updatePTime() volatile {
	processorTime = (processorTime + time_took)/2;
}

