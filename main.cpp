#include <stdio.h>
#include <string.h>
#include "kernel.h"
#include "hlpThr.h"
#include "SCHEDULE.H"

void synchronizedPrintf(const char *format, ...);

int main(int argc, char* argv[]){
	Sched sched;
	int preemtive = 0;
	if(argc < 2){
		synchronizedPrintf("Argument error\n");
	}
	if(strcmp(argv[1], "FCFS")){
		sched = s_FCFS;
	}
	else if(strcmp(argv[1], "SJF"))
	{
		sched = s_SJF;
	}
	else if(strcmp(argv[1], "PRF"))
		{
			sched = s_PRF;
		}
	if(argc > 2){
		preemtive = atoi(argv[2]);
	}
	Kernel::boot(sched, preemtive);
	uMainT* user_thread = new uMainT(argc, argv);
	user_thread->start();
	user_thread->waitToComplete();
	Kernel::restore();
	return 0;
}
