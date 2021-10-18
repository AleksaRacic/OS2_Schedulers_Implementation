#include "thread.h"

class uMainT: public Thread {
public:
	uMainT(int, char**);
	virtual ~uMainT();
	volatile int ret;
	void run();
	Thread* clone()const;
private:
	int argc;
	char** argv;
};

class idleThread : public Thread {
public:
	idleThread();
	virtual ~idleThread();
	volatile int running;
	void run();
	void start();
	Thread* clone()const;
};
