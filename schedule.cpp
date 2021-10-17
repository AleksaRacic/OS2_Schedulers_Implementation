#include "SCHEDULE.H"
#include "PCB.h"

Scheduler::BaseScheduler* Scheduler::myScheduler = NULL;

int testTimeSlice(PCB* pcb1, PCB* pcb2){
	return pcb1->getProcessorTime() > pcb2->getProcessorTime();
}

int testPriority(PCB* pcb1, PCB* pcb2){
	return pcb1->getPriority() > pcb2->getPriority();
}

void Scheduler::setScheduler(Sched sched){
	if(myScheduler) delete myScheduler;
	myScheduler = NULL;
	if(sched == s_FCFS) myScheduler = new FCFS();
	else if(sched == s_SJF) myScheduler = new SJF();
	else if(sched == s_PRF) myScheduler = new PRF();
}

void Scheduler::put(PCB* pcb){
	if(myScheduler) myScheduler->put(pcb);
}

PCB* Scheduler::get(){
	if(myScheduler) return myScheduler->get();
	else return 0;
}


/*FCFS - First Come First Served */
Scheduler::FCFS::FCFS(){
	myList = new PCBList();
}

Scheduler::FCFS::~FCFS(){
	delete myList;
}

void Scheduler::FCFS::put(PCB* pcb){
	myList->push_back(pcb);
}

PCB* Scheduler::FCFS::get(){
	if(myList->isEmpty()) return NULL;
	else{
		PCB* tmp = myList->first();
		myList->remove(tmp);
		return tmp;
	}
}

/*SJF - Shortest Job First */
Scheduler::SJF::SJF(){
	myList = new PCBList();
}

Scheduler::SJF::~SJF(){
	delete myList;
}

void Scheduler::SJF::put(PCB* pcb){
	myList->insert_priority(testTimeSlice, pcb);
}

PCB* Scheduler::SJF::get(){
	if(myList->isEmpty()) return NULL;
	else{
		PCB* tmp = myList->first();
		myList->remove(tmp);
		return tmp;
	}
}

Scheduler::PRF::PRF(){
	myList = new PCBList();
}

Scheduler::PRF::~PRF(){
	delete myList;
}

PCB* Scheduler::PRF::get(){
	if(myList->isEmpty()) return NULL;
	else{
		PCB* tmp = myList->first();
		myList->remove(tmp);
		return tmp;
	}
}

void Scheduler::PRF::put(PCB* pcb){
	myList->insert_priority(testPriority, pcb);
}

