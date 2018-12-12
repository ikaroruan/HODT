#include "Point.h"

Point& Point::operator=(const Point& p)
{
	_x = p._x;
	_y = p._y;
	_info = p._info;
	return *this;
}

bool Point::operator==(const Point& p)
{
	return (_x == p._x && _y == p._y && _info == p._info);
}

bool Point::operator!=(const Point& p)
{
	return !(*this == p);
}

void Point::set_x(double x)
{
	_x = x;
}

void Point::set_y(double y)
{
	_y = y;
}

void Point::info(double in)
{
	_info = in;
}

double Point::get_x() const
{
	return _x;
}

double Point::get_y() const
{
	return _y;
}

double Point::info() const
{
	return _info;
}
