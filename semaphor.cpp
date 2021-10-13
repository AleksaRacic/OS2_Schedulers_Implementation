/*
 * semaphore.cpp
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#include "semaphor.h"
#include "kernel.h"

Semaphore::Semaphore(int init) {
	systemLock();
	myImpl = new KernelSem(init);
	systemUnlock();
}

Semaphore::~Semaphore() {
	systemLock();
	delete myImpl;
	systemUnlock();
}

int Semaphore::wait(Time maxTimeToWait) {
	systemLock();
	return myImpl->wait(maxTimeToWait);
}

void Semaphore::signal() {
	systemLock();
	myImpl->signal();
	systemUnlock();
}

int Semaphore::val() const {
	return myImpl->val(); //bolje preneti preko globalnog containera??
}
