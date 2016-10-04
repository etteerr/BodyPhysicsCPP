/*
 * NbodySim.h
 *
 *  Created on: Sep 30, 2016
 *      Author: erwin
 */

#ifndef NBODYSIM_H_
#define NBODYSIM_H_

#include "vec2.h"
#include "Particle.h"
#include <string.h>
namespace enbody {

//Define error types
enum errorType {
	noError, allocationError, deallocationError, mathError, memoryError
};

//Define error struct
struct Error {
	errorType error;
	std::string errormsg;
	int line;
	std::string file;
};

class NbodySim {
public:

	// Simulation Parameters
	static double deltaT = 0;

	/**
	 * Initializes the simulator with an initial space of 1024 particles
	 * Allocated space increases when particles are added beyond 1024
	 *
	 */
	NbodySim();
	/**
	 * Initializes the simulator with an initial space of nParticles
	 * Allocated space increases when particles exceed the current allocated space
	 */
	NbodySim(int nParticles);
	/**
	 * Safely destroys the allocated particle space and handles other stuff
	 */
	~NbodySim();

//Error messaging
	Error getError();
	bool hadError();
	void printError();
	void resetError();

//Space management
	/**
	 * Initial space allocation for particles
	 * returns true on succes and false on fail
	 * On false, error can be found with getError();
	 */
	bool initAlloc(unsigned int n);
	/**
	 * Increases current allocated space with n
	 * on false, errors can be found with getError();
	 */
	bool increaseAlloc(unsigned int n);
	/**
	 * decreases allocation by n spaces
	 * on error, error is found with get error
	 */
	bool decreaseAlloc(unsigned int n);
	/**
	 * cleanAlloc
	 * Clean allocated space by deleting unused allocated space
	 * Leaves n empty entries
	 * (use std::memmove)
	 */
	bool cleanAlloc(unsigned int n);

	/**
	 *
	 * Free all allocated memory (del all :))
	 */
	void freeAllocatedMemory();

	/**
	 * getFreeSpace
	 * Returns amount of slots left for particles
	 */
	unsigned int getFreeSpace();

//stepsize / precision
	void setDT(double dt);
	double getDT();
	//NOTE: Simspeed is set in glutloop display fun

//Sets/gets the current working sector
	void setWorkingSector(vec2<int> sector);
	vec2<int> getWorkingSector();

//Set/gets global weight and sd for generation
	/**
	 * Sets the mean for the normal weight distribution
	 * This is than used for the following particle generation calls
	 */
	void setNormalWeight(double w, double s);
	/**
	 * Gets the mean used for random particle weights
	 */
	double getNormalWeight();
	/**
	 * Gets the sd for particle weight generation
	 */
	double getNormalWeightSD();

//Set/gets global size and sd for generation
	/**
	 * Sets the mean for the normal size distribution
	 * This is than used for the following particle generation calls
	 */
	void setNormalSize(double w, double s);
	/**
	 * Gets the mean used for random particle sizes
	 */
	double getNormalSize();
	/**
	 * Gets the sd for particle size generation
	 */
	double getNormalSizeSD();

//Add particles (random loc, size and weight)
	/**
	 * addParticles(int n)
	 * Adds n particles in current WorkingSector at random locations
	 * Uses addParticle(vec2<double> pos, Particle particle)
	 * Particles are generated with random weights based on the global mean weight +- sd
	 * Idem for size
	 */
	void addParticles(int);
	/**
	 * Adds particles in a bound region between min and max where
	 * 	0 < min < max && min < max < 1
	 * Uses addParticle(vec2<double> pos, Particle particle)
	 * Particles are generated with random weights based on the global mean weight +- sd
	 * Idem for size
	 */
	void addParticles(vec2<double> min, vec2<double> max, int);
	/**
	 * Particle particle is added at position pos and sector 'workingSector'
	 * Idem for size
	 */
	void addParticle(Particle particle);
//delete particle
	/**
	 * deleteParticles(n)
	 * deletes particle on index n
	 * use memmove to shift array to left, keep freespace right
	 */
	void deleteParticles(int n);

//Step
	/**
	 * step makes the transition from state n to state n+1
	 * with stepsize dt
	 */
	void step();

private:
	//Particles (memory and current working stuff)
	Particle* particleArrayPointer = nullptr_t;
	unsigned int nParticles = 0;
	unsigned int particleBufferSize;
	unsigned int freeSpacePointer = 0;
	vec2<int> workingSector;

	//Generation variables
	double meanWeight = 0;
	double sdWeight = 0;
	double meanSize = 0;
	double sdSize = 0;

	//Error handling
	Error myError = noError;
#define setError(E,M) _setError((Error)E,std::string(M), int(__LINE__), std::string(__FILE__))
	void _setError(Error,std::string message, int line, std::string file);
};

}

#endif /* NBODYSIM_H_ */
