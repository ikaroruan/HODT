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
	
	input >> size >> aux1 >> aux2 >> aux3;
	for(int i = 0; i < size; ++i){
		input >> aux1 >> x >> y;
		Pinfo p(x, y, 0);
		t.insert(p);
	}

	std::cout << "Printing out triangulation...\n";
	t.output_triangulation(filename);
	t.show_triangulation(filename);
}
