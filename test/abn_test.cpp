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
	Delaunay_triangulation dt;
	std::ifstream input("toys/KinzelSprings-VIP-1pct-rnd.cin");
	int psize, aux1, aux2, aux3;
	double x, y, info;

	input >> psize >> aux1 >> aux2 >> aux3;
	std::cout << "Constructing Triangulation.\n";
	for(int i = 0; i < psize; ++i){
		input >> x >> y >> info;
		Point p(x, y);
		Vertex_iterator v = t.insert(p);
		v->info(info);
		Point pp(x, y);
		dt.insert(pp);
	}
	std::cout << "Done.\n";
	std::cout << "Outputting triangulation.n\n";
	dt.show_triangulation();
	std::cout << "Done.\n";

	// Brute-force, including reapated faces.
	// Just for testing purposes.
	for(Face_iterator it = t.faces().begin(); it != t.faces().end(); ++it){
		for(int i = 0; i < 3; ++i){
			Face_iterator fc = it;
			Face_iterator ff = fc->neighbor(i);
			if(t.is_infinite(fc) || t.is_infinite(ff))
				continue;
			double abn = t.abn(fc, ff);
			
			Triangle t1(P3d(fc->vertex(0)->point().get_x(), fc->vertex(0)->point().get_y(), fc->vertex(0)->info()), 
				    P3d(fc->vertex(1)->point().get_x(), fc->vertex(1)->point().get_y(), fc->vertex(1)->info()),
				    P3d(fc->vertex(2)->point().get_x(), fc->vertex(2)->point().get_y(), fc->vertex(2)->info()));
			Triangle t2(P3d(ff->vertex(0)->point().get_x(), ff->vertex(0)->point().get_y(), ff->vertex(0)->info()), 
				    P3d(ff->vertex(1)->point().get_x(), ff->vertex(1)->point().get_y(), ff->vertex(1)->info()),
				    P3d(ff->vertex(2)->point().get_x(), ff->vertex(2)->point().get_y(), ff->vertex(2)->info()));
			double result = ABN(t1, t2);
			if(abn != result){
				std::cout << "WRONG ABN!\n";
				std::cout << "abn = " << abn << "\n";
				std::cout << "CGAL: abn = " << result << "\n\n";
			}
		}
	}
	std::cout << "Done!\n";


	//std::cout << "ABN = " << t.abn(fc, ff) << "\n";

	//Triangle t1(P3d(fc->vertex(0)->point().get_x(), fc->vertex(0)->point().get_y(), fc->vertex(0)->point().get_info()), 
	//	    P3d(fc->vertex(1)->point().get_x(), fc->vertex(1)->point().get_y(), fc->vertex(1)->point().get_info()),
	//	    P3d(fc->vertex(2)->point().get_x(), fc->vertex(2)->point().get_y(), fc->vertex(2)->point().get_info()));
	//Triangle t2(P3d(ff->vertex(0)->point().get_x(), ff->vertex(0)->point().get_y(), ff->vertex(0)->point().get_info()), 
	//	    P3d(ff->vertex(1)->point().get_x(), ff->vertex(1)->point().get_y(), ff->vertex(1)->point().get_info()),
	//	    P3d(ff->vertex(2)->point().get_x(), ff->vertex(2)->point().get_y(), ff->vertex(2)->point().get_info()));
	//double result = ABN(t1, t2);
	//std::cout << "ABN = " << result << "\n";
	
	return 0;
}

double ABN(Triangle t1, Triangle t2)
{
	V3d n1 = CGAL::unit_normal(t1.vertex(0), t1.vertex(1), t1.vertex(2));
	V3d n2 = CGAL::unit_normal(t2.vertex(0), t2.vertex(1), t2.vertex(2));
	//std::cout << "\n -- CGAL -- \n";
	//std::cout << "Printing t1\n";
	//std::cout << "v0: x = " << t1.vertex(0).x() << " y = " << t1.vertex(0).y() << " z = " << t1.vertex(0).z() <<"\n";
	//std::cout << "v1: x = " << t1.vertex(1).x() << " y = " << t1.vertex(1).y() << " z = " << t1.vertex(1).z() <<"\n";
	//std::cout << "v2: x = " << t1.vertex(2).x() << " y = " << t1.vertex(2).y() << " z = " << t1.vertex(2).z() <<"\n";
	//std::cout << "Printing t2\n";
	//std::cout << "v0: x = " << t2.vertex(0).x() << " y = " << t2.vertex(0).y() << " z = " << t1.vertex(0).z() <<"\n";
	//std::cout << "v1: x = " << t2.vertex(1).x() << " y = " << t2.vertex(1).y() << " z = " << t1.vertex(1).z() <<"\n";
	//std::cout << "v2: x = " << t2.vertex(2).x() << " y = " << t2.vertex(2).y() << " z = " << t1.vertex(2).z() <<"\n\n";
	//std::cout << "n1: x = " << n1.x() << " y = " << n1.y() << " z = " << n1.z() << "\n";
	//std::cout << "n2: x = " << n2.x() << " y = " << n2.y() << " z = " << n2.z() << "\n";
	
	double c = n1.x()*n2.x() + n1.y()*n2.y() + n1.z()*n2.z();
	//std::cout << "Dot Product = " << c << "\n";
	double a = std::acos(c) * (180 / boost::math::constants::pi<double>());
	return a;

}
