#include "Vector3d.h"

Vector3d Vector3d::operator+(const Vector3d& b)
{
	const double x = _x + b._x;
	const double y = _y + b._y;
	const double z = _z + b._z;

	return Vector3d(x, y, z);
}

double Vector3d::operator*(const Vector3d& b)
{
	return _x * b._x + _y * b._y + _z * b._z;
}

Vector3d Vector3d::operator*(const double i)
{
	double x = _x * i;
	double y = _y * i;
	double z = _z * i;

	return Vector3d(x, y, z);
}

Vector3d Vector3d::operator-(const Vector3d& b)
{
	double x = _x - b._x;
	double y = _y - b._y;
	double z = _z - b._z;

	return Vector3d(x, y, z);
}

Vector3d& Vector3d::operator=(const Vector3d& b)
{
	_x = b._x;
	_y = b._y;
	_z = b._z;

	return *this;
}

bool Vector3d::operator==(const Vector3d& b)
{
	return (_x == b._x && _y == b._y && _z == b._z);
}

bool Vector3d::operator!=(const Vector3d& b)
{
	return !(*this == b);
}

void Vector3d::set_x(double x)
{
	_x = x;
}

void Vector3d::set_y(double y)
{
	_y = y;
}

void Vector3d::set_z(double z)
{
	_z = z;
}

double Vector3d::get_x()
{
	return _x;
}

double Vector3d::get_y()
{
	return _y;
}

double Vector3d::get_z()
{
	return _z;
}

void Vector3d::print()
{
	std::cout << "[" << _x << ", " << _y << ", " << _z << "]\n";
}

void Vector3d::normalize()
{
	double abs = std::sqrt( std::pow(_x, 2) + std::pow(_y, 2) + std::pow(_z, 2));

	set_x( get_x()/abs );
	set_y( get_y()/abs );
	set_z( get_z()/abs );
}

double Vector3d::squared_norm()
{
	return std::pow(_x, 2) + std::pow(_y, 2) + std::pow(_z, 2);
}

double Vector3d::norm()
{
	return std::sqrt(squared_norm());
}

Vector3d Vector3d::cross_product(Vector3d& v)
{
	return Vector3d(get_y() * v.get_z() - get_z() * v.get_y(),
			get_z() * v.get_x() - get_x() * v.get_z(),
			get_x() * v.get_y() - get_y() * v.get_x());
}
