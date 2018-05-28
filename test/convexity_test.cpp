#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include "HODT.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangle_3<K> Triangle;
typedef K::Point_3 P3d;
typedef K::Vector_3 V3d;

int main(int argc, char** argv)
{
	HODT t;

	Point p1(0, 0);
	Point p2(1, 0);
	Point p3(1, 1);
	Point p4(2, 0);

	std::cout << "Inserting points...\n";
	Vertex_iterator v1 = t.insert(p1);
	Vertex_iterator v2 = t.insert(p2);
	Vertex_iterator v3 = t.insert(p3);
	Vertex_iterator v4 = t.insert(p4);
	std::cout << "Inserted.\n";
	
	std::cout << "Checking convexity.\n";
	Face_iterator fc = v1->incident_face();
	Face_iterator ff = v4->incident_face();
	if(t.convex_polygon(fc, ff))
		std::cout << "CONVEX\n";
	else
		std::cout << "NOT CONVEX\n";

	t.print_face(fc);
	t.print_face(ff);

	t.output_triangulation();
	t.show_triangulation();

	return 0;
}
