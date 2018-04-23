#ifndef POINT_WITH_INFO
#define POINT_WITH_INFO

#include "Point.h"

class Point_with_info : public Point
{
	public:
	Point_with_info() : _info(0) {}
	Point_with_info(const Point_with_info& p) : Point(p._x, p._y), _info(p._info) {}
	Point_with_info(double x, double y, double info) : Point(x, y), _info(info) {}
	Point_with_info& operator=(const Point_with_info& p);
	bool operator==(const Point_with_info& p);
	bool operator!=(const Point_with_info& p);


	void set_info(double info);
	double get_info() const;

	private:
	double _info;
};

#endif
