#include "kernel.h"
#include "hlpThr.h"

void synchronizedPrintf(const char *format, ...);

int main(int argc, char* argv[]){
	//synchronizedPrintf("\n\n\n\n");
	Kernel::boot();
	uMainT* user_thread = new uMainT(argc, argv);
	user_thread->start();
	user_thread->waitToComplete();
	Kernel::restore();
	return 0;
}
