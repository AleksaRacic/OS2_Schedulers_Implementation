/*
 * KernelEv.h
 *
 *  Created on: Sep 20, 2021
 *      Author: OS1
 */

#ifndef OS2_PROJECT_KERNELEV_H_
#define OS2_PROJECT_KERNELEV_H_

class PCB;
typedef unsigned char IVTNo;

class KernelEv{
public:
	KernelEv(IVTNo);
	~KernelEv();

	void wait();
	void signal();
private:
	int entryNum; // number of entry linked with this event
	PCB* myPCB; // thread that created object, only this thread can be blocked
};

#endif /* OS2_PROJECT_KERNELEV_H_ */
