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

double ABN(Triangle t1, Triangle t2);

int main(int argc, char** argv)
{
	HODT t;

	Point p1(-5, 8);
	Point p2(2, -4);
	Point p3(5, -1);

	std::cout << "Inserting points...\n";
	Vertex_iterator v1 = t.insert(p1);
	Vertex_iterator v2 = t.insert(p2);
	Vertex_iterator v3 = t.insert(p3);
	v1->info(7);
	v2->info(-3);
	v3->info(0);
	std::cout << "Inserted.\n";
	
	std::cout << "Length results.\n";
	std::cout << "d(v1, v2) = " << t.edge_length(v1, v2) << "\n";
	std::cout << "d(v1, v3) = " << t.edge_length(v1, v3) << "\n";
	std::cout << "d(v2, v3) = " << t.edge_length(v2, v3) << "\n";

	t.output_triangulation();
	t.show_triangulation();

	return 0;
}
