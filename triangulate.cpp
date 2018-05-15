#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include "HODT.h"
#include "Point_with_info.h"

typedef Point_with_info Pinfo;

int main(int argc, char** argv)
{
	HODT t;

	if(argc < 2){
		std::cout << "Not enough arguments.\n";
		return 0;
	}

	std::string filename = argv[1];
	std::ifstream input(filename);
	int aux1, aux2, aux3, size;
	double x, y;
	
	Vertex_iterator v = nullptr;
	input >> size >> aux1 >> aux2 >> aux3;
	for(int i = 0; i < size; ++i){
		input >> aux1 >> x >> y;
		Pinfo p(x, y, 0);
		v = t.insert(p);
	}

	//std::cout << "Checking order of a face:\n";
	//int order = t.face_order(v->incident_face());
	//std::cout << "order = " << order << "\n\n";

	std::cout << "Checking order of the triangulation (brute force):\n";
	int t_order = t.brute_order();
	std::cout << "Triangulation order = " << t_order << "\n\n";

	std::cout << "Printing out triangulation...\n";
	t.output_triangulation(filename);
	t.show_triangulation(filename);
}
