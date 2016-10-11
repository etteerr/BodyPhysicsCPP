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
	noError, allocationError, deallocationError, mathError, memoryError, rngUninitialized
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
	static double deltaT;

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
	NbodySim(unsigned int nParticles);
	/**
	 * Safely destroys the allocated particle space and handles other stuff
	 */
	~NbodySim();

	/**
	 * Initializes random number generator
	 * creates normal distribution for mass and size
	 * Creates uniform distribution for location
	 */
	void initRNG(double s);


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
	void setWorkingSector(int x, int y);
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

	/**
	 * startSimulation
	 * 	start a threaded simulation using as many threads as cores
	 * 	initIterator creates a copy
	 */
	void startSimulation();

	/**
	 * setRealtimeFraction(x)
	 * 	sets the max simulation speed based on real time.
	 * 	If the step duration < sim-dt, delay/sleep will fill the remaining time.
	 * 	The fraction sets the speed relative to real time (eg. 2.0 means 2x faster than normal time)
	 * 	If fraction == 0, simulation is paused (results in pauseSimulation() call)
	 * 	If fraction < 0, no sleep/delay is used and simulation will run fullspeed
	 * 	setting fraction from 0 to non-zero will results in a resumeSimulation(x) call.
	 * 	example
	 * 		-dt:	0.1
	 * 		fraction:1.0
	 * 		-start: 0.0
	 * 		simulation-step
	 * 		-end:	0.01
	 * 		-delay: (dt/fraction)-(end-start)
	 * 		sleep() when delay > 0
	 *
	 */
	void setRealtimeFraction(double x);

	/**
	 * pauseSimulation()
	 * 	sets realtime fraction to 0
	 * 	Saves current realtime fraction to variable
	 * 	Note: Simulation is paused After finishing its current step
	 * 		  But before its sleep delay if fraction > 0
	 */
	void pauseSimulation();

	/**
	 * resumeSimulation()
	 * 	restores prev value of realtimeFraction
	 * resumeSimulation(x)
	 * 	resumes simulation with realtimeFraction x
	 */
	void resumeSimulation();
	void resumeSimulation(double x);

private:
	//Particles (memory and current working stuff)
	Particle* particleArrayPointer;
	unsigned int nParticles = 0;
	unsigned int particleArraySize;
	unsigned int freeSpacePointer = 0; //Points to the first free space in particleArray
	vec2<int> workingSector;

	//Generation variables
	double meanWeight = 0;
	double sdWeight = 0;
	double meanSize = 0;
	double sdSize = 0;
	bool rngIntialized = false;

	std::default_random_engine gen;
	std::normal_distribution<double> normMass;
	std::normal_distribution<double> normSize;
	std::uniform_real_distribution<double> uniLoc;

//Step
	/**
	 * step makes the transition from state n to state n+1
	 * with stepsize dt
	 */
	void step();

	//Error handling
	Error myError;
#define setError(E,M) _setError((errorType)E,std::string(M), int(__LINE__), std::string(__FILE__))
	void _setError(errorType, std::string message, int line, std::string file);
};

}

#endif /* NBODYSIM_H_ */
