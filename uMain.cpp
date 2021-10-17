#include <iostream.h>
#include "thread.h"

extern int synchronizedPrintf(const char *format, ...);

class ShortJob : public Thread{
public:
	ShortJob() : Thread(){}
	virtual ~ShortJob(){}
	void run(){

	}
	Thread* clone() const {
		return new ShortJob();
	}
};

class LongJob : public Thread{
public:
	LongJob() : Thread(){}
	virtual ~LongJob(){}
	void run(){

	}
	Thread* clone() const {
		return new ShortJob();
	}
};

int userMain(int h, char** argc){
	int n;
	Thread *SJ, *LJ;
	while(1){
		synchronizedPrintf("1. Short Job,\n2. Long Job\n, 3. Exit\n\n");
		std::cin>>n;
		switch(n){
		case 1:
			break;
		case 2:

			break;
		case 3:
			Thread::exit();
			break;
		default:
			synchronizedPrintf("invalid input\n");
		}
	}
	return 0;
}
