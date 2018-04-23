#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include "Delaunay_triangulation.h"
#include "Point_with_info.h"

typedef Point_with_info Pinfo;

int main(int argc, char** argv)
{
	Pinfo p1(1, 2, 3);
	Pinfo p2(2, 3, 4);
	Pinfo p3(1, 2, 3);

	std::cout << "== Staring Point with info tests... ==\n";
	std::cout << "-- Constructor with data: (should be 11.5, 14.4, 18.6)\n";
	Pinfo p4(11.5, 14.4, 18.6);
	std::cout << p4.get_x() << std::endl;
	std::cout << p4.get_y() << std::endl;
	std::cout << p4.get_info() << std::endl;

	std::cout << "-- Copy constructor: (should be 2, 3, 4)\n";
	Pinfo p5(p2);
	std::cout << p5.get_x() << std::endl;
	std::cout << p5.get_y() << std::endl;
	std::cout << p5.get_info() << std::endl;

	std::cout << "-- Setters and getters: (shold be 1, 2, 3)\n";
	std::cout << p1.get_x() << std::endl;
	std::cout << p1.get_y() << std::endl;
	std::cout << p1.get_info() << std::endl;
	p2.set_x(5); p2.set_y(6); p2.set_info(7);

	std::cout << "-- Now for setters: (should be 5, 6, 7)\n";	
	std::cout << p2.get_x() << std::endl;
	std::cout << p2.get_y() << std::endl;
	std::cout << p2.get_info() << std::endl;

	std::cout << "-- Assignment sign: (should be 5, 6, 7)\n";	
	Pinfo p6 = p2;
	std::cout << p6.get_x() << std::endl;
	std::cout << p6.get_y() << std::endl;
	std::cout << p6.get_info() << std::endl;

	std::cout << "-- Operators test: (should be: true false false true)\n";
	if(p1 == p3)
		std::cout << "true\n";
	else
		std::cout << "false\n";
	if(p1 != p3)
		std::cout << "true\n";
	else
		std::cout << "false\n";
	if(p1 == p2)
		std::cout << "true\n";
	else
		std::cout << "false\n";
	if(p1 != p2)
		std::cout << "true\n";
	else
		std::cout << "false\n";

}
