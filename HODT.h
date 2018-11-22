#ifndef HODT_H
#define HODT_H

#include <stack>
#include <algorithm>
#include <boost/math/constants/constants.hpp>

#include "Delaunay_triangulation.h"
#include "Point_with_info.h"

typedef enum{
	ABN, WABN, JND
} Optimization_criteria;

class HODT : public Delaunay_triangulation
{
	public:
	HODT() : Delaunay_triangulation(), _max_order(0) {}
	HODT(int m_order) : Delaunay_triangulation(), _max_order(m_order) {}

	// INSERTION.
	// Only the insertion for dimension has changed, and for lower 
	// dimension it is inherited from Delaunay Triangulation.
	/*Vertex_iterator insert(Point& p);
	Vertex_iterator insert_dimension_2(Point& p);
	Vertex_iterator insert_in_face(Face_iterator fc, Point& p, Vertex_location lc, int li);
	Vertex_iterator insert_on_edge_2(Face_iterator fc, Point& p, Vertex_location lc, int li);
	Vertex_iterator insert_outside_convex_hull_2(Face_iterator fc, Point& p, Vertex_location lc, int li);*/

	double abn(Face_iterator fc, int i);
	double abn(Face_iterator fc, Face_iterator ff);
	double wabn(Face_iterator fc, int i);
	double wabn(Face_iterator fc, Face_iterator ff);
	double jnd(Face_iterator fc, int i);
	double jnd(Face_iterator fc, Face_iterator ff);
	double edge_length(Face_iterator fc, int i);
	double edge_length(Vertex_iterator v, Vertex_iterator u);
	double distance_point_segment(const Point& y, const Point& p, const Point& r);
	bool valid_boundary(Face_iterator fc);
	bool valid_whole_boundary(Face_iterator fc);
	bool out_of_bound(Vertex_iterator v);
	bool out_of_bound(Face_iterator fc);
	double aspect_ratio(Face_iterator fc);
	double max_aspect_ratio();
	double average_aspect_ratio();
	double criteria_result(Face_iterator fc, int i, Optimization_criteria& c);
	double criteria_result(Face_iterator fc, Face_iterator ff, Optimization_criteria& c);
	double global_min_criteria(Optimization_criteria& c);
	double global_max_criteria(Optimization_criteria& c);
	double global_average_criteria(Optimization_criteria& c);
	double elevation_from_face(Face_iterator fc, Point& p);
	double rmse_point(Point& p, double actual_elev);
	double rmse(std::vector<std::pair<Point, double>>& vec);
	bool convex_polygon(Face_iterator fc, int i);
	bool convex_polygon(Face_iterator fc, Face_iterator ff);
	double optimize(Optimization_criteria& c);
	bool locally_optimal(Optimization_criteria& c);
	void optimize_brute_force(Optimization_criteria& c);

	int face_order(Face_iterator fc);
	int brute_order();
	int order();
	void max_order(int m_order);
	int max_order();
	void lb_tr_vertices();

	void print_face(Face_iterator fc);

	private:
	int _max_order;
	//Vertex_iterator _lb = nullptr;
	//Vertex_iterator _tr = nullptr;
	int _min_x = 0;
	int _min_y = 0;
	int _max_x = 0;
	int _max_y = 0;
};

#endif
