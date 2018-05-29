#include <iostream>
#include <fstream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Projection_traits_xy_3.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Segment_2.h>

#include <boost/math/constants/constants.hpp>

struct Vertex_info
{
	double elevation = 0;
};

struct Face_info
{
	bool visited_edge[3];
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Projection_traits_xy_3<K> Gt;
typedef CGAL::Triangulation_vertex_base_with_info_2<Vertex_info, Gt> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Face_info, Gt> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> TDS;
typedef CGAL::Delaunay_triangulation_2<Gt, TDS> Delaunay_triangulation;

typedef K::Point_2 Point;
typedef K::Point_3 Point3;
typedef K::Vector_3 V3d;
typedef K::Triangle_3 Triangle;
typedef K::Segment_2 Segment;

typedef Delaunay_triangulation::Face_handle Face_handle;
typedef Delaunay_triangulation::Vertex_handle Vertex_handle;
typedef Delaunay_triangulation::Finite_faces_iterator Face_iterator;
typedef Delaunay_triangulation::Finite_edges_iterator Edge_iterator;

double ABN(Triangle t1, Triangle t2);
double max_abn(const Delaunay_triangulation& dt);
double min_abn(const Delaunay_triangulation& dt);
double distance_2d(Vertex_handle v1, Vertex_handle v2);
double aspect_ratio(Delaunay_triangulation& dt, Face_handle fc);
double max_aspect_ratio(Delaunay_triangulation& dt);
double average_aspect_ratio(Delaunay_triangulation& dt);

int main(int argc, char** argv)
{
	if(argc < 2){
		std::cerr << "Not enough arguments.\n";
		std::cerr << "./triangulate [file_path]\n";
		return 0;
	}

	std::ifstream input(argv[1]);
	int size, aux1, aux2, aux3;
	double x, y, z;
	std::vector<Point3> vector;

	input >> size >> aux1 >> aux2 >> aux3;
	for(int i = 0; i < size; ++i){
		input >> x >> y >> z;
		Point3 p(x, y, z);
		vector.push_back(p);
	}

	std::cout << "Constructing Delaunay Triangulation...\n";
	Delaunay_triangulation dt(vector.begin(), vector.end());
	std::cout << "Triangulation done.\n\n";

	std::cout << "Max ABN = " << max_abn(dt) << "\n";
	std::cout << "Min ABN = " << min_abn(dt) << "\n";
	std::cout << "Max Aspect Ratio = " << max_aspect_ratio(dt) << "\n";
	std::cout << "Avg Aspect Ratio = " << average_aspect_ratio(dt) << "\n";

	return 0;
}

// Calculating ABN for two faces.
double ABN(Triangle t1, Triangle t2)
{
	V3d n1 = CGAL::unit_normal(t1.vertex(0), t1.vertex(1), t1.vertex(2));
	V3d n2 = CGAL::unit_normal(t2.vertex(0), t2.vertex(1), t2.vertex(2));

	double c = n1.x()*n2.x() + n1.y()*n2.y() + n1.z()*n2.z();
	double a = std::acos(c) * (180.0 / boost::math::constants::pi<double>());
	return a;
}

// Max ABN on Delaunay Triangulation.
double max_abn(const Delaunay_triangulation& dt)
{
	double max = 0;
	for(Edge_iterator it = dt.finite_edges_begin(); it != dt.finite_edges_end(); ++it){
		Face_handle fc = it->first;
		int i = it->second;
		Face_handle fn = fc->neighbor(i);
		double abn = ABN(dt.triangle(fc), dt.triangle(fn));
		if(abn > max)
			max = abn;
	}

	return max;
}

double min_abn(const Delaunay_triangulation& dt)
{
	double min = 0;
	for(Edge_iterator it = dt.finite_edges_begin(); it != dt.finite_edges_end(); ++it){
		Face_handle fc = it->first;
		int i = it->second;
		Face_handle fn = fc->neighbor(i);
		double abn = ABN(dt.triangle(fc), dt.triangle(fn));
		if(it == dt.finite_edges_begin()) min = abn;
		if(abn < min)
			min = abn;
	}

	return min;
}

double distance_2d(Vertex_handle v1, Vertex_handle v2)
{
	Point p1(v1->point().x(), v1->point().y());
	Point p2(v2->point().x(), v2->point().y());

	return std::sqrt(CGAL::squared_distance(p1, p2));
}

double aspect_ratio(Delaunay_triangulation& dt, Face_handle fc)
{
	double max_length = 0;
	int j = 0; // This is the index pointing to the longest edge.
	
	// Finding the longest edge of a face.
	for(int i = 0; i < 3; ++i){
		double length = distance_2d(fc->vertex(dt.ccw(i)), fc->vertex(dt.cw(i)));
		if(length > max_length){
			max_length = length;
			j = i;
		}
	}

	// Calculating minimum distance between segment and point.
	Point p1(fc->vertex(dt.ccw(j))->point().x(), fc->vertex(dt.ccw(j))->point().y());
	Point p2(fc->vertex(dt.cw(j))->point().x(), fc->vertex(dt.cw(j))->point().y());
	Segment segment(p1, p2);

	Point p3(fc->vertex(j)->point().x(), fc->vertex(j)->point().y());
	return (max_length/std::sqrt(CGAL::squared_distance(segment, p3))) - (2/std::sqrt(3));
}

double max_aspect_ratio(Delaunay_triangulation& dt)
{
	double max = 0;
	for(Face_iterator it = dt.finite_faces_begin(); it != dt.finite_faces_end(); ++it){
		double aspect = aspect_ratio(dt, it);
		if(aspect > max)
			max = aspect;
	}

	return max;
}

double average_aspect_ratio(Delaunay_triangulation& dt)
{
	double sum = 0;
	int count = 0;
	for(Face_iterator it = dt.finite_faces_begin(); it != dt.finite_faces_end(); ++it){
		sum += aspect_ratio(dt, it);
		count++;
	}

	return sum/(static_cast<double>(count));
}
