#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <chrono>

#include "HODT.h"

int main(int argc, char** argv)
{
	/*Point p1(0,0);
	Point p2(1, 0);
	Point p3(0, 1);
	Point p4(1, 1);
	HODT t;

	t.insert(p1);
	t.insert(p2);
	Vertex_iterator v = t.insert(p3);
	t.insert(p4);
	Face_iterator fc = v->incident_face();
	Face_iterator fn = nullptr;
	int j = -1;

	if(t.is_infinite(fc))
		std::cout << "ERROR: fc is infinite.\n";
	else{
		for(int i = 0; i < 3; ++i){
			if(!t.is_infinite(fc->neighbor(i))){
				fn = fc->neighbor(i);
				//t.flip(fc, i);
			}
		}
	}
	j = fc->index(fn);
	std::cout << "The number is " << j << "\n";
	//t.flip(fc, fc->index(fn));
	
	t.show_triangulation();
	t.output_triangulation();

	return 0;*/
	
	/*HODT t;
	Point a(1, 1);
	Point b(2, 2);
	Point c(2, -2);
	Point d(3, 2);

	Vertex_iterator v = nullptr;
	Vertex_iterator vi = nullptr;
	v = t.insert(a);
	v->info(0.75);
	v = t.insert(b);
	v->info(0.77);
	v = t.insert(c);
	v->info(0.80);
	vi = v;
	v = t.insert(d);
	v->info(0.75);
	
	Face_iterator fci = v->incident_face();
	Face_iterator fc = nullptr;
	for(int i = 0; i < 3; ++i){
		if(!t.is_infinite(fci->neighbor(i)))
			fc = fci->neighbor(i);
	}
	t.print_face(fc);

	Face_iterator ff = nullptr;
	for(int i = 0; i < 3; ++i){
		if(t.is_infinite(fc->neighbor(i)))
			std::cout << "Infinite_neighbor.\n";
		else{
			ff = fc->neighbor(i);
			std::cout << "Finite neighbor found.\n";
		}
	}
	t.print_face(ff);
	if(ff == nullptr){
		std::cout << "ERROR! No finite neighbor found.\n";
		return 0;
	}

	Optimization_criteria cr = JND;

	double jnd_result = t.criteria_result(fc, ff, cr);

	std::cout << "JND = " << std::fixed << std::setprecision(10) << jnd_result << "\n";
	t.output_triangulation();
	t.show_triangulation();*/



	if(argc < 4){
		std::cerr << "Not enough arguments.\n";
		return 0;
	}
	std::string tfilename = argv[1];
	int input_order = std::atof(argv[2]);
	std::string test_filename = argv[3];
	HODT t(input_order);
	std::ifstream input(tfilename);
	std::ifstream test_file(test_filename);
	int psize, aux1, aux2, aux3;
	double x, y, info;

	input >> psize >> aux1 >> aux2 >> aux3;
	std::cout << "Constructing Triangulation.\n";
	for(int i = 0; i < psize; ++i){
		input >> x >> y >> info;
		Point p(x, y);
		Vertex_iterator v = t.insert(p);
		v->info(info);
	}
	std::cout << "Done.\n";

	std::cout << "Populating vector with ground truth points.\n";
	std::vector<std::pair<Point, double>> vec;
	test_file >> psize >> aux1 >> aux2 >> aux3;
	for(int i = 0; i < psize; ++i){
		test_file >> x >> y >> info;
		Point p(x, y);
		vec.push_back(std::make_pair(p, info));
	}
	std::cout << "Done!\n";

	std::cout << "\nOptimizing triangulation with JND criteria\n";
	Optimization_criteria c = JND;
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;
	std::chrono::duration<double> elapsed;

	start = std::chrono::high_resolution_clock::now();
	t.optimize_brute_force(c);
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	std::cout << "Optimization done.\n\n";
	std::cout << "Running time = " << elapsed.count() << "s.\n";
	std::cout << "Max aspect ratio = " << t.max_aspect_ratio() << "\n";
	std::cout << "Avg aspect ratio = " << t.average_aspect_ratio() << "\n\n";

	std::cout << "Calculating the RMSE...\n";
	double RMSE = t.rmse(vec);
	std::cout << "RMSE = " << RMSE << "\n";

	std::cout << "Checking final order.\n";
	int order = t.brute_order();
	std::cout << "Order = " << order << "\n\n";

	std::cout << "Outputting triangulation.n\n";
	t.show_triangulation();
	std::cout << "Done.\n";

	//HODT dt(0);
	//std::ifstream in("out_cgal.tri");

	//dt.create_triangulation(in);
	//std::cout << "\n-- New test --\n";
	//std::cout << "Max aspect ratio = " << dt.max_aspect_ratio() << "\n";
	//std::cout << "Avg aspect ratio = " << dt.average_aspect_ratio() << "\n";
	//dt.show_triangulation("out_cgal.tri");

	return 0;
}
