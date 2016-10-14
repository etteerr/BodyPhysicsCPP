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

#endif /* DWARVES_H_ */
