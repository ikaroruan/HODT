#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include "HODT.h"

int main(int argc, char** argv)
{

	Point u(3, 2);
	Point v(5, 5);
	Point w(-5, 4);

	HODT t;
	Vertex_iterator vit = nullptr;
	vit = t.insert(u);
	vit->info(2);
	vit = t.insert(v);
	vit->info(-3);
	vit = t.insert(w);
	vit->info(7);

	Point a(4.75, 4.75);
	Point b(-2, 2);
	Point c(0, 0);
	std::vector<std::pair<Point, double>> vec;
	vec.push_back(std::make_pair(a, -2.40));
	vec.push_back(std::make_pair(b, 6.70));
	vec.push_back(std::make_pair(c, 6.56));
	double result = t.rmse(vec);
	std::cout << "RMSE = " << result << "\n";
	/*if(argc < 2){
		std::cerr << "Not enough arguments.\n";
		return 0;
	}
	int input_order = std::atof(argv[1]);
	HODT t(input_order);
	std::ifstream input("toys/KinzelSprings-VIP-3pct-rnd-copy.cin");
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

	std::cout << "\nOptimizing triangulation with ABN criteria\n";
	Optimization_criteria c = ABN;
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;
	std::chrono::duration<double> elapsed;

	start = std::chrono::high_resolution_clock::now();
	double max_abn = t.optimize(c);
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	std::cout << "Optimization done.\n\n";
	std::cout << "Running time = " << elapsed.count() << "s.\n";
	std::cout << "Max aspect ratio = " << t.max_aspect_ratio() << "\n";
	std::cout << "Avg aspect ratio = " << t.average_aspect_ratio() << "\n";

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

	return 0;*/
}
