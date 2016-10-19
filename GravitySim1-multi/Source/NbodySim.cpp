/*
 * NbodySim.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: erwin
 */

#include "NbodySim.h"
#include <stdlib.h>
#include <stdexcept>
#include <vector>
#include <math.h>
#include <queue>
#include <time.h>
#include <iostream>
namespace enbody {
double NbodySim::deltaT = 0;

#define G 0.0000000000667
/************************************
 *  Constructor / Deconstructor
 ************************************/

enbody::NbodySim::NbodySim(unsigned int nParticles) {
	cstep = 0;
	initAlloc(nParticles);
	resetError();
}

NbodySim::NbodySim() {
	cstep = 0;
	initAlloc(1024);
	resetError();
}

enbody::NbodySim::~NbodySim() {
	freeAllocatedMemory();
}

void enbody::NbodySim::initRNG(double s) {
	gen = std::default_random_engine(s);
	normMass = std::normal_distribution<double>(getNormalWeight(),
			getNormalWeightSD());
	normSize = std::normal_distribution<double>(getNormalSize(),
			getNormalSizeSD());
	uniLoc = std::uniform_real_distribution<double>();
	rngIntialized = true;
	//TODO: return bool for error handling
}

/************************************
 *  Memory Control
 ************************************/
bool enbody::NbodySim::initAlloc(unsigned int n) {
	buffer = NULL;
	//Alloc space and set (rel)pointer to 0
	try {
		particleArrayPointer = (Particle *) malloc(sizeof(Particle) * n);
		particleArraySize = n;
		freeSpacePointer = 0;
	} catch (std::exception* e) {
		//TODO: Exception handeling
		throw *e;
		return false;
	}

	return true;
}

bool enbody::NbodySim::increaseAlloc(unsigned int n) {
	if (particleArrayPointer <= 0)
		return false;

	Particle *newPointer = (Particle *) realloc(particleArrayPointer,
			sizeof(Particle) * (particleArraySize + n));

	if (newPointer != NULL) {
		particleArraySize += n;
		particleArrayPointer = newPointer;
		return true;
	}

	//Allocation failed, old pointer is still valid.
	setError(allocationError, "Failed to allocate more memory");
	return false;

}

bool enbody::NbodySim::decreaseAlloc(unsigned int n) {
	if (particleArrayPointer <= 0 || (particleArraySize - n) < 1)
		return false;
	if (freeSpacePointer >= (particleArraySize - n)) {
		setError(deallocationError, "Memory to deallocate is in use.");
		return false;
	}

	//Realloc memory (now -n)
	Particle *newPointer = (Particle *) realloc(particleArrayPointer,
			sizeof(Particle) * (particleArraySize - n));

	//If it worked
	if (newPointer != NULL) {
		particleArraySize -= n;
		if (freeSpacePointer == particleArraySize)
			freeSpacePointer--;
		return true;
	}

	//Alloc failed
	setError(allocationError, "de- and re-allocation failed.");
	return false;
}

void NbodySim::freeAllocatedMemory() {
	free(particleArrayPointer);
	freeSpacePointer = 0;
	particleArraySize = 0;
	nParticles = 0;
}

unsigned int NbodySim::getFreeSpace() {
	return (unsigned int) (particleArraySize - freeSpacePointer);
}

/**
 * removes excess free memory until at max n particles of free space is left
 * Does not allocate more memory!!
 */
bool enbody::NbodySim::cleanAlloc(unsigned int n) {
	if (getFreeSpace() <= n)
		return true;

	//excessSpace is freeSpace - wantedSpace
	unsigned int excessSpace = getFreeSpace() - n;

	//Dealloc space and return result
	return decreaseAlloc(excessSpace);
}

/************************************
 *  Simulation variable control
 ************************************/

void enbody::NbodySim::setDT(double dt) {
	this->deltaT = dt;
}

double enbody::NbodySim::getDT() {
	return this->deltaT;
}

void enbody::NbodySim::setWorkingSector(vec2<int> sector) {
	this->workingSector = sector;
}

void NbodySim::setWorkingSector(int x, int y) {
	this->workingSector.x = x;
	this->workingSector.y = y;
}

vec2<int> enbody::NbodySim::getWorkingSector() {
	return this->workingSector;
}

/************************************
 *  Particle generation
 ************************************/

void enbody::NbodySim::setNormalWeight(double w, double s) {
	this->meanWeight = w;
	this->sdWeight = s;
}

double enbody::NbodySim::getNormalWeight() {
	return this->meanWeight;
}

double enbody::NbodySim::getNormalWeightSD() {
	return this->sdWeight;
}

void enbody::NbodySim::setNormalSize(double w, double s) {
	this->meanSize = w;
	this->sdSize = s;
}

double enbody::NbodySim::getNormalSize() {
	return this->meanSize;
}

double enbody::NbodySim::getNormalSizeSD() {
	return this->sdSize;
}

/************************************
 *  Particle control (add/remove)
 ************************************/
void enbody::NbodySim::addParticles(unsigned int nParticles, double locscale, vec2d start) {

	if (getFreeSpace() < nParticles)
		increaseAlloc(nParticles - getFreeSpace());

	if (!rngIntialized) {
		setError(rngUninitialized, "RNG was not correctly initialized");
		throw std::runtime_error("RNG was not correctly initialized");
	}
	//Add particles

	for (unsigned int i = 0; i < nParticles; i++) {
		Particle par(normMass(gen), ((uniLoc(gen)-0.5)*2.0*locscale)+start.x, ((uniLoc(gen)-0.5)*2.0*locscale)+start.y, normSize(gen));
		addParticle(par);

	}
}

void enbody::NbodySim::addParticles(vec2<double> min, vec2<double> max,
		int nParticles) {
}

void enbody::NbodySim::addParticle(Particle particle) {
	if (getFreeSpace() == 0)
		increaseAlloc(10);

	if (getFreeSpace() > 0) {
		particleArrayPointer[freeSpacePointer++] = particle;
		nParticles++;
	}else
		setError(memoryError, "No free memory available");
}

void enbody::NbodySim::deleteParticle(int index) {
}

/**********************************
 * Simulation control
 * ********************************/

void NbodySim::startSimulation() {
	if (simrunning)
		return;

	simrunning = true;

	//Recruit dwarves
	unsigned int nDwarves = std::thread::hardware_concurrency();
	if (nDwarves==0)
		nDwarves = 3;
	myDwarves.recruit(nDwarves-1); //Might return 0!

	//start simloop thread
	simthread = std::thread(&NbodySim::simloop, this);
	simthread.detach();

}

void NbodySim::setRealtimeFraction(double x) {
	realtimeFraction = x;
}
static bool paused = false;
bool NbodySim::isPaused() {
	return paused;
}
void NbodySim::pauseSimulation() {
	if (paused)
		return;
	//pauseRealtimeFraction = realtimeFraction;
	//realtimeFraction = 0;
	paused = true; // prevend deadlock
	bufferMutex.lock(); //wait until paused and sure lock it
}

void NbodySim::resumeSimulation() {
	if (!paused)//prevent unjust unlock and var change
		return;
	paused = false;
	//realtimeFraction = pauseRealtimeFraction;
	//pauseRealtimeFraction = 0;
	bufferMutex.unlock();
}

void NbodySim::resumeSimulation(double x) {
	realtimeFraction = x;
}

void NbodySim::stopSimulation() {
	if (!simrunning)
		return;
	simrunning = false;
	simthread.join();
}

/************************************
 * Simulation Logic
 ************************************/

void enbody::NbodySim::step() {
	//Fill queue
	queueForce();
	queueStep();

	//Work on queue as well (dont let the dwarves to all the work!!)
	Dwarves::Work work;
	while (!(work = myDwarves.getWork()).empty) {
		void (*fun)(void*, void*) = (void (*) (void*,void*))work.work;
		fun(work.data, work.instructions);
	}

	//We have done all the work
	cstep++;
}

void forceDwarf(void * particles, void * instr) {
	unsigned int myParticle = ((instructions*) instr)->from;
	unsigned int nParticles = ((instructions*) instr)->nPart;
	for (unsigned int i = myParticle + 1; i < nParticles; i++)
		((Particle*) particles)[myParticle].addForce(
				((Particle*) particles)[i]);
	delete ((instructions*) instr);
}

void stepDwarf(void * particles, void * instr) {
	((Particle*) particles)[((instructions*) instr)->from].step();
	delete ((instructions*) instr);
}

void resolveCollision(Particle* ball, Particle *ball2)
{
    // get the mtd
    vec2d delta = ball->position-ball2->position;
    double d = ball->position.dist(ball2->position);
    // minimum translation distance to push balls apart after intersecting
    vec2d mtd = delta * (((ball->getRadius() + ball2->getRadius())-d)/d);


    // resolve intersection --
    // inverse mass quantities
    double im1 = 1 / ball->getMass();
    double im2 = 1 / ball2->getMass();

    // push-pull them apart based off their mass
    ball->position += mtd*(im1 / (im1 + im2));
    ball2->position -= mtd*(im2 / (im1 + im2));

    // impact speed
    vec2d v = (ball->velocity - (ball2->velocity));
    double vn = v * (mtd.getnormalize());

    // sphere intersecting but moving away from each other already
    if (vn > 0.0f) return;

    // collision impulse
    double i = (-(1.0f + 0.85) * vn) / (im1 + im2);
    vec2d impulse = mtd * i;

    // change in momentum
    ball->velocity += (impulse*im1);
    ball2->velocity -= (impulse*im2);
}

void collisionDwarf(void * particles, void * instr) {
	unsigned int myParticle = ((instructions*) instr)->from;
	unsigned int nParticles = ((instructions*) instr)->nPart;
	Particle * p = (Particle *)particles;
	double r;
	r = p[myParticle].getRadius();
	for (unsigned int i = 0; i < nParticles; i++){
		if (!(i==myParticle)) {
			if (p[myParticle].getDistance(p[i])<r+p[i].getRadius()) {
				//collision
				/* retarded
				double insideDist = (r+p[i].getRadius()) - p[myParticle].getDistance(p[i]);
				vec2d dir = p[myParticle].position.direction(p[i].position);
				p[myParticle].position += (dir*insideDist)/2;
				p[i].position -= (dir*insideDist)/2;
				*/
				//p[myParticle].velocity = -p[myParticle].velocity;
				//p[i].velocity = -p[i].velocity;
				resolveCollision(&p[myParticle], &p[i]);
			}
		}
	}
	delete ((instructions*) instr);
}

void NbodySim::queueForce() {

	instructions* a;
	Dwarves::Work w;

	for (unsigned int i = 0; i < nParticles; i++) {
		a = new instructions;
		a->nPart = nParticles;
		a->from = i;
		w.data = particleArrayPointer;
		w.work = (void*) forceDwarf;
		w.instructions = a;
		myDwarves.emplaceWork(w);
	}

}

void NbodySim::queueCollision() {

	instructions* a;
	Dwarves::Work w;

	for (unsigned int i = 0; i < nParticles; i++) {
		a = new instructions;
		a->nPart = nParticles;
		a->from = i;
		w.data = particleArrayPointer;
		w.work = (void*) collisionDwarf;
		w.instructions = a;
		myDwarves.emplaceWork(w);
	}

}

void NbodySim::queueStep() {
	instructions* a;
	Dwarves::Work w;

	for (unsigned int i = 0; i < nParticles; i++) {
		a = new instructions;
		a->from = i;
		w.data = particleArrayPointer;
		w.work = (void*) stepDwarf;
		w.instructions = a;
		myDwarves.emplaceWork(w);
	}
}

void NbodySim::helpUpdate() {
	Dwarves::Work work;
	while (myDwarves.hasWork()) {
		work = myDwarves.getWork();
		if (work.empty)
			return;
		void (*fun)(void*, void*) = (void (*) (void*,void*))work.work;
		fun(work.data, work.instructions);
	}
}

void NbodySim::simloop() {
	using namespace std::chrono_literals;

	while (simrunning) {
		//Lock buffer updates
		bufferMutex.lock();

		//start timer
		auto start = std::chrono::high_resolution_clock::now();

		//queue operations
		queueForce();
		queueCollision();
		queueStep();


		//Help work
		helpUpdate();

		//update loggers
		this->processLoggers();

		cstep++;
		//done with step
		bufferMutex.unlock();

		//step time & wait
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds dt = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
		//wait
		while (realtimeFraction == 0) {
			std::this_thread::yield();
		}
		if (this->realtimeFraction >= 0) {
			long int sleept = (long int)((deltaT/realtimeFraction)*1000000000)-(long int)dt.count();
			std::chrono::duration<int, std::nano> sleeptime = std::chrono::duration<int, std::nano>(sleept);
			//std::cout << "sleept: " << dt.count() << std::endl;
			//std::cout << "sleep: " << sleeptime.count() << std::endl;
			if (sleeptime.count() > 0) {
				std::this_thread::__sleep_for(std::chrono::duration<int>(0),
						sleeptime);
			}
			stimstepmutex.lock();
			simsteptime = (double)dt.count()/1000000.0;
			stimstepmutex.unlock();
		}
	}

}

Particle* NbodySim::enableReadBuffer() {
	if (buffer != NULL)
		return buffer;

	buffer = (Particle*) malloc(nParticles * sizeof(Particle));
	memcpy(buffer, particleArrayPointer, nParticles * sizeof(Particle));
	buffCstep = cstep;
	buffersize = nParticles;
	return buffer;
}

bool NbodySim::updateBuffer() {
	if (buffer == NULL)
		return false;

	if (buffCstep == cstep)
		return true;

	//adjust size
	if (!(nParticles == buffersize)) {
		Particle* newb = (Particle*) realloc(buffer, nParticles * sizeof(Particle));
		if (newb == NULL)
			return false;
		buffersize = nParticles;
		buffer = newb;
	}
	//update buffer
	if (bufferMutex.try_lock_for(std::chrono::milliseconds(64))){
		memcpy(buffer, particleArrayPointer, nParticles * sizeof(Particle));
		buffCstep = cstep;
		bufferMutex.unlock();
		std::cout << "update" << std::endl;
	}
	return true;
}

void NbodySim::disableReadBuffer() {
	free(buffer);
	buffer = NULL;
}

/************************************
 * Error control
 ************************************/
void NbodySim::_setError(errorType err, std::string message, int line,
		std::string file, bool print) {
	myError.error = err;
	myError.errormsg = message;
	myError.line = line;
	myError.file = file;
	if (print)
		printError();
}

Error enbody::NbodySim::getError() {
	return myError;
}

void NbodySim::resetError() {
	myError.errormsg = "";
	myError.file = "";
	myError.line = -1;
	myError.error = noError;
}
bool enbody::NbodySim::hadError() {
	return myError.error != noError;
}

void enbody::NbodySim::printError() {
	std::printf("Error occurred: %s\nline: %i\nFile: %s\n",
			myError.errormsg.c_str(), myError.line, myError.file.c_str());
}

} //Namespace

