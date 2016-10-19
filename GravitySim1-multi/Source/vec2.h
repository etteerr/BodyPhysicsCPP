
#ifndef __VEC2_H__
#define __VEC2_H__

#include <cmath>


template <class T>
class vec2 {
public:
	T x, y;

	vec2() :x(0), y(0) {}
	vec2(T x, T y) : x(x), y(y) {}
	vec2(const vec2& v) : x(v.x), y(v.y) {}

	vec2& operator=(const vec2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}

	vec2 operator+(const vec2& v) {
		return vec2(x + v.x, y + v.y);
	}

	double operator*(const vec2& v) {
			return (x * v.x + y * v.y);
		}

	vec2 operator-(const vec2& v) {
		return vec2(x - v.x, y - v.y);
	}

	vec2& operator+=(const vec2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	vec2& operator-=(const vec2& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}

	vec2 operator+(const double &s) {
		return vec2(x + s, y + s);
	}
	vec2 operator-(const double &s) {
		return vec2(x - s, y - s);
	}

	vec2& operator-() {
		x = -x;
		y = -y;
		return *this;
	}

	vec2 operator*(const double &s) {
		return vec2(x * s, y * s);
	}

	vec2 operator/(const double &s) {
		return vec2(x / s, y / s);
	}


	vec2& operator+=(const double & s) {
		x += s;
		y += s;
		return *this;
	}
	vec2& operator-=(const double & s) {
		x -= s;
		y -= s;
		return *this;
	}
	vec2& operator*=(const double & s) {
		x *= s;
		y *= s;
		return *this;
	}
	vec2& operator/=(const double & s) {
		x /= s;
		y /= s;
		return *this;
	}

	void set(T x, T y) {
		this->x = x;
		this->y = y;
	}

	void rotate(const double & deg) {
		double theta = deg / 180.0 * M_PI;
		double c = cos(theta);
		double s = sin(theta);
		double tx = x * c - y * s;
		double ty = x * s + y * c;
		x = tx;
		y = ty;
	}

	vec2& normalize() {
		if (length() == 0) return *this;
		*this *= (1.0 / length());
		return *this;
	}

	vec2 getnormalize() {
			if (length() == 0) return *this;

			return (*this) * (1.0 / length());
		}

	float dist(const vec2& v) const {
		vec2 d(v.x - x, v.y - y);
		return d.length();
	}
	float length() const {
		return std::sqrt(x * x + y * y);
	}
	void truncate(const double& length) {
		double angle = atan2f(y, x);
		x = length * cos(angle);
		y = length * sin(angle);
	}

	vec2 ortho() const {
		return vec2(y, -x);
	}

	static float dot(vec2 v1, vec2 v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}
	static float cross(vec2 v1, vec2 v2) {
		return (v1.x * v2.y) - (v1.y * v2.x);
	}

	vec2 direction(vec2& that) {
		vec2<double> direction;
		double r = dist(that);
		direction.x = (that.x - x)/r;
		direction.y = (that.y - y)/r;
		return direction;
	}

};

typedef vec2<float> vec2f;
typedef vec2<double> vec2d;


#endif
