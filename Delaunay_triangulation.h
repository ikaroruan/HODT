#ifndef DELAUNAY_TRIANGULATION_H
#define DELAUNAY_TRIANGULATION_H

#include <iostream>
#include <chrono>
#include "Triangulation.h"

typedef Point_with_info Pinfo;

class Delaunay_triangulation : public Triangulation
{
	public:
	
	// INSERTION
	Vertex_iterator insert(Pinfo& p);
	Vertex_iterator insert_first(Pinfo& p);
	Vertex_iterator insert_second(Pinfo& p);
	Vertex_iterator insert_in_face(Face_iterator fc, Pinfo& p, Vertex_location lc, int li);
	Vertex_iterator insert_in_hole(Face_iterator fc, Pinfo& p, int li, Vertex_location lc);
	Vertex_iterator insert_on_edge_2(Face_iterator fc, Pinfo& p, Vertex_location lc, int li);
	Vertex_iterator insert_dimension_2(Pinfo& p);
	Vertex_iterator insert_in_single_face(Face_iterator fc, Pinfo& p);
	Vertex_iterator insert_outside_convex_hull_2(Face_iterator fc, Pinfo& p, Vertex_location lc, int li);
	Vertex_iterator insert_dimension_1(Pinfo& p);
	Vertex_iterator insert_on_edge_1(Face_iterator fc, Pinfo& p, Vertex_location lt, int li);
	Vertex_iterator insert_outside_convex_hull_1(Pinfo& p, Face_iterator fc, int li);
	void 	insert(std::vector<Pinfo>::iterator start, std::vector<Pinfo>::iterator end);

	// REMOVAL
	//void remove(Pinfo& p);
	void remove(Vertex_iterator v);
	void remove_dim_0(Vertex_iterator v);
	void remove_dim_1(Vertex_iterator v);
	void remove_dim_2(Vertex_iterator v);
	void remove_dim_decrease(Vertex_iterator v);

	// GEOMETRIC PREDICATES
	double incircle_test(Vertex_iterator va, Vertex_iterator vb, Vertex_iterator vc, Vertex_iterator vd);
	double incircle_test(Pinfo& a, Pinfo& b, Pinfo& c, Pinfo& d);

	// UTILITIES
	void conflict_region(Face_iterator fc, Pinfo& pp, std::vector<Face_iterator>& region, std::vector<std::pair<Face_iterator, int>>& cavity, Vertex_location lc, int li);
	void flip_check(std::queue<std::pair<Face_iterator, int>>& queue);

#ifndef NDEBUG
	std::chrono::duration<double> elapsed_locate;
	std::chrono::duration<double> elapsed_conflict;
	std::chrono::duration<double> elapsed_update;
#endif
};

#endif // DELAUNAY_TRIANGULATION_H
