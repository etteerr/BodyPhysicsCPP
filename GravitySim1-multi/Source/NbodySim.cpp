/*
 * NbodySim.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: erwin
 */

#include "NbodySim.h"
#include <stdlib.h>;
namespace enbody {
enbody::NbodySim::NbodySim(int nParticles) {
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

Error enbody::NbodySim::getError() {
}

void NbodySim::resetError() {
	myError = myError();
	myError.error = noError;
}
bool enbody::NbodySim::hadError() {
	return myError.error != noError;
}

void enbody::NbodySim::printError() {
}

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
	return (unsigned int) (particleArraySize - freeSpacePointer)-1;
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

void enbody::NbodySim::setDT(double dt) {
	this->deltaT = dt;
}

double enbody::NbodySim::getDT() {
	return this->deltaT;
}

void enbody::NbodySim::setWorkingSector(vec2<int> sector) {
	this->workingSector = sector;
}

vec2<int> enbody::NbodySim::getWorkingSector() {
	return this->workingSector;
}

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

void enbody::NbodySim::addParticles(int nParticles) {

	if (getFreeSpace() < nParticles)
		increaseAlloc(nParticles - getFreeSpace());

	//Add particles

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

void enbody::NbodySim::step() {
}

void NbodySim::_setError(Error err, std::string message, int line,
		std::string file) {
}

} //Namespace

