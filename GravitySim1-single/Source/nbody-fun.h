/*
 * nbody-fun.h
 *
 *  Created on: Sep 20, 2016
 *      Author: erwin
 */

#ifndef INCLUDE_NBODY_FUN_H_
#define INCLUDE_NBODY_FUN_H_

#include <math.h>
#include "dvec2.h"
#include <random>

//Define _dt
#ifndef _dt
	#define _dt 0.001
	#warning _dt not defined, using default value
#endif

//define G constant
#define G 0.0000000000667



struct particle {
	dvec2 pos;
	dvec2 vel;
	dvec2 force;
	double mass;
};



double force(particle &p1, particle &p2) {
	return (G * p1.mass * p2.mass)/pow(p1.pos.distance(p2.pos) ,2);
}



void fill_random(particle * arr, int n, double seed) {
	std::default_random_engine gen(seed);
	std::uniform_real_distribution<double> unifloc(_START_SIZE);
	std::uniform_real_distribution<double> unifmass(_MIN_WEIGHT, _MAX_WEIGHT);

	for(int i=0; i < n; i++) {
		arr[i] = particle();
		arr[i].pos.x = unifloc(gen);
		arr[i].pos.y = unifloc(gen);
		arr[i].mass = unifmass(gen);
	}
}

void step(particle * state, int n) {


	for (int i = 0; i < n-1; i ++) { //For all except the last
		for (int j = i+1; j < n; j++){ //For all the following i (prev where completed)
			double f = force(state[i], state[j]);
			dvec2 force = state[i].pos.direction(state[j].pos) * f; //Force and vector between particles i&j
			//Update both i and j
			state[i].force+= force; //Force from i to j
			state[j].force+= -force; //inverted force from i to j (j to i as result)
		}
	}
	//update pos &reset force vectors
	for (int i = 0; i < n; i ++) {
		//update velocity vector
		state[i].vel+= (state[i].force * _dt) / state[i].mass;
		//Update pos
		state[i].pos+= state[i].vel*_dt;
		//reset
		state[i].force.x = 0;
		state[i].force.y = 0;

		}
}


#endif /* INCLUDE_NBODY_FUN_H_ */
