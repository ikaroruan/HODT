#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <chrono>

#include "HODT.h"

void output_results(std::ostream& out, int max_order, double abn1, double abn2,
		    double avg_aratio1, double avg_aratio2, int order1, int order2,
		    double elapsed1, double elapsed2){
	out << std::setw(8)  << max_order
	    << std::setw(12) << abn1
	    << std::setw(12) << abn2
	    << std::setw(13) << avg_aratio1
	    << std::setw(13) << avg_aratio2
	    << std::setw(8)  << order1
	    << std::setw(8)  << order2
	    << std::setw(12) << elapsed1
	    << std::setw(12) << elapsed2
	    << std::endl;
}

void output_header(std::ostream& out){
	out << std::setw(8)  << "K-ORDER"
	    << std::setw(12) << "ABN1"
	    << std::setw(12) << "ABN2"
	    << std::setw(13) << "AVG.ASPR1"
	    << std::setw(13) << "AVG.ASPR2"
	    << std::setw(8)  << "ORDER1"
	    << std::setw(8)  << "ORDER2"
	    << std::setw(12) << "E.TIME1"
	    << std::setw(12) << "E.TIME2"
	    << std::endl;
}

int main(int argc, char** argv)
{
	if(argc < 4){
		std::cout << "Not enough arguments!\n";
		std::cout << "Usage: ./triangulate FILE_TO_BE_TRIANGULATED GROUND_TRUTH_POINTS OUTPUT_FILE\n";
		return 0;
	}

	// File handling.
	std::string filename = argv[1];
	std::string gtruth_filename = argv[2];
	std::string out_filename = argv[3];
	std::ifstream file(filename);
	std::ifstream gtruth_file(gtruth_filename);
	std::ofstream out_file(out_filename);

	output_header(out_file);
	int final_order = 10;

	for(int i = 0; i <= final_order; ++i){
		std::cout << " -- RUN " << i << " --\n";

		HODT t1(i);
		HODT t2(i);
		
		// Reading input files.
		int aux1, aux2, aux3, size;
		double x, y, nn;
		file >> size >> aux1 >> aux2 >> aux3;
		for(int i = 0; i < size; ++i){
			file >> nn >> x >> y;
			Point p(x, y);
			Vertex_iterator v1 = t1.insert(p);
			Vertex_iterator v2 = t2.insert(p);
			v1->info( std::pow(x, 2) + std::pow(y, 2) );
			v2->info( std::pow(x, 2) + std::pow(y, 2) );
		}
		std::cout << "Insertion done.\n";

		Optimization_criteria c = ABN;
		std::chrono::high_resolution_clock::time_point start;
		std::chrono::high_resolution_clock::time_point end;
		std::chrono::duration<double> elapsed1;
		std::chrono::duration<double> elapsed2;

		// Optimizing both triangulations.
		std::cout << "Initializing optimization.\n";
		start = std::chrono::high_resolution_clock::now();
		t1.optimize(c);
		end = std::chrono::high_resolution_clock::now();
		elapsed1 = end - start;
		std::cout << "Optimization done.\n";

		std::cout << "Starting brute force optimization.\n";
		start = std::chrono::high_resolution_clock::now();
		t2.optimize_brute_force(c);
		end = std::chrono::high_resolution_clock::now();
		elapsed2 = end - start;
		std::cout << "Optimization done.\n";
		
		// Obtaining the results.	
		double abn1 = t1.global_max_criteria(c);
		double abn2 = t2.global_max_criteria(c);
		double avg_aratio1 = t1.average_aspect_ratio();
		double avg_aratio2 = t2.average_aspect_ratio();
		int order1 = t1.brute_order();
		int order2 = t2.brute_order();
		
		output_results(out_file, i, abn1, abn2, avg_aratio1, avg_aratio2, order1, order2,
			       elapsed1.count(), elapsed2.count());
		std::cout << "Results written.\n\n";

		// Going back the beginning of the file.
		file.clear();
		file.seekg(0, std::ios::beg);

		// Outputting the triangulation.
		std::string name =  std::to_string(i); 
		t1.show_triangulation(name);
	}
	
	return 0;
}
