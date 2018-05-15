#ifndef HODT_H
#define HODT_H

#include <stack>

#include "Delaunay_triangulation.h"
#include "Point_with_info.h"

typedef Point_with_info Pinfo;

class HODT : public Delaunay_triangulation
{
	public:
	// INSERTION.
	// Only the insertion for dimension has changed, and for lower 
	// dimension it is inherited from Delaunay Triangulation.
	Vertex_iterator insert(Pinfo& p);
	Vertex_iterator insert_dimension_2(Pinfo& p);
	Vertex_iterator insert_in_face(Face_iterator fc, Pinfo& p, Vertex_location lc, int li);
	Vertex_iterator insert_on_edge_2(Face_iterator fc, Pinfo& p, Vertex_location lc, int li);
	Vertex_iterator insert_outside_convex_hull_2(Face_iterator fc, Pinfo& p, Vertex_location lc, int li);

	int face_order(Face_iterator fc);
	int brute_order();
	int order();

	void print_face(Face_iterator fc);
};

#endif
