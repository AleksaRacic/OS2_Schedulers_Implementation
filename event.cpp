#include "event.h"
#include "kernel.h"
#include "KernelEv.h"
#include <stdlib.h>

Event::Event (IVTNo ivtNo){
	systemLock();
	if(IVTEntry::getEntry(ivtNo) == NULL) myImpl = new KernelEv(ivtNo);
	else myImpl = NULL;
	systemUnlock();
}

Event::~Event (){
	//videti da li ovde treba lock
	delete myImpl;
}

void Event::wait (){
	systemLock();
	myImpl->wait();
}

void Event::signal(){
	systemLock();
	myImpl->signal();
	systemUnlock();
}
