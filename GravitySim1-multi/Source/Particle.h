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
	Particle();

	//Getters and setters, each with their own mutex lock or a global one..
	void setWeight(double);
	double getWeight();
	void setPosition(vec2<double>);
	vec2<double> getPosition();

	// calculates new velocity and steps position one velocity * dt
	void step();

private:
	double weight;
	vec2<double> position;
	vec2<double> velocity;
	vec2<double> force;
	vec2<int> sector;
	//std::mutex m_lock;
};

} /* namespace enbody */

#endif /* PARTICLE_H_ */
