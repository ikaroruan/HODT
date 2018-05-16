#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
#include <cmath>

class Vector3d
{
	public:
	
	// CONSTRUCTORS.
	Vector3d() : _x(0), _y(0), _z(0) {}
	Vector3d(const double x, const double y, const double z) : _x(x), _y(y), _z(z){}
	Vector3d(const Vector3d& p) : _x(p._x), _y(p._y), _z(p._z) {}

	// OPERATORS OVERLOAD
	Vector3d operator+(const Vector3d& b);
	Vector3d operator-(const Vector3d& b);
	double operator*(const Vector3d& b);
	Vector3d operator*(const double i);
	Vector3d& operator=(const Vector3d& b);
	bool operator==(const Vector3d& b);
	bool operator!=(const Vector3d& b);

	// SET AND GET
	void set_x(double x);
	void set_y(double y);
	void set_z(double z);
	double get_x();
	double get_y();
	double get_z();

	// FUNCTIONS.
	void print();
	void normalize();

	private:
	double _x;
	double _y;
	double _z;
};

#endif
