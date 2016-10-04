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

bool enbody::NbodySim::initAlloc(int n) {
	//Alloc space and set (rel)pointer to 0
	try {
		particleArrayPointer = (Particle *) malloc(sizeof(Particle) * n);
		particleBufferSize = n;
		freeSpacePointer = 0;
	} catch (std::exception* e) {
		//TODO: Exception handeling
		throw *e;
		return false;
	}

	return true;
}

bool enbody::NbodySim::increaseAlloc(int n) {
	Particle *newPointer = (Particle *) realloc(particleArrayPointer,
			sizeof(Particle) * (particleBufferSize + n));
	if (newPointer != NULL) {
		particleBufferSize += n;
		particleArrayPointer = newPointer;
		return true;
	}

	//Allocation failed, old pointer is still valid.
	setError(allocationError, "Failed to allocate more memory");
	return false;

}

bool enbody::NbodySim::decreaseAlloc(int n) {
	if (freeSpacePointer >= (particleBufferSize - n)) {
		setError(deallocationError, "Memory to deallocate is in use.");
		return false;
	}

	//Realloc memory (now -n)
	Particle *newPointer = (Particle *) realloc(particleArrayPointer,
			sizeof(Particle) * (particleBufferSize - n));

	//If it worked
	if (newPointer != NULL) {
		particleBufferSize -= n;
		if (freeSpacePointer == particleBufferSize)
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
	particleBufferSize = 0;
	nParticles = 0;
}

/**
 * iterate all items and remove freespace (shift items)
 */
bool enbody::NbodySim::cleanAlloc(int n) {
	throw std::exception("Unimplemented function");
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
}

void enbody::NbodySim::addParticles(vec2<double> min, vec2<double> max,
		int nParticles) {
}

void enbody::NbodySim::addParticle(Particle particle) {
}

void enbody::NbodySim::deleteParticles(int index) {
}

void enbody::NbodySim::step() {
}

void NbodySim::_setError(Error err, std::string message, int line,
		std::string file) {
}

} //Namespace

