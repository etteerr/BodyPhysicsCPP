/*
 * Particle.h
 *
 *  Created on: Sep 30, 2016
 *      Author: erwin
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "vec2.h"
#include <mutex>

namespace enbody {

//IMPORTANT NOTICE:
// ALL GETTER AND SETTER FUNCTIONS MUST USE A MUTEX LOCK
// in Effect, the whole particle can be locked or just the variable.
// For variable locking use static std::mutex in the function
// This remains locked across function calls
class Particle {
public:
	Particle(double nMass, vec2<double> pos, double _radius);
	Particle(double nMass, double x, double y, double _radius);

	//Getters and setters, each with their own mutex lock or a global one..
	double getMass();
	/**
	 * Returns relative to sector
	 */
	vec2<double> getPosition();	//Position is only set by this class (step and normalizePos..)
	vec2<double> getVelocity(); //Velocity is only set by this class (step)
	vec2<double> getForce();
	vec2<double> getSector();
	double 		 getRadius();
	double		 getDistance(Particle& p);
	vec2<double> calcForce(Particle& p);
	/**
	 * returns relative to origin (less accurate)
	 */
	vec2<double> getNormPosition(); //returns
	void setForce(vec2<double>&);
	void setForce(double x, double y);
	void addForce(vec2<double>&);
	void addForce(double x, double y);
	void addForce(Particle&);

	// calculates new velocity and steps position one velocity * dt
	void step();


private:
	//Variable
	double mass;
	double radius;
	vec2<double> position;
	vec2<double> velocity;
	vec2<double> force;
	vec2<double> sector;
	//std::mutex m_lock;

	//Private setters
	void setVelocity(vec2<double>&);
	void addVelocity(vec2<double>&);

	//private fun
	/**
	 * Updates the relative position in the sector and the sector position
	 * Warning: No mutex locks used in this function, manual locks required
	 */
	void normalizePosition(); //Devides in sectors
};

} /* namespace enbody */

#endif /* PARTICLE_H_ */
