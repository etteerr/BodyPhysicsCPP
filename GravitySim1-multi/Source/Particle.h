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
	Particle(double nMass, vec2d pos, double _radius);
	Particle(double nMass, double x, double y, double _radius);
	Particle();

	//Getters and setters, each with their own mutex lock or a global one..
	double getMass();
	/**
	 * Returns relative to sector
	 */
	vec2d getPosition();	//Position is only set by this class (step and normalizePos..)
	vec2d getVelocity(); //Velocity is only set by this class (step)
	vec2d getForce();
	vec2d getSector();
	double 		 getRadius();
	double		 getDistance(Particle& p);
	vec2d calcForce(Particle& p);
	/**
	 * returns relative to origin (less accurate)
	 */
	vec2d getNormPosition(); //returns
	void setForce(vec2d&);
	void setForce(double x, double y);
	void addForce(vec2d&);
	void addForce(double x, double y);
	void addForce(Particle&);

	// calculates new velocity and steps position one velocity * dt
	void step();


	//Variable
	double mass;
	double radius;
	vec2d position;
	vec2d velocity;
	vec2d force;
	vec2d sector;
	//std::mutex m_lock;

	//Private setters
	void setVelocity(vec2d&);
	void addVelocity(vec2d&);

	//private fun
	/**
	 * Updates the relative position in the sector and the sector position
	 * Warning: No mutex locks used in this function, manual locks required
	 */
	void normalizePosition(); //Devides in sectors

	void print(unsigned int id) {
		vec2d dir = velocity / velocity.length();
		printf("Particle %i:\n\t"
				"mass:\t\t %.5f\n\t"
				"radius:\t\t %.5f\n\t"
				"position:\t (%.5f,%.5f)\n\t"
				"direction:\t (%.5f,%.5f)\n\t"
				"speed:\t\t %.5f\n",
				id,
				mass,
				radius,
				position.x,
				position.y,
				dir.x,
				dir.y,
				velocity.length()
				);
	}
};

} /* namespace enbody */

#endif /* PARTICLE_H_ */
