#ifndef THREAD_H_
#define THREAD_H_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
const int defaultPriority = 1;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

class PCB;


class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	ID getId();
	int getPriority();

	static ID getRunningId();
	static Thread * getThreadById(ID id);
	static ID fork();
	static void exit();
	static void waitForForkChildren();

protected:
	friend class PCB;
	friend class Kernel;

	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice, int priority = defaultPriority);

	virtual void run(){}
	virtual Thread* clone() const; //ovo je bilo const

	PCB* myPCB;
private:
};

void dispatch ();

#endif /* THREAD_H_ */
