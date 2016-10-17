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

Particle::Particle(double nMass, vec2<double> pos, double _radius){
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


double Particle::getMass() {
	return mass;
}

vec2<double> Particle::getPosition() {
	vec2<double> tmp;
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

vec2<double> Particle::getVelocity() {
	std::shared_lock<std::shared_timed_mutex> lock(vwrite);
	vec2<double> tmp = this->velocity;
	lock.unlock();
	return tmp;

}

vec2<double> Particle::getForce() {
	std::shared_lock<std::shared_timed_mutex> lock(fwrite);
	vec2<double> tmp = force;
	lock.unlock();
	return tmp;
}

void Particle::setForce(vec2<double>& nForce) {
	std::lock_guard<std::shared_timed_mutex> lock(fwrite);
	this->force = nForce;
}

void Particle::addForce(vec2<double>& aForce) {
	std::lock_guard<std::shared_timed_mutex> lock(fwrite);
	this->force += aForce;
}

//Private
void Particle::setVelocity(vec2<double>& vel) {
	std::lock_guard<std::shared_timed_mutex> lock(vwrite);
	this->velocity = vel;
}

void Particle::setForce(double x, double y) {
	std::lock_guard<std::shared_timed_mutex> lock(fwrite);
	this->force.x = x;
	this->force.y = y;
}

//Private
void Particle::addVelocity(vec2<double>& vel) {
	std::lock_guard<std::shared_timed_mutex> lock(vwrite);
	this->velocity += vel;
}

vec2<double> Particle::getSector() {
	vec2<double> tmp;
	std::shared_lock<std::shared_timed_mutex> lock(pwrite);
	tmp = sector;
	lock.unlock();
	return tmp;
}

vec2<double> Particle::getNormPosition() {
	vec2<double> tmp;
	std::shared_lock<std::shared_timed_mutex> lock(pwrite);
	tmp = sector;
	tmp += position;
	lock.unlock();
	return tmp;
}

void Particle::addForce(double x, double y) {
	std::lock_guard<std::shared_timed_mutex> lock(fwrite);
	force += vec2<double>(x, y);
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

vec2<double> Particle::calcForce(Particle& p) {
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

