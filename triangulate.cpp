#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include "HODT.h"

int main(int argc, char** argv)
{
	HODT t(1);
	std::ifstream input("toys/KinzelSprings-VIP-3pct-rnd.cin");
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
	double max_abn = t.optimize(c);
	std::cout << "Optimization done.\n\n";

	std::cout << "Checking final order.\n";
	int order = t.brute_order();
	std::cout << "Order = " << order << "\n\n";

	std::cout << "Outputting triangulation.n\n";
	t.show_triangulation();
	std::cout << "Done.\n";

	return 0;
}
