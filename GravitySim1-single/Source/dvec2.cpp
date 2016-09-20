/*
 * dvec2.cpp
 *
 *  Created on: Sep 20, 2016
 *      Author: erwin
 */

#include "dvec2.h"

dvec2::dvec2(double _x, double _y) {
	this->x = _x;
	this->y = _y;
}
dvec2::dvec2() {
	x = 0;
	y = 0;
}

double dvec2::distance(dvec2 & v2) {
	return sqrt( pow(x-v2.x ,2) + pow(y-v2.y ,2));
}

double dvec2::distance0() {
	return sqrt(x*x+y*y);
}

dvec2& dvec2::operator+= (const dvec2& that) {
	this->x += that.x;
	this->y += that.y;
	return *this;
}


dvec2 dvec2::direction(dvec2& that) {
	dvec2 direction;
	double r = distance(that);
	direction.x = (that.x - this->x)/r; // from this to that
	direction.y = (that.y - this->y)/r;
	return direction;
}

dvec2& dvec2::operator*= (const dvec2& that) {
	this->x *= that.x;
	this->y *= that.y;
	return *this;

}

dvec2& dvec2::operator*= (double _x) {
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

dvec2 dvec2::operator*(double _x){
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

dvec2 dvec2::operator/(double that) {
	dvec2 res;
	res.x = this->x / that;
	res.y = this->y / that;
	return res;
}
