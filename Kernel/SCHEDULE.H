#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_

// Obavezno odbrati HUGE mem. model kao sto je prikazano u uputstvu za podesavanje BC31 okruzenja. Takodje, podesiti putanje ka .lib i .h fajlovima kao i ostale parametre prema uputstvu!
// Dodati APLICAT.LIB u projekat: (Project->Add Item...), promeniti Name filter u *.* kako bi se videli svi fajlovi i odabrati APLICAT.LIB. Zatim pozvati Compile->Build All
class PCB;

#define NULL 0
#define PCBList LinkedList<PCB*>
#include "list.h"

typedef enum {s_FCFS, s_SJF, s_PRF} Sched;

class Scheduler{
public:
	static void put (PCB *);
	static PCB* get ();
	static void setScheduler(Sched);
	friend class PCB;

private:
	class BaseScheduler{
	public:
		virtual void put (PCB *){};
		virtual PCB* get (){};
	};

	class FCFS : public BaseScheduler{
	public:
		FCFS();
		virtual ~FCFS();
		void put(PCB *);
		PCB* get ();
		PCBList* myList;
	};

	class SJF : public BaseScheduler{
		public:
			SJF();
			virtual ~SJF();
			void put(PCB *);
			PCB* get ();
			PCBList* myList;
		};

	class PRF : public BaseScheduler{
			public:
				PRF();
				virtual ~PRF();
				void put(PCB *);
				PCB* get ();
				PCBList* myList;
			};

	static BaseScheduler* myScheduler;
};



#endif
