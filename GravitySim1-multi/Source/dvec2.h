/*
 * dvec2.h
 *
 *  Created on: Sep 20, 2016
 *      Author: erwin
 */

#ifndef DVEC2_H_
#define DVEC2_H_
#include <math.h>
double sqrtq(double x) { return sqrt(x); };
double powq(double x, double y) { return pow(x,y); };
#include <quadmath.h>
#ifndef _dvec2_T
#define _dvec2_T double
#endif

class dvec2 {
public:
	dvec2(_dvec2_T _x, _dvec2_T _y);
	dvec2();
	_dvec2_T x,y;
	_dvec2_T distance(dvec2&);
	_dvec2_T distance0();
	//void normalize();
	_dvec2_T length() { return distance0(); };
	dvec2 direction(dvec2&);

	//Operators
	dvec2& operator+= (const dvec2&);
	dvec2& operator*= (const dvec2&);
	dvec2& operator*= (_dvec2_T);
	dvec2& operator/= (const dvec2&);

	dvec2 operator-();
	dvec2 operator+(const dvec2&);
	dvec2 operator*(const dvec2&);
	dvec2 operator*(_dvec2_T);
	dvec2 operator/(const dvec2&);
	dvec2 operator/(_dvec2_T);
};

dvec2::dvec2(_dvec2_T _x, _dvec2_T _y) {
	this->x = _x;
	this->y = _y;
}
dvec2::dvec2() {
	x = 0;
	y = 0;
}

_dvec2_T dvec2::distance(dvec2 & v2) {
	return sqrtq( powq(x-v2.x ,2) + powq(y-v2.y ,2));
}

_dvec2_T dvec2::distance0() {
	return sqrtq(x*x+y*y);
}

dvec2& dvec2::operator+= (const dvec2& that) {
	this->x += that.x;
	this->y += that.y;
	return *this;
}


dvec2 dvec2::direction(dvec2& that) {
	dvec2 direction;
	_dvec2_T r = distance(that);
	direction.x = (that.x - this->x)/r; // from this to that
	direction.y = (that.y - this->y)/r;
	return direction;
}

dvec2& dvec2::operator*= (const dvec2& that) {
	this->x *= that.x;
	this->y *= that.y;
	return *this;

}

dvec2& dvec2::operator*= (_dvec2_T _x) {
	this->x *= _x;
	this->y *= _x;
	return *this;

}

dvec2 dvec2::operator-() {
	dvec2 res;
	res.x = -this->x;
	res.y = -this->y;
	return res;
}


dvec2 dvec2::operator+(const dvec2& that){
	dvec2 res;
	res.x = this->x + that.x;
	res.y = this->y + that.y;
	return res;
}

dvec2 dvec2::operator*(const dvec2& that){
	dvec2 res;
	res.x = this->x * that.x;
	res.y = this->y * that.y;
	return res;
}

dvec2 dvec2::operator*(_dvec2_T _x){
	dvec2 res;
	res.x = this->x * _x;
	res.y = this->y * _x;
	return res;
}

dvec2& dvec2::operator/=(const dvec2& that) {
	this->x /= that.x;
	this->y /= that.y;
	return *this;
}

dvec2 dvec2::operator/(const dvec2& that) {
	dvec2 res;
	res.x = this->x + that.x;
	res.y = this->y + that.y;
	return res;
}

dvec2 dvec2::operator/(_dvec2_T that) {
	dvec2 res;
	res.x = this->x / that;
	res.y = this->y / that;
	return res;
}

#endif /* DVEC2_H_ */
