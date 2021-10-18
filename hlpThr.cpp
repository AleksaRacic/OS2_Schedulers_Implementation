#include "hlpThr.h"

extern int userMain(int, char**);

uMainT::uMainT(int argc_, char** argv_):Thread(256, 2, 10), argc(argc_), argv(argv_) {}

uMainT::~uMainT() {
	// TODO Auto-generated destructor stub
}

void uMainT::run() {
	ret = userMain(argc, argv);
}

Thread* uMainT::clone()const{
	return new uMainT(argc, argv);
}

idleThread::idleThread() : Thread(512,1){
	running = 1;
}

idleThread::~idleThread() {
	running = 0;
}

void idleThread::run(){
	while(running);
}

Thread* idleThread::clone() const {
	return 0;
}
