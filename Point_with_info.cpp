#include "Point_with_info.h"

bool Point_with_info::operator==(const Point_with_info& p)
{
	return (_x == p._x && _y == p._y && _info == p._info);
}

bool Point_with_info::operator!=(const Point_with_info& p)
{
	return !(*this == p);
}

void Point_with_info::set_info(double info)
{
	_info = info;
}

double Point_with_info::get_info() const
{
	return _info;
}
