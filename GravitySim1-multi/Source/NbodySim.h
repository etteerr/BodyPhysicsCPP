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
	noError,
	allocationError,
	mathError
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

	/**
	 * Initializes the simulator with an initial space of 1024 particles
	 * Allocated space increases when particles are added beyond 1024
	 *
	 */
	NbodySim() { NbodySim(1024); }
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

//Space management
	/**
	 * Initial space allocation for particles
	 * returns true on succes and false on fail
	 * On false, error can be found with getError();
	 */
	bool initAlloc(int n);
	/**
	 * Increases current allocated space with n
	 * on false, errors can be found with getError();
	 */
	bool increaseAlloc(int n);
	/**
	 * decreases allocation by n spaces
	 * on error, error is found with get error
	 */
	bool decreaseAlloc(int n);
	/**
	 * cleanAlloc
	 * Clean allocated space by deleting unused allocated space
	 * Leaves n empty entries
	 * (use std::memmove)
	 */
	bool cleanAlloc(int n);

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
	void setNormalWeight(double w);
	/**
	 * Gets the mean used for random particle weights
	 */
	double getNormalWeight();
	/**
	 * Sets the sd for particle weight generation
	 */
	void setNormalWeightSD(double w);
	/**
	 * Gets the sd for particle weight generation
	 */
	double getNormalWeightSD();

//Set/gets global size and sd for generation
	/**
	 * Sets the mean for the normal size distribution
	 * This is than used for the following particle generation calls
	 */
	void setNormalSize(double w);
	/**
	 * Gets the mean used for random particle sizes
	 */
	double getNormalSize();
	/**
	 * Sets the sd for particle size generation
	 */
	void setNormalSizeSD(double w);
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
	void addParticle(vec2<double> pos, Particle particle);
//delete particle
	/**
	 * deleteParticle(n)
	 * deletes particle on index n
	 * use memmove to shift array to left, keep freespace right
	 */
	void deleteParticle(int n);

//Step
	/**
	 * step makes the transition from state n to state n+1
	 * with stepsize dt
	 */
	void step();


private:
	//Particles
	Particle* particleArrayPointer = nullptr_t;
	int nParticles = 0;
	vec2<int> workingSector;

	//Generation variables
	double meanWeight 	= 0;
	double sdWeight 	= 0;
	double meanSize 	= 0;
	double sdSize 		= 0;


	//Error handling
	Error myError = noError;
#define setError(E) _setError(E, __LINE__, __FILE__)
	void _setError(Error, int line, std::string file);
};

}

#endif /* NBODYSIM_H_ */
