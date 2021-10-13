/*
 * thread.h
 *
 *  Created on: Sep 15, 2021
 *      Author: OS1
 */

#ifndef THREAD_H_
#define THREAD_H_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

#ifndef PCB_H_
class PCB;
#endif

class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	ID getId();

	static ID getRunningId();
	static Thread * getThreadById(ID id);
	static ID fork();
	static void exit();
	static void waitForForkChildren();

protected:
	friend class PCB;
	friend class Kernel;

	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);

	virtual void run(){}
	virtual Thread* clone() const; //ovo je bilo const

	PCB* myPCB;
private:
};

void dispatch ();

#endif /* THREAD_H_ */
