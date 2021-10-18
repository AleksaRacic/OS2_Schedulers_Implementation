/*
 * KernelSem.h
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#ifndef OS2_PROJECT_KERSEM_H_
#define OS2_PROJECT_KERSEM_H_

class PCB;
#define SemList LinkedList<KernelSem*>
#define PCBList LinkedList<PCB*>

#include "list.h"

typedef unsigned int Time; // time, x 55ms


class KernelSem {
public:
	KernelSem(int init = 1);
	virtual ~KernelSem();
	int wait (Time maxTimeToWait);
	void signal();
	int val () const;
	void decTime();
	static void decAllSem();
private:
	friend class Kernel;
	static SemList* allSem;
	PCBList* blockedPCB;
	int value;
	int block(Time);
	void unblock();
	static int decTimeWrapper(PCB*);
	static int decAllWrapper(KernelSem*);
};

#endif /* OS2_PROJECT_KERSEM_H_ */
