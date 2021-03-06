#ifndef _event_h_
#define _event_h_

#include "IVTEntry.h"

#define PREPAREENTRY(numEntry, callOld)\
\
void interrupt inter##numEntry(...); \
\
IVTEntry newEntry##numEntry(numEntry, &inter##numEntry); \
\
void interrupt inter##numEntry(...){\
	if(callOld == 1) newEntry##numEntry.callOldRoutine();\
	newEntry##numEntry.signal();\
}

class Event{
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();
protected:
	friend class KernelEv;
	void signal();
private:
	KernelEv* myImpl;
};

#endif
