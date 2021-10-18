/*
 * semaphore.h
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#ifndef OS2_PROJECT_SEMAPHOR_H_
#define OS2_PROJECT_SEMAPHOR_H_

#include "KerSem.h"

typedef unsigned int Time;


class Semaphore {
	public:
		Semaphore (int init=1);
		virtual ~Semaphore ();
		virtual int wait (Time maxTimeToWait);
		virtual void signal();
		int val () const; // Returns the current value of the semaphore
	private:
		KernelSem* myImpl;
};

#endif /* OS2_PROJECT_SEMAPHOR_H_ */
