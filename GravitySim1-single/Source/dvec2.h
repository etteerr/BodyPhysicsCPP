/*
 * dvec2.h
 *
 *  Created on: Sep 20, 2016
 *      Author: erwin
 */

#ifndef DVEC2_H_
#define DVEC2_H_
#include <math.h>

class dvec2 {
public:
	dvec2(double _x, double _y);
	dvec2();
	double x,y;
	double distance(dvec2&);
	double distance0();
	//void normalize();
	double length() { return distance0(); };
	dvec2 direction(dvec2&);

	//Operators
	dvec2& operator+= (const dvec2&);
	dvec2& operator*= (const dvec2&);
	dvec2& operator*= (double);
	dvec2& operator/= (const dvec2&);

	dvec2 operator-();
	dvec2 operator+(const dvec2&);
	dvec2 operator*(const dvec2&);
	dvec2 operator*(double);
	dvec2 operator/(const dvec2&);
	dvec2 operator/(double);
};

#endif /* DVEC2_H_ */
