/*
 * IVTEntry.h
 *
 *  Created on: Sep 20, 2021
 *      Author: OS1
 */

#ifndef OS2_PROJECT_IVTENTRY_H_
#define OS2_PROJECT_IVTENTRY_H_

#define NULL 0

class KernelEv;
typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);

class IVTEntry{
public:
	IVTEntry(IVTNo ivtNo, pInterrupt interruptRoutine);
	~IVTEntry();

	static IVTEntry* getEntry(IVTNo ivtNo);
	void setKernelEv(KernelEv* event);

	void signal();
	void callOldRoutine();
private:
	int entryNum;
	pInterrupt oldRoutine;
	KernelEv* kernelEvent;
};

#endif /* OS2_PROJECT_IVTENTRY_H_ */
