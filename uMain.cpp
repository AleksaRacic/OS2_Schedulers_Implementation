#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include "thread.h"
#include "Logger.h"
#include "kernel.h"
#include "PCB.h"

extern int synchronizedPrintf(const char *format, ...);

Logger* time_passedr;
Logger* newPtime_passed = NULL;

unsigned int time_passed = 0;
void tick(){time_passed++;};

class ShortJob : public Thread{
public:
	ShortJob() : Thread(256, 5, 1){}
	virtual ~ShortJob(){}
	void run(){
		if(newPtime_passed){
			newPtime_passed->write("SJ: %d\n", Kernel::running->getProcessorTime());
		}
		synchronizedPrintf("Short Job started!\n\n");
		time_passedr->write("SJ: %d-", time_passed);

		int buffer=2;

		for(int i=0;i<32000;i++)
		{
			buffer = 4096/2048;
			for (int j = 0; j < 1024; j++)
			{
				buffer = buffer*2;
				if(buffer%2)
					buffer = 2;
			}
		}

		synchronizedPrintf("Short Job ended!\n\n");
		time_passedr->write("%d\n", time_passed);
	}
	Thread* clone() const {
		return new ShortJob();
	}
};

class LongJob : public Thread{
public:
	LongJob() : Thread(256, 75, 5){}
	virtual ~LongJob(){}
	void run(){
		if(newPtime_passed){
			newPtime_passed->write("LJ: %d\n", Kernel::running->getProcessorTime());
		}
		synchronizedPrintf("\nLong Job started!\n");
		time_passedr->write("LJ: %d-", time_passed);
		int buffer=2;
		for(int m = 0; m < 15; m++){
			buffer = buffer*2;
			if(buffer%2)
				buffer = 2;
			for(int i=0;i<32000;i++)
			{
				buffer = 4096/2048;

				for (int j = 0; j < 1024; j++)
				{
					buffer = buffer*2;
					if(buffer%2)
						buffer = 2;
				}
			}
		}
		synchronizedPrintf("Long Job ended!\n\n");
		time_passedr->write("%d\n", time_passed);
	}
	Thread* clone() const {
		return new ShortJob();
	}
};

int userMain(int h, char** argv){
	int n;
	Time t;
	Thread *SJ, *LJ;
	time_passedr = new Logger(argv[1]);
	if(strcmp(argv[1], "SJF") == 0){
		newPtime_passed = new Logger("Processortime_passeds");
	}
	int start = 1;
	synchronizedPrintf("1. Short Job,\n2. Long Job, \n3. Test(cocktail),\n4. Dispatch,\n5. Exit\n\n");
	while(1){
		if(start){
			if(newPtime_passed){
				t = Kernel::running->getProcessorTime();
				newPtime_passed->write("M: %d\n", t);
			}
			time_passedr->write("M: %d-", time_passed);
			start = 0;
		}
		synchronizedPrintf("Unesi opciju..\n");
		cin>>n;
		switch(n){
		case 1:
			SJ = new ShortJob();
			synchronizedPrintf("Starting Short Job..!\n");
			SJ->start();
			break;
		case 2:
			LJ = new LongJob();
			synchronizedPrintf("Starting Long Job..!\n");
			LJ->start();
			break;
		case 3:
			for(int i = 0 ; i<3; i++){
				SJ = new ShortJob();
				SJ->start();
			}
			for(int j = 0 ; j<3; j++){
				LJ = new LongJob();
				LJ->start();
			}
			for(int k = 0 ; k<3; k++){
				SJ = new ShortJob();
				SJ->start();
			}
			break;
		case 4:
			time_passedr->write("%d\n", time_passed);
			start = 1;
			dispatch();
			break;
		case 5:
			time_passedr->write("%d\n", time_passed);
			start = 1;
			Thread::exit();
			break;
		default:
			synchronizedPrintf("invalid input\n");
		}
	}
	return 0;
}
