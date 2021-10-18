#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kernel.h"
#include "hlpThr.h"
#include "SCHEDULE.H"

void synchronizedPrintf(const char *format, ...);

int main(int argc, char* argv[]){
	Sched sched;
	int preemtive = 0;
	if(argc < 2){
		synchronizedPrintf("Argument error!\n");
		return -1;
	}

	if(strcmp(argv[1], "FCFS") == 0){
		sched = s_FCFS;
	}
	else if(strcmp(argv[1], "SJF") == 0)
	{
		sched = s_SJF;
	}
	else if(strcmp(argv[1], "PRF") == 0)
		{
			sched = s_PRF;
		}
	else{
		synchronizedPrintf("Scheduler error!\n");
		return -1;
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
