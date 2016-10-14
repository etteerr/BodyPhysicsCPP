/*
 * NbodySim.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: erwin
 */

#include "NbodySim.h"
#include <stdlib.h>
#include <stdexcept>
namespace enbody {
double NbodySim::deltaT = 0;



/************************************
 *  Constructor / Deconstructor
 ************************************/

enbody::NbodySim::NbodySim(unsigned int nParticles) {
	initAlloc(nParticles);
	resetError();
}

NbodySim::NbodySim() {
	initAlloc(1024);
	resetError();
}

enbody::NbodySim::~NbodySim() {
	freeAllocatedMemory();
}

void enbody::NbodySim::initRNG(double s){
	gen = std::default_random_engine (s);
	normMass = std::normal_distribution<double>(getNormalWeight(), getNormalWeightSD());
	normSize = std::normal_distribution<double>(getNormalSize(), getNormalSizeSD());
	uniLoc = std::uniform_real_distribution<double>();
	rngIntialized = true;
	//TODO: return bool for error handling
}

/************************************
 *  Memory Control
 ************************************/
bool enbody::NbodySim::initAlloc(unsigned int n) {
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
	if(particleArrayPointer <= 0)
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
	if(particleArrayPointer <= 0 || (particleArraySize - n) < 1)
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
void enbody::NbodySim::addParticles(int nParticles) {

	if (getFreeSpace() < nParticles)
		increaseAlloc(nParticles - getFreeSpace());

	if (!rngIntialized) {
		setError(rngUninitialized, "RNG was not correctly initialized");
		throw std::runtime_error("RNG was not correctly initialized");
	}
	//Add particles


	for (int i = 0; i < nParticles; i++) {
		Particle par(normMass(gen), uniLoc(gen), uniLoc(gen), normSize(gen));
		addParticle(par);
	}
}

void enbody::NbodySim::addParticles(vec2<double> min, vec2<double> max,
		int nParticles) {
}

void enbody::NbodySim::addParticle(Particle particle) {
	if (getFreeSpace() == 0)
		increaseAlloc(10);

	if (getFreeSpace() > 0)
		particleArrayPointer[freeSpacePointer++] = particle;
	else
		setError(memoryError, "No free memory available");
}

void enbody::NbodySim::deleteParticles(int index) {
}

/**********************************
 * Simulation control
 * ********************************/
void NbodySim::startSimulation() {
}

void NbodySim::setRealtimeFraction(double x) {
}

void NbodySim::pauseSimulation() {
}

void NbodySim::resumeSimulation() {
}

void NbodySim::resumeSimulation(double x) {
}

/************************************
 * Simulation Logic
 ************************************/

void enbody::NbodySim::step() {
}

/************************************
 * Error control
 ************************************/
void NbodySim::_setError(errorType err, std::string message, int line,
		std::string file) {
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
}

} //Namespace

