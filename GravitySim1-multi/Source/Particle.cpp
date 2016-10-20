/*
 * Particle.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: erwin
 */

#include "Particle.h"
#include "NbodySim.h"
#include <shared_mutex>

#define G 0.0000000000667

namespace enbody {

std::shared_timed_mutex vwrite;
std::shared_timed_mutex pwrite;
std::shared_timed_mutex fwrite;

Particle::Particle(double nMass, vec2d pos, double _radius){
	mass = nMass;
	position = pos;
	radius = _radius;
}


Particle::Particle(double nMass, double x, double y, double _radius) {
	mass = nMass;
	radius = _radius;
	vec2d pos(x,y);
	position = pos;
}

Particle::Particle() {
	mass = 0;
	radius = 0;
	vec2d pos(0,0);
	position = pos;
}


double Particle::getMass() {
	return mass;
}

vec2d Particle::getPosition() {
	vec2d tmp;
	std::shared_lock<std::shared_timed_mutex> lock(pwrite);
	tmp = this->position;
	lock.unlock();
	return tmp;
}

void Particle::step() {
	// Lock all variable to prevent strange shit
	std::lock_guard<std::shared_timed_mutex> lock1(fwrite);
	std::lock_guard<std::shared_timed_mutex> lock2(pwrite);
	std::lock_guard<std::shared_timed_mutex> lock3(vwrite);

	//Do step
	{
		//update velocity vector
		velocity += (force * NbodySim::deltaT) / mass;
		//Update pos
		position += velocity * NbodySim::deltaT;
		//reset force
		force = vec2d(0,0);
		//Normalize position
		//normalizePosition();
	}
	return;
}

vec2d Particle::getVelocity() {
	std::shared_lock<std::shared_timed_mutex> lock(vwrite);
	vec2d tmp = this->velocity;
	lock.unlock();
	return tmp;

}

vec2d Particle::getForce() {
	std::shared_lock<std::shared_timed_mutex> lock(fwrite);
	vec2d tmp = force;
	lock.unlock();
	return tmp;
}

void Particle::setForce(vec2d& nForce) {
	std::lock_guard<std::shared_timed_mutex> lock(fwrite);
	this->force = nForce;
}

void Particle::addForce(vec2d& aForce) {
	std::lock_guard<std::shared_timed_mutex> lock(fwrite);
	this->force += aForce;
}

//Private
void Particle::setVelocity(vec2d& vel) {
	std::lock_guard<std::shared_timed_mutex> lock(vwrite);
	this->velocity = vel;
}

void Particle::setForce(double x, double y) {
	std::lock_guard<std::shared_timed_mutex> lock(fwrite);
	this->force.x = x;
	this->force.y = y;
}

//Private
void Particle::addVelocity(vec2d& vel) {
	std::lock_guard<std::shared_timed_mutex> lock(vwrite);
	this->velocity += vel;
}

vec2d Particle::getSector() {
	vec2d tmp;
	std::shared_lock<std::shared_timed_mutex> lock(pwrite);
	tmp = sector;
	lock.unlock();
	return tmp;
}

vec2d Particle::getNormPosition() {
	vec2d tmp;
	std::shared_lock<std::shared_timed_mutex> lock(pwrite);
	tmp = sector;
	tmp += position;
	lock.unlock();
	return tmp;
}

void Particle::addForce(double x, double y) {
	std::lock_guard<std::shared_timed_mutex> lock(fwrite);
	force += vec2d(x, y);
}


double Particle::getRadius() {
	return radius;
}

void Particle::normalizePosition() {
	if (position.x >= 1) {
		position.x--;
		sector.x++;
	} else if (position.x < 0) {
		position.x++;
		sector.x--;
	}
	if (position.y >= 1) {
		position.y--;
		sector.y++;
	} else if (position.y < 0) {
		position.y++;
		sector.y--;
	}
}

double Particle::getDistance(Particle& p) {
	return sqrt(pow(p.position.x - position.x,2)+pow(p.position.y - position.y,2));
}

vec2d Particle::calcForce(Particle& p) {
	double f = (G * mass * p.mass)/pow(getDistance(p) ,2);
	vec2d v = position.direction(p.position);
	return v*f;
}

void Particle::addForce(Particle& p) {
	vec2d v = calcForce(p);
	this->addForce(v);
	p.addForce(-v);
}

} /* namespace enbody */

