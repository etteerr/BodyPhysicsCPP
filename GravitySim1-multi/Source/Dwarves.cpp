/*
 * Dwarves.cpp
 *
 *  Created on: Oct 13, 2016
 *      Author: erwin
 */

#include "Dwarves.h"

Dwarves::Dwarves() {
	dwarves = std::nullptr_t();
	noWork.data = std::nullptr_t();
	noWork.work = std::nullptr_t();
	noWork.instructions = std::nullptr_t();
	noWork.empty = true;
	dismissal = false;
}

Dwarves::~Dwarves() {
	// TODO Auto-generated destructor stub
	dismiss();
}

void Dwarves::recruit(int workers) {
	dismissal = false;
	nDwarves = 0;
	dwarves = new std::thread[workers-1];
	for(int i = 0; i < workers; i++) {
		dwarves[nDwarves] = std::thread(&Dwarves::dwarf, this);
		dwarves[nDwarves++].detach();
	}
}

void Dwarves::dismiss() {
	if (dismissal || nDwarves == 0)
		return;
	dismissal = true;
	c.notify_all();
	for(int i = 0; i < nDwarves; i++) {
		if (dwarves[i].joinable())
			dwarves[i].join();
	}
	delete[] dwarves;
	nDwarves = 0;
}

void Dwarves::emplaceWork(Work work) {
	std::lock_guard<std::mutex> lock(m);
	workQueue.push(work);
	c.notify_one();
}

void Dwarves::emplaceWork(void* data, void* work, void *instructions) {
	Work w;
	w.data = data;
	w.work = work;
	w.instructions = instructions;
	emplaceWork(w);
}

Dwarves::Work Dwarves::getWorkBlocking() {
	std::unique_lock<std::mutex> lock(m);
	while(workQueue.empty()) {
		c.wait(lock);
		if (dismissal)
			return noWork;
	}

	Work w = workQueue.front();
	workQueue.pop();

	return w;
}

bool Dwarves::hasWork() {
	return !workQueue.empty();
}

int Dwarves::getAmountOfWorkLeft() {
	return workQueue.size();
}

Dwarves::Work Dwarves::getWork() {
	std::unique_lock<std::mutex> lock(m);
	if (workQueue.empty())
		return noWork; //return Work::empty == true

	Work w = workQueue.front();
	workQueue.pop();

	return w;
}

void Dwarves::dwarf() {
	Work work;
	while(1){
		work = this->getWorkBlocking();
		if (work.empty || this->dismissal)
			return;
		void (*fun)(void*,void*) = (void (*) (void*,void*))work.work;
		fun(work.data, work.instructions);
	}
}
