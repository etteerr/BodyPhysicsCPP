/*
 * Dwarves.h
 *
 *  Created on: Oct 13, 2016
 *      Author: erwin
 */
#ifndef DWARVES_H_
#define DWARVES_H_

#include <cstddef>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

class Dwarves {
public:
	//predef
	struct Work;
	//constructor / deconstructor
	Dwarves();
	virtual ~Dwarves();

	/**
	 * recruits dwarves to work the queue, without dwarves to work, you must pull from the queue
	 * manually and work (dwarves are threads)
	 */
	void recruit(int workers);

	/**
	 * dismisses all dwarves
	 */
	void dismiss();

	/**
	 * add work to the dwarves already overflowing schedule.
	 * data points to the data to work on, this is written and read. be sure it thread safe!
	 * work is the function that handles data
	 * (optional) instructions is given as a argument to the work function
	 * pass NULL if you dont want instructions
	 * Work function:
	 * 	void work(void * data, void * instructions)
	 * tip: Create a struct for instructions :)
	 */
	void emplaceWork(void * data, void * work, void * instructions);
	void emplaceWork(Work);
	//void emplaceWorkFront(Work work);
	//void emplaceWorkFront(void * data, void * work, void * instructions);
	/**
	 * retrieves the next to do thing
	 */
	Work getWork();

	/**
	 * hasWork()
	 */
	bool hasWork();

	/**
	 * getAmountOfWorkLeft()
	 */
	int getAmountOfWorkLeft();

	//Datatypes
	struct Work {
		void * data;
		void * work;
		void * instructions;
		bool empty = false;
	};

	/**
	 * worker aka dwarf
	 */
	void dwarf();

private:
	std::queue<Work> workQueue;
	int nDwarves = 0;
	std::thread * dwarves;
	bool dismissal;

	//thread save thing
	mutable std::mutex m;
	std::condition_variable c;

	Work noWork;

};
/*
void workfun(void *data, void * instuctions) {
	int i = *(int*)data;
	std::cout << "Worker works: " << i << std::endl;
	delete (int*)data;
}

void workfun2(void *data, void * instuctions) {
	int i = *(int*)data;
	std::cout << "me works: " << i << std::endl;
	delete (int*)data;
}
void testQueue() {
	Dwarves myDwarves;
	myDwarves.recruit(1);
	for (int i = 0; i < 200000; i++)
		myDwarves.emplaceWork((void*)new int(i),(void*)workfun, NULL);

	Dwarves::Work w = myDwarves.getWork();
	workfun2(w.data, w.instructions);

	w = myDwarves.getWork();
	workfun2(w.data, w.instructions);


	int i = 0;
	while(myDwarves.hasWork()) {
		std::cout << myDwarves.getAmountOfWorkLeft() << std::endl;
		w = myDwarves.getWork();
		if (!w.empty)
			workfun2(w.data, w.instructions);
		i++;
	}
	std::cout << "Work done: " << i << std::endl;

	myDwarves.dismiss();

}*/
#endif /* DWARVES_H_ */
