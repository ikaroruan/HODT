#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <chrono>

#include "HODT.h"

void output_results(std::ostream& out, int max_order, double abn1, double abn2,
		    double avg_aratio1, double avg_aratio2, int order1, double order2,
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
	    << std::setw(8)  << "Order"
	    << std::setw(8)  << "RMSE1"
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
	int final_order = 20;

	for(int i = 0; i <= final_order; ++i){
		std::cout << " -- RUN " << i << " --\n";

		HODT t1(i);
		//HODT t2(i);
		
		// Reading input files.
		int aux1, aux2, aux3, size;
		double x, y, info;
		std::vector<Point> ins_vector;
		file >> size >> aux1 >> aux2 >> aux3;
		for(int i = 0; i < size; ++i){
			file >> x >> y >> info;
			Point p(x, y);
			p.info(info);
			ins_vector.push_back(p);
			//Vertex_iterator v1 = t1.insert(p);
			//v1->info(info);
		}
		
		Spatial_sort_traits sst;
		CGAL::spatial_sort(ins_vector.begin(), ins_vector.end(), sst);
		for(std::vector<Point>::iterator it = ins_vector.begin(); it != ins_vector.end(); ++it){
			Vertex_iterator v = t1.insert(*it);
			v->info((*it).info());
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

		/*std::cout << "Starting brute force optimization.\n";
		start = std::chrono::high_resolution_clock::now();
		t2.optimize_brute_force(c);
		end = std::chrono::high_resolution_clock::now();
		elapsed2 = end - start;
		std::cout << "Optimization done.\n";*/
		
		// Obtaining the results.	
		double abn1 = t1.global_max_criteria(c);
		//double abn2 = t2.global_max_criteria(c);
		double avg_aratio1 = t1.average_aspect_ratio();
		//double avg_aratio2 = t2.average_aspect_ratio();
		int order1 = t1.brute_order();
		//int order2 = t2.brute_order();
		
		gtruth_file >> size >> aux1 >> aux2 >> aux3;
		std::vector<std::pair<Point, double>> vec;
		for(int i = 0; i < size; ++i){
			gtruth_file >> x >> y >> info;
			Point p(x, y);
			vec.push_back(std::make_pair(p, info));
		}
		double rmse1 = t1.rmse(vec);
		//double rmse2 = t2.rmse(vec);
		std::cout << "RMSE = " << rmse1 << "\n";
		
		output_results(out_file, i, abn1, abn1, avg_aratio1, avg_aratio1, order1, rmse1,
			       elapsed1.count(), elapsed1.count());
		std::cout << "Results written.\n\n";

		// Going back the beginning of the file.
		file.clear();
		file.seekg(0, std::ios::beg);
		gtruth_file.clear();
		gtruth_file.seekg(0, std::ios::beg);

		// Outputting the triangulation.
		std::string name =  std::to_string(i); 
		t1.show_triangulation(name);
	}
	
	std::system("vlc /home/ikaroruan/Documents/temp/5c0dc7899adc94.48583798.mp3");
	return 0;
}
