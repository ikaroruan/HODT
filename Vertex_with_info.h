#ifndef VERTEX_WITH_INFO_H
#define VERTEX_WITH_INFO_H

#include "Vertex.h"

//class Vertex_with_info;
//class Face;
//
//typedef CGAL::Compact_container<Face> Faces_container;
//typedef CGAL::Compact_container<Vertex_with_info> Vertices_container;
//typedef Faces_container::iterator Face_iterator;
//typedef Vertices_container::iterator Vertex_iterator;

class Vertex_with_info : public Vertex
{
	public:
	Vertex_with_info() : Vertex(Point()), _info(0) {}
	Vertex_with_info(const Point& p) : Vertex(p), _info(0) {}
	Vertex_with_info(Face_iterator f) : Vertex(Point(), f), _info(0) {}
	Vertex_with_info(const Point& p, Face_iterator f) : Vertex(p, f), _info(0) {}
	Vertex_with_info(const Point& p, Face_iterator f, double nf) : Vertex(p, f), _info(nf) {}
	Vertex_with_info(const Vertex_with_info& v) : Vertex(v._point, v._incident_face), _info(v._info) {}

	Vertex_with_info& operator=(const Vertex_with_info& v);
	bool operator==(const Vertex_with_info& v);
	bool operator!=(const Vertex_with_info& v);

	void info(double nf);
	double info();

	protected:
	double _info;
};

#endif
