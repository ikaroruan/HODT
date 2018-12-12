#ifndef POINT_H
#define POINT_H

class Point
{
	public:
	
	Point() : _x(0), _y(0), _info(0) {}
	Point(const Point& p) : _x(p._x), _y(p._y), _info(p._info) {}
	Point(const double x, const double y) : _x(x), _y(y), _info(0){}
	Point& operator=(const Point& p);
	bool operator==(const Point& p);
	bool operator!=(const Point& p);

	void set_x(double x);
	void set_y(double y);
	void info(double in);
	double get_x() const;
	double get_y() const;
	double info() const;

	protected:
	double _x;
	double _y;
	double _info;
};

#endif
