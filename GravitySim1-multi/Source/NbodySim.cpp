/*
 * NbodySim.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: erwin
 */

#include "NbodySim.h"
namespace enbody {
enbody::NbodySim::NbodySim(int nParticles) {
}

enbody::NbodySim::~NbodySim() {
}

Error enbody::NbodySim::getError() {
}

bool enbody::NbodySim::hadError() {
}

void enbody::NbodySim::printError() {
}

bool enbody::NbodySim::initAlloc(int n) {
}

bool enbody::NbodySim::increaseAlloc(int n) {
}

bool enbody::NbodySim::decreaseAlloc(int n) {
}

bool enbody::NbodySim::cleanAlloc(int n) {
}

void enbody::NbodySim::setDT(double dt) {
}

double enbody::NbodySim::getDT() {
}

void enbody::NbodySim::setWorkingSector(vec2<int> sector) {
}

vec2<int> enbody::NbodySim::getWorkingSector() {
}

void enbody::NbodySim::setNormalWeight(double w) {
}

double enbody::NbodySim::getNormalWeight() {
}

void enbody::NbodySim::setNormalWeightSD(double w) {
	if (w>=0)
		this->sdWeight = w;
	else
		this->sdWeight = -w;
}

double enbody::NbodySim::getNormalWeightSD() {
}

void enbody::NbodySim::setNormalSize(double w) {
}

double enbody::NbodySim::getNormalSize() {
}

void enbody::NbodySim::setNormalSizeSD(double w) {
	if (w>=0)
		this->sdSize = w;
	else
		this->sdSize = -w;
}

double enbody::NbodySim::getNormalSizeSD() {
	return this->sdSize;
}

void enbody::NbodySim::addParticles(int nParticles) {
}

void enbody::NbodySim::addParticles(vec2<double> min, vec2<double> max,
		int nParticles) {
}

void enbody::NbodySim::addParticle(vec2<double> pos, Particle particle) {
}

void enbody::NbodySim::deleteParticle(int index) {
}

void enbody::NbodySim::step() {
}

void enbody::NbodySim::_setError(Error error1, int line, std::string file) {
}
}
