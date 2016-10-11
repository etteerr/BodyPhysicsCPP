/*
 * Particle.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: erwin
 */

#include "Particle.h"
#include "NbodySim.h"
#define G 0.0000000000667

namespace enbody {

std::mutex vwrite;
std::mutex pwrite;
std::mutex fwrite;

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
	pwrite.lock(); //Lock to prevent writing
	tmp = this->position;
	pwrite.unlock();
	return tmp;
}

void Particle::step() {
	// Lock all variable to prevent strange shit
	pwrite.lock();
	fwrite.lock();

	//Do step
	{
		//update velocity vector
		velocity += (force * NbodySim::deltaT) / mass;
		//Update pos
		position += velocity * NbodySim::deltaT;
		//reset force
		this->setForce(0, 0);
		//Normalize position
		normalizePosition();
	}

	pwrite.unlock();
	fwrite.unlock();

	return;
}

vec2<double> Particle::getVelocity() {
	vwrite.lock();
	vec2<double> tmp = this->velocity;
	vwrite.unlock();
	return tmp;

}

vec2<double> Particle::getForce() {
	fwrite.lock();
	vec2<double> tmp = this->getForce();
	fwrite.unlock();
	return tmp;
}

void Particle::setForce(vec2<double>& nForce) {
	fwrite.lock();
	this->force = nForce;
	fwrite.unlock();
}

void Particle::addForce(vec2<double>& aForce) {
	fwrite.lock();
	this->force += aForce;
	fwrite.unlock();
}

//Private
void Particle::setVelocity(vec2<double>& vel) {
	vwrite.lock();
	this->velocity = vel;
	vwrite.unlock();
}

void Particle::setForce(double x, double y) {
	fwrite.lock();
	this->force.x = x;
	this->force.y = y;
	fwrite.unlock();
}

//Private
void Particle::addVelocity(vec2<double>& vel) {
	vwrite.lock();
	this->velocity += vel;
	vwrite.unlock();
}

vec2<double> Particle::getSector() {
	vec2<double> tmp;
	pwrite.lock();
	tmp = sector;
	pwrite.unlock();
	return tmp;
}

vec2<double> Particle::getNormPosition() {
	vec2<double> tmp;
	pwrite.lock();
	tmp = sector;
	tmp += position;
	pwrite.unlock();
	return tmp;
}

void Particle::addForce(double x, double y) {
	fwrite.lock();
	force += vec2<double>(x, y);
	fwrite.unlock();
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

} /* namespace enbody */
