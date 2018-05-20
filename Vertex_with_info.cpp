#include "Vertex_with_info.h"

void Vertex_with_info::info(double nf)
{
	_info = nf;
}

double Vertex_with_info::info()
{
	return _info;
}

Vertex_with_info& Vertex_with_info::operator=(const Vertex_with_info& v)
{
	_point = v._point;
	_incident_face = v._incident_face;
	_info = v._info;
	return *this;
}

bool Vertex_with_info::operator==(const Vertex_with_info& v)
{
	return (_point == v._point && _incident_face == v._incident_face &&
		_info == v._info);
}

bool Vertex_with_info::operator!=(const Vertex_with_info& v)
{
	return !(*this == v);
}
