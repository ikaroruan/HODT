#ifndef HODT_H
#define HODT_H

#include <stack>
#include <boost/math/constants/constants.hpp>

#include "Delaunay_triangulation.h"
#include "Point_with_info.h"

class HODT : public Delaunay_triangulation
{
	public:
	// INSERTION.
	// Only the insertion for dimension has changed, and for lower 
	// dimension it is inherited from Delaunay Triangulation.
	Vertex_iterator insert(Point& p);
	Vertex_iterator insert_dimension_2(Point& p);
	Vertex_iterator insert_in_face(Face_iterator fc, Point& p, Vertex_location lc, int li);
	Vertex_iterator insert_on_edge_2(Face_iterator fc, Point& p, Vertex_location lc, int li);
	Vertex_iterator insert_outside_convex_hull_2(Face_iterator fc, Point& p, Vertex_location lc, int li);

	double abn(Face_iterator fc, int i);
	double abn(Face_iterator fc, Face_iterator ff);
	double wabn(Face_iterator fc, int i);
	double wabn(Face_iterator fc, Face_iterator ff);
	double edge_length(Face_iterator fc, int i);
	double edge_length(Vertex_iterator v, Vertex_iterator u);

	int face_order(Face_iterator fc);
	int brute_order();
	int order();

	void print_face(Face_iterator fc);
};

#endif
