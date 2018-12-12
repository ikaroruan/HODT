#include "HODT.h"

void HODT::max_order(int m_order)
{
	_max_order = m_order;
}

int HODT::max_order()
{
	return _max_order;
}

/*
Vertex_iterator HODT::insert(Point& p)
{
	switch(dimension())
	{
		case -1:
			return insert_first(p);
			break;
		case 0:
			return insert_second(p);
			break;
		case 1:
			return insert_dimension_1(p);
			break;
		case 2:
			return insert_dimension_2(p);
			break;
		default:
			std::cerr << "ERROR: wrong dimension. (HODT insert)\n";
	}

	// Should never reach this line.
	return nullptr;
}

Vertex_iterator HODT::insert_dimension_2(Point& p)
{
	Vertex_location lc;
	int li = -1;
	Face_iterator fc = locate(p, lc, li);

	switch(lc){
		case ON_EDGE:
			return insert_on_edge_2(fc, p, lc, li);
			break;
		case ON_INTERIOR:
			return insert_in_face(fc, p, lc, li);
			break;
		case OUTSIDE_CONVEX_HULL:
			return insert_outside_convex_hull_2(fc, p, lc, li);
			break;
		case ON_VERTEX:
			return is_vertex(fc, p);
			break;
		default:
			std::cerr << "ERROR: wrong location for insertion. (HODT insert_dimension_2)\n";
			break;
	}

	// Should never reach this line.
	return nullptr;
}

Vertex_iterator HODT::insert_on_edge_2(Face_iterator fc, Point& p, Vertex_location lc, int li)
{
	Vertex_iterator v = vertices().insert(Vertex_with_info(p));

	// Data for neighborhood.
	Face_iterator nli = fc->neighbor(li);
	int loi = nli->index(fc);
	Face_iterator nccw = fc->neighbor(ccw(li));
	Face_iterator ncw = fc->neighbor(cw(li));
	Face_iterator nnccw = nli->neighbor(ccw(loi));
	Face_iterator nncw = nli->neighbor(cw(loi));

	// Creating faces.
	Face_iterator new_face = faces().insert(Face(fc->vertex(cw(li)), fc->vertex(li), v));
	Face_iterator nnli = faces().insert(Face(nli->vertex(loi), nli->vertex(ccw(loi)), v));
	fc->vertex(v, cw(li));
	nli->vertex(v, ccw(loi));
	v->incident_face(new_face);

	// Setting up neighborhood.
	new_face->neighbor(nccw, new_face->index(v));
	new_face->neighbor(fc, ccw(new_face->index(v)));
	new_face->neighbor(nnli, cw(new_face->index(v)));
	nnli->neighbor(nncw, nnli->index(v));
	nnli->neighbor(nli, cw(nnli->index(v)));
	nnli->neighbor(new_face, ccw(nnli->index(v)));
	fc->neighbor(ncw, fc->index(v));
	fc->neighbor(new_face, cw(fc->index(v)));
	fc->neighbor(nli, ccw(fc->index(v)));
	nli->neighbor(nnccw, nli->index(v));
	nli->neighbor(fc, cw(nli->index(v)));
	nli->neighbor(nnli, ccw(nli->index(v)));
	ncw->neighbor(fc, ncw->index(fc));
	nccw->neighbor(new_face, nccw->index(fc));
	nncw->neighbor(nnli, nncw->index(nli));
	nnccw->neighbor(nli, nnccw->index(nli));

	key_face(new_face);

	return v;
}

Vertex_iterator HODT::insert_outside_convex_hull_2(Face_iterator fc, Point& p, Vertex_location lc, int li)
{
	std::list<Face_iterator> list;
	bool done = false;
	Vertex_iterator v = vertices().insert(Vertex_with_info(p));
	Face_iterator temp = fc;
	Vertex_iterator lastccw_vertex = nullptr;
	Vertex_iterator lastcw_vertex = nullptr;
	Face_iterator n0 = nullptr;
	Face_iterator n1 = nullptr;

	// Turning counterclockwise searching for visible faces.
	while(!done){
		int infinite_index = temp->index(infinite_vertex());
		if(orientation_test(temp->vertex(ccw(infinite_index)), temp->vertex(cw(infinite_index)), v) > 0){
			list.push_back(temp);
			temp = temp->neighbor(ccw(infinite_index));
		}
		else{
			lastccw_vertex = temp->vertex(ccw(temp->index(infinite_vertex())));
			n0 = temp;
			done = true;
		}
	}
	done = false;
	temp = fc->neighbor(cw(fc->index(infinite_vertex())));

	// Now, turning clockwise.
	while(!done){
		int infinite_index = temp->index(infinite_vertex());
		if(orientation_test(temp->vertex(ccw(infinite_index)), temp->vertex(cw(infinite_index)), v) > 0){
			list.push_back(temp);
			temp = temp->neighbor(cw(infinite_index));
		}
		else{
			lastcw_vertex = temp->vertex(cw(temp->index(infinite_vertex())));
			n1 = temp;
			done = true;
		}
	}

	// Creating new infinite faces.
	Face_iterator ninf1 = faces().insert(Face(v, lastccw_vertex, infinite_vertex()));
	Face_iterator ninf2 = faces().insert(Face(lastcw_vertex, v, infinite_vertex()));
	infinite_vertex()->incident_face(ninf1);

	// Setting the new vertex on faces.
	for(std::list<Face_iterator>::iterator it = list.begin(); it != list.end(); ++it)
		(*it)->vertex(v, (*it)->index(infinite_vertex()));
	v->incident_face(*(list.begin()));
	key_face(*(list.begin()));

	
	// Setting up neighborhood.
	Face_iterator nf1 = n1->neighbor(ccw(n1->index(infinite_vertex())));
	Face_iterator nf0 = n0->neighbor(cw(n0->index(infinite_vertex())));

	nf0->neighbor(ninf1, nf0->index(n0));
	ninf1->neighbor(nf0, ninf1->index(infinite_vertex()));
	n0->neighbor(ninf1, n0->index(nf0));
	ninf1->neighbor(n0, ninf1->index(v));
	nf1->neighbor(ninf2, nf1->index(n1));
	ninf2->neighbor(nf1, ninf2->index(infinite_vertex()));
	ninf2->neighbor(n1, ninf2->index(v));
	n1->neighbor(ninf2, n1->index(nf1));
	ninf1->neighbor(ninf2, ccw(ninf1->index(v)));
	ninf2->neighbor(ninf1, cw(ninf2->index(v)));

	return v;
}

Vertex_iterator HODT::insert_in_face(Face_iterator fc, Point& p, Vertex_location lc, int li)
{
	Vertex_iterator v = vertices().insert(Vertex_with_info(p));

	// Storing neighborhood.
	Face_iterator n0 = fc->neighbor(0);
	Face_iterator n1 = fc->neighbor(1);
	Face_iterator n2 = fc->neighbor(2);

	// Creating new faces.
	Face_iterator nf1 = faces().insert(Face(fc->vertex(1), fc->vertex(2), v));
	Face_iterator nf2 = faces().insert(Face(fc->vertex(2), fc->vertex(0), v));
	fc->vertex(v, 2);
	v->incident_face(nf1);

	// Setting up neighborhood.
	fc->neighbor(nf1, ccw(fc->index(v)));
	fc->neighbor(nf2, cw(fc->index(v)));
	fc->neighbor(n2, fc->index(v));
	nf1->neighbor(n0, nf1->index(v));
	nf1->neighbor(nf2, ccw(nf1->index(v)));
	nf1->neighbor(fc, cw(nf1->index(v)));
	n0->neighbor(nf1, n0->index(fc));
	nf2->neighbor(n1, nf2->index(v));
	nf2->neighbor(fc, ccw(nf2->index(v)));
	nf2->neighbor(nf1, cw(nf2->index(v)));
	n1->neighbor(nf2, n1->index(fc));

	key_face(nf1);

	return v;
}*/

int HODT::face_order(Face_iterator fc)
{
	if(is_infinite(fc))
		return 0;
	
	int f_order = 0;
	
	for(Vertex_iterator vi = vertices().begin(); vi != vertices().end(); ++vi){
		if(vi != infinite_vertex() && vi != fc->vertex(0) && vi != fc->vertex(1) && vi != fc->vertex(2) && !out_of_bound(vi)){
			if(incircle_test(fc->vertex(0), fc->vertex(1), fc->vertex(2), vi) > 0)
				f_order++;
		}
	}

	return f_order;
}

int HODT::brute_order()
{
	int order = 0;
	for(Face_iterator it = faces().begin(); it != faces().end(); ++it){
		if(!out_of_bound(it)){
			int f_order = face_order(it);
			if(f_order > order)
				order = f_order;
		}
	}

	return order;
}

int HODT::order()
{
	int t_order = 0;

	for(Face_iterator it = faces().begin(); it != faces().end(); ++it){
		if(!out_of_bound(it)){
			int f_order = face_order(it);
			if(f_order > t_order)
				t_order = f_order;
		}
	}

	return t_order;
}

// Angle between normals criteria.
double HODT::abn(Face_iterator fc, int i)
{
	return abn(fc, fc->neighbor(i));
}

double HODT::abn(Face_iterator fc, Face_iterator ff)
{
	Vector3d n1 = fc->unit_normal();
	Vector3d n2 = ff->unit_normal();

	double dot_product = n1 * n2;
	//std::cout << "dot_product = " << std::setprecision(20) << dot_product << "\n";
	//std::cout << "acos = " << std::setprecision(20) << std::acos(dot_product) << "\n\n";

	if(dot_product >= 1) return 0;

	return std::acos(dot_product) * (180.0/boost::math::constants::pi<double>());
}

// Edge is implicit in the data structure as edge between two neighbor faces.
double HODT::edge_length(Face_iterator fc, int i)
{
	return edge_length(fc->vertex(ccw(i)), fc->vertex(cw(i)));
}

double HODT::edge_length(Vertex_iterator v, Vertex_iterator u)
{
	return std::sqrt(std::pow(v->point().get_x() - u->point().get_x(), 2) +
			 std::pow(v->point().get_y() - u->point().get_y(), 2) +
			 std::pow(v->info() - u->info(), 2));
}

// Distante between point y and segment pr.
double HODT::distance_point_segment(const Point& y, const Point& p, const Point& r)
{
	Vector3d d(r.get_x() - p.get_x(), r.get_y() - p.get_y(), 0);
	d.normalize();
	Vector3d u(y.get_x() - p.get_x(), y.get_y() - p.get_y(), 0);

	return std::sqrt( u.squared_norm() - std::pow( d*u, 2));
}

// Calculating the Aspect Ratio.
double HODT::aspect_ratio(Face_iterator fc)
{
	int j = 0; // Index to longest edge.
	double max_length = 0;
	for(int i = 0; i < 3; ++i){
		double length = edge_length(fc, i);
		if(length > max_length){
			max_length = length;
			j = i;
		}
	}

	double s_distance = distance_point_segment(fc->vertex(j)->point(), fc->vertex(ccw(j))->point(),
						  fc->vertex(cw(j))->point());
	return (max_length/s_distance) - (2/std::sqrt(3));
}

// Weighted angle between normals, where the edge length is the weight.
double HODT::wabn(Face_iterator fc, int i)
{
	return wabn(fc, fc->neighbor(i));
}

double HODT::wabn(Face_iterator fc, Face_iterator ff)
{
	return edge_length(fc, fc->index(ff)) * abn(fc, ff);
}

// Calculating the Jump Normal Derivative - N2y) * normal.get_y());.
double HODT::jnd(Face_iterator fc, Face_iterator ff)
{
	return jnd(fc, fc->index(ff));
}

double HODT::jnd(Face_iterator fc, int i)
{
	Vertex_iterator v = fc->vertex(ccw(i));
	Vertex_iterator u = fc->vertex(cw(i));

	//std::cout << "  v: x = " << v->point().get_x() << "   y = " << v->point().get_y() << "\n";
	//std::cout << "  u: x = " << u->point().get_x() << "   y = " << u->point().get_y() << "\n";
	
	Vector3d normal(-1*(u->point().get_y() - v->point().get_y()), 
			u->point().get_x() - v->point().get_x(), 0);
	normal.normalize();
	//normal.print();

	Vertex_iterator w1 = fc->vertex(i);
	Vertex_iterator w2 = fc->neighbor(i)->vertex(mirror_index(fc, i));

	Vector3d uv(u->point().get_x() - v->point().get_x(),
		    u->point().get_y() - v->point().get_y(),
		    u->info() - v->info());
	Vector3d w1v(w1->point().get_x() - v->point().get_x(),
		     w1->point().get_y() - v->point().get_y(),
		     w1->info() - v->info());
	Vector3d w2v(w2->point().get_x() - v->point().get_x(),
		     w2->point().get_y() - v->point().get_y(),
		     w2->info() - v->info());
	//uv.print();
	//w1v.print();
	//w2v.print();

	double N1x = uv.get_y() * w1v.get_z() - uv.get_z() * w1v.get_y();
	double N1y = uv.get_z() * w1v.get_x() - uv.get_x() * w1v.get_z();
	double N2x = uv.get_y() * w2v.get_z() - uv.get_z() * w2v.get_y();
	double N2y = uv.get_z() * w2v.get_x() - uv.get_x() * w2v.get_z();

	return std::abs((N1x - N2x) * normal.get_x() - (N1y - N2y) * normal.get_y());
}

// Checking wheter or not two faces forms a convex polygon.
bool HODT::convex_polygon(Face_iterator fc, int i)
{
	return convex_polygon(fc, fc->neighbor(i));
}

bool HODT::convex_polygon(Face_iterator fc, Face_iterator ff)
{
	int i = fc->index(ff);
	Vertex_iterator u = fc->vertex(i);
	int j = ff->index(fc);
	Vertex_iterator v = ff->vertex(j);
	
	double key1 = orientation_test(u, v, fc->vertex(ccw(i)));
	double key2 = orientation_test(u, v, fc->vertex(cw(i)));

	// FIXME: If three points are collinear the faces still forms a convex polygon, 
	// but this situation may not be useful for flipping faces.
	return !((key1 > 0 && key2 > 0) || (key1 < 0 && key2 < 0));
}

double HODT::max_aspect_ratio()
{
	double max_aspect = 0;
	for(Face_iterator it = faces().begin(); it != faces().end(); ++it){
		if(!is_infinite(it) && !out_of_bound(it)){
			double aspect = aspect_ratio(it);
			if(aspect > max_aspect)
				max_aspect = aspect;
		}
	}

	return max_aspect;
}

double HODT::average_aspect_ratio()
{
	double sum = 0;
	int count = 0;
	for(Face_iterator it = faces().begin(); it != faces().end(); ++it){
		if(!is_infinite(it) && !out_of_bound(it)){
			sum += aspect_ratio(it);
			count++;
		}
	}

	return sum/(static_cast<double>(count));
}

double HODT::criteria_result(Face_iterator fc, int i, Optimization_criteria& c)
{
	return criteria_result(fc, fc->neighbor(i), c);
}

double HODT::criteria_result(Face_iterator fc, Face_iterator ff, Optimization_criteria& c)
{
	switch(c)
	{
		case ABN:
			return abn(fc, ff);
			break;
		case WABN:
			return wabn(fc, ff);
			break;
		case JND:
			return jnd(fc, ff);
			break;
		default:
			std::cerr << "ERROR: Wrong optimization criteria. (criteria_result function)\n";
			break;
	}
	
	// Error sign.
	return -1;
}

double HODT::global_min_criteria(Optimization_criteria& c)
{
	double min_face = 0;
	int count = 0;
	for(Face_iterator it = faces().begin(); it != faces().end(); ++it){
		if(!is_infinite(it)){
			for(int i = 0; i < 3; ++i){
				if(!is_infinite(it->neighbor(i)) && !out_of_bound(it) && !out_of_bound(it->neighbor(i))){
					double abn_result = criteria_result(it, it->neighbor(i), c);
					if(count == 0) min_face = abn_result;
					count++;
					if(abn_result < min_face)
						min_face = abn_result;
				}
			}
		}
	}

	return min_face;
}

double HODT::global_max_criteria(Optimization_criteria& c)
{
	double max_face = 0;
	for(Face_iterator it = faces().begin(); it != faces().end(); ++it){
		if(!is_infinite(it)){
			for(int i = 0; i < 3; ++i){
				if(convex_polygon(it, it->neighbor(i)) && !is_infinite(it->neighbor(i)) && 
				   !out_of_bound(it) && !out_of_bound(it->neighbor(i))){
					double abn_result = criteria_result(it, it->neighbor(i), c);
					if(abn_result > max_face){
						max_face = abn_result;
					}
				}
			}
		}
	}

	return max_face;
}

double HODT::global_average_criteria(Optimization_criteria& c)
{
	double sum = 0;
	double count = 0;
	for(Face_iterator it = faces().begin(); it != faces().end(); ++it){
		if(!is_infinite(it)){
			for(int i = 0; i < 3; ++i){
				if(!is_infinite(it->neighbor(i))){
					double abn_result = criteria_result(it, it->neighbor(i), c);
					sum += abn_result;
					count++;
				}
			}
		}
	}

	return (sum/count);
}

bool HODT::valid_boundary(Face_iterator fc)
{
	if(is_infinite(fc))
		return false;

	// Checking the 5% distance.
	for(int i = 0; i < 3; ++i){
		if(is_infinite(fc->neighbor(i))){
			double vertex_to_edge = distance_point_segment(fc->vertex(i)->point(), 
						   		       fc->vertex(ccw(i))->point(),
								       fc->vertex(cw(i))->point());
			double e_length = edge_length(fc, i);
			if(vertex_to_edge <= 0.05*e_length)
				return false;

		}
	}

	return true;
}

void HODT::lb_tr_vertices()
{
	Vertex_iterator v = vertices().begin();
	Vertex_iterator lb = v == infinite_vertex() ? ++v : v;
	Vertex_iterator tr = v;
	double max_length = 0;
	for(Vertex_iterator vi = vertices().begin(); vi != vertices().end(); ++vi){
		for(Vertex_iterator vt = vertices().begin(); vt != vertices().end(); ++vt)
		{
			if(vt != infinite_vertex() && vi != infinite_vertex()){
				if(edge_length(vi, vt) > max_length){
					lb = vi;
					tr = vt;
					max_length = edge_length(vi, vt);
				}
			}
		}
		/*if(vi != infinite_vertex()){

			if(vi->point().get_x() <= lb->point().get_x() && vi->point().get_y() <= lb->point().get_y()){
				lb = vi;
			}
			if(vi->point().get_x() >= tr->point().get_x() && vi->point().get_y() >= tr->point().get_y()){
				std::cout << "Hey " << vi->point().get_x() << "\n";
				tr = vi;
			}
		}*/
	}

	if(lb->point().get_x() > tr->point().get_x())
		std::swap(lb, tr);

	//_tr = tr;
	//_lb = lb;
	//std::cout << "lb: x = " << lb->point().get_x() << "   y = " << lb->point().get_y() << "\n";
	//std::cout << "tr: x = " << tr->point().get_x() << "   y = " << tr->point().get_y() << "\n";

	/*double min_x = 0;
	double min_y = 0;
	double max_x = 0;
	double max_y = 0;
	int count = 0;
	for(Vertex_iterator vi = vertices().begin(); vi != vertices().end(); ++vi){
		if(vi != infinite_vertex()){
			if(count == 0){ // Initialize min values.
				min_x = vi->point().get_x();
				min_y = vi->point().get_y();
				count++;
			}

			if(min_x > vi->point().get_x())
				min_x = vi->point().get_x();
			if(max_x < vi->point().get_x())
				max_x = vi->point().get_x();
			if(min_y > vi->point().get_y())
				min_y = vi->point().get_y();
			if(max_y < vi->point().get_y())
				max_y = vi->point().get_y();
		}
	}

	_min_x = min_x;
	_min_y = min_y;
	_max_x = max_x;
	_max_y = max_y;*/

	// Finding vertex with longest distance from lb
	/*double max_dist = 0;
	for(Vertex_iterator vi = vertices().begin(); vi != vertices().end(); ++vi){
		if(vi != infinite_vertex()){
			double length = edge_length(lb, vi);
			if(length > max_dist){
				tr = vi;
				max_dist = length;
			}
		}
	}*/

	_min_x = lb->point().get_x();
	_min_y = lb->point().get_y();
	_max_x = tr->point().get_x();
	_max_y = tr->point().get_y();
}

bool HODT::out_of_bound(Vertex_iterator v)
{
	if(_min_x == 0 && _min_y == 0 && _max_x == 0 && _max_y == 0)
		lb_tr_vertices();

	double percent = 0.05;
	double width = _max_x - _min_x;
	double height = _max_y - _min_y;
	if(v->point().get_x() < _min_x + percent*width)
		return true;
	if(v->point().get_x() > _max_x - percent*width)
		return true;
	if(v->point().get_y() < _min_y + percent*height)
		return true;
	if(v->point().get_y() > _max_y - percent*height)
		return true;
	
	return false;
}

double HODT::area_from_face(Point p0, Point p1, Point p2)
{
	double area = 0.5 * (p0.get_y() * p2.get_x() + p1.get_x() * p2.get_y() + p0.get_x() * p1.get_y()
	        	    -p0.get_x() * p2.get_y() - p0.get_y() * p1.get_x() - p2.get_x() * p1.get_y());

	// Points need to be given in counterclockwise order to result in positive value. Otherwise, the result will be negative.
	// However, the absolute value is correct.
	if(area < 0)
		area = area * -1;

	return area;
}

double HODT::elevation_from_face(Face_iterator fc, Point& p)
{
	double area0 = area_from_face(fc->vertex(1)->point(), fc->vertex(2)->point(), p);
	double area1 = area_from_face(fc->vertex(2)->point(), fc->vertex(0)->point(), p);
	double area2 = area_from_face(fc->vertex(0)->point(), fc->vertex(1)->point(), p);
	double total = area0 + area1 + area2;

	return (area0/total) * fc->vertex(0)->info() + (area1/total) * fc->vertex(1)->info() + (area2/total) * fc->vertex(2)->info();
}

double HODT::elevation_from_face_deprecated(Face_iterator fc, Point& p)
{
	Vector3d u(fc->vertex(0)->point().get_x(), fc->vertex(0)->point().get_y(), fc->vertex(0)->info());
	Vector3d v(fc->vertex(1)->point().get_x(), fc->vertex(1)->point().get_y(), fc->vertex(1)->info());
	Vector3d w(fc->vertex(2)->point().get_x(), fc->vertex(2)->point().get_y(), fc->vertex(2)->info());
	
	Vector3d vu = v - u;
	Vector3d wv = w - v;

	Vector3d normal = vu.cross_product(wv);

	return ((-1 * normal.get_x() * p.get_x()) + (normal.get_x() * u.get_x()) - (normal.get_y() * p.get_y()) + 
		(normal.get_y() * u.get_y()) + (normal.get_z() * u.get_z()))/normal.get_z();
}

double HODT::rmse_point(Point& p, double actual_elev)
{
	Vertex_location lc;
	int li = -1;
	Face_iterator fc = locate(p, lc, li);
	if(is_infinite(fc))
		fc = fc->neighbor(fc->index(infinite_vertex()));

	double el = elevation_from_face(fc, p);

	return std::pow(el - actual_elev, 2);
}

double HODT::rmse(std::vector<std::pair<Point, double>>& vec)
{
	double sq_diff = 0;
	for(std::vector<std::pair<Point, double>>::iterator it = vec.begin(); it != vec.end(); ++it){
		std::pair<Point, double> pair = *it;
		Point p = std::get<0>(pair);
		double actual_elev = std::get<1>(pair);
		sq_diff += rmse_point(p, actual_elev);
	}

	return std::sqrt(sq_diff/vec.size());
}

bool HODT::out_of_bound(Face_iterator fc)
{
	return (out_of_bound(fc->vertex(0)) || out_of_bound(fc->vertex(1)) || out_of_bound(fc->vertex(2)));
}

bool HODT::valid_whole_boundary(Face_iterator fc)
{
	Face_circulator ff(infinite_vertex());
	Face_iterator begin = ff.current_face();

	do{
		Face_iterator fh = ff.current_face();
		int index = fh->index(infinite_vertex());
		double e_length = edge_length(fh, index);

		for(int i = 0; i < 3; ++i){
			double p_segment = distance_point_segment(fc->vertex(i)->point(),
								  fh->vertex(ccw(index))->point(),
								  fh->vertex(cw(index))->point());

			if(p_segment < 0.05*e_length)
				return false;
		}
		ff.next();
	}while(ff.current_face() != begin);

	return true;
}

bool HODT::locally_optimal(Optimization_criteria& c)
{
	for(Face_iterator it = faces().begin(); it != faces().end(); ++it){
		if(!is_infinite(it)){
			for(int i = 0; i < 3; ++i){ // Iterating over the three neighbors. For now, it's okay to double check an edge.
				if(!is_infinite(it->neighbor(i))){
					if(convex_polygon(it, it->neighbor(i)) && !out_of_bound(it) && !out_of_bound(it->neighbor(i))){
						double initial_value = std::max(criteria_result(it, i, c), 
									std::max(criteria_result(it, ccw(i), c), 
									std::max(criteria_result(it, cw(i), c), 
									std::max(criteria_result(it->neighbor(i), ccw(mirror_index(it, i)), c),
										 criteria_result(it->neighbor(i), cw(mirror_index(it, i)), c)))));
						Face_iterator fn = it->neighbor(i);
						flip(it, i); // Performing the flip here.
						int it_order = face_order(it);
						int fn_order = face_order(fn);
						double after_value = std::max(criteria_result(it, it->index(fn), c),
									std::max(criteria_result(it, ccw(it->index(fn)), c),
									std::max(criteria_result(it, cw(it->index(fn)), c),
									std::max(criteria_result(fn, ccw(fn->index(it)), c),
										 criteria_result(fn, cw(fn->index(it)), c)))));
						flip(it, it->index(fn)); // Undoing the flip.

						if(it_order <= max_order() && fn_order <= max_order() && after_value < initial_value){
							//std::cout << "== This is " << i << " ==\n";
							//std::cout << "it_order = " << it_order << "\n";
							//std::cout << "fn_order = " << fn_order << "\n";
							//std::cout << "max_order = " << max_order() << "\n";
							//std::cout << "initial_value = " << initial_value << "\n";
							//std::cout << "after_value = " << after_value << "\n\n";
							return false; // Optimization is possible, so the triangulation is not locally optimal.
						}
					}
				}
			} // End of inner for.
		}
	} // End of outer for.
	
	//std::cout << "GOT IT TRUE.\n";
	return true;
}

void HODT::optimize_brute_force(Optimization_criteria& c)
{
	int flips_count = 0;
	int count = 0;

	while(!locally_optimal(c))
	{
		for(Face_iterator it = faces().begin(); it != faces().end(); ++it){
			if(!is_infinite(it)){
				for(int i = 0; i < 3; ++i){ // Iterating over the three neighbors. For now, it's okay to double check an edge.
					if(!is_infinite(it->neighbor(i))){
						if(convex_polygon(it, it->neighbor(i)) && !out_of_bound(it) && !out_of_bound(it->neighbor(i))){
							double initial_value = std::max(criteria_result(it, i, c), 
										std::max(criteria_result(it, ccw(i), c), 
										std::max(criteria_result(it, cw(i), c), 
										std::max(criteria_result(it->neighbor(i), ccw(mirror_index(it, i)), c),
											 criteria_result(it->neighbor(i), cw(mirror_index(it, i)), c)))));
							Face_iterator fn = it->neighbor(i);
							flip(it, i); // Performing the flip here.
							double after_value = std::max(criteria_result(it, it->index(fn), c),
										std::max(criteria_result(it, ccw(it->index(fn)), c),
										std::max(criteria_result(it, cw(it->index(fn)), c),
										std::max(criteria_result(fn, ccw(fn->index(it)), c),
											 criteria_result(fn, cw(fn->index(it)), c)))));
							int it_order = face_order(it);
							int fn_order = face_order(fn);
							if(it_order <= max_order() && fn_order <= max_order() && after_value < initial_value){ // Edge optimized.
								count++;
								flips_count++;}
							else // Undo the flip.
								flip(it, it->index(fn));
						}
					}
				} // End of inner for.
			}
		} // End of outer for.
	} // End of while.
	std::cout << "Number of flips = " << flips_count << "\n";
	double min = global_min_criteria(c);
	double max_result = global_max_criteria(c);
	double average_result = global_average_criteria(c);
	std::cout << "Min ABN = " << min << "\n";
	std::cout << "Max ABN = " << max_result << "\n";
	std::cout << "Average ABN = " << average_result << "\n";
}

double HODT::optimize(Optimization_criteria& c)
{
	int flips_count = 0;
	double max = 0;
	std::queue<std::pair<Face_iterator, int>> queue;

	for(Face_iterator it = faces().begin(); it != faces().end(); ++it){
		if(!is_infinite(it)){
			for(int i = 0; i < 3; ++i){
				if(!is_infinite(it->neighbor(i)) && !out_of_bound(it)){
					queue.push(std::make_pair(it, i));
				}
			}
		}
	}

	while(!queue.empty()){
		std::pair<Face_iterator, int> pair = queue.front();
		Face_iterator fc = std::get<0>(pair);
		int i = std::get<1>(pair);
		queue.pop();
		double max_face = 0;
		if(!is_infinite(fc) && !is_infinite(fc->neighbor(i))){
			if(convex_polygon(fc, fc->neighbor(i)) && !out_of_bound(fc) && !out_of_bound(fc->neighbor(i))){
				double initial_value = std::max(criteria_result(fc, i, c), 
								std::max(criteria_result(fc, ccw(i), c), 
								std::max(criteria_result(fc, cw(i), c), 
								std::max(criteria_result(fc->neighbor(i), ccw(mirror_index(fc, i)), c), 
									 criteria_result(fc->neighbor(i), cw(mirror_index(fc, i)), c)))));
				Face_iterator fn = fc->neighbor(i);
				flip(fc, i);
				double after_value = std::max(criteria_result(fc, fc->index(fn), c), 
							      std::max(criteria_result(fc, ccw(fc->index(fn)), c), 
							      std::max(criteria_result(fc, cw(fc->index(fn)), c), 
							      std::max(criteria_result(fn, ccw(fn->index(fc)), c), 
							      	       criteria_result(fn, cw(fn->index(fc)), c)))));
				int fc_order = face_order(fc);
				int fn_order = face_order(fn);
				if(fc_order > max_order() || fn_order > max_order() || after_value >= initial_value){
					flip(fc, fc->index(fn));
					if(initial_value > max_face) max_face = initial_value;
				}
				else{ // Then, edge was optmized.
					if(after_value < max_face)
						max_face = after_value;
					queue.push(std::make_pair(fc, ccw(fc->index(fn))));
					queue.push(std::make_pair(fc, cw(fc->index(fn))));
					queue.push(std::make_pair(fn, ccw(fn->index(fc))));
					queue.push(std::make_pair(fn, cw(fn->index(fc))));
					flips_count++;
					std::cout << "\r" << flips_count << std::flush;

					if(flips_count >= 10000 && flips_count <= 10005){
						//std::cout << "\n\n";
						//print_face(fc);
						//std::cout << "\n";
						//print_face(fn);
						//std::cout << "\n";
						//print_face(fc->neighbor(ccw(fc->index(fn))));
						//std::cout << "\n";
						//print_face(fc->neighbor(cw(fc->index(fn))));
						//std::cout << "\n";
						//print_face(fn->neighbor(ccw(fn->index(fc))));
						//std::cout << "\n";
						//print_face(fn->neighbor(cw(fn->index(fc))));
						//std::cout << "Initial value = " << initial_value << "\n";
						//std::cout << "After value = " << after_value << "\n";
						//std::cout << "\n------------------------\n";
						//std::cout << "\n\n";
					}
				}
			}
		}
		if(max_face > max)
			max = max_face;
	}
	

	std::cout << "Number of flips = " << flips_count << "\n";
	double min = global_min_criteria(c);
	double max_result = global_max_criteria(c);
	double average_result = global_average_criteria(c);
	std::cout << "Min ABN = " << min << "\n";
	std::cout << "Max ABN = " << max_result << "\n";
	std::cout << "Average ABN = " << average_result << "\n";
	return max;
}

void HODT::print_face(Face_iterator fc)
{
	std::cout << std::endl;
	if(fc->vertex(0) == infinite_vertex())
		std::cout << "v0: infinite vertex.\n";
	else
		std::cout << "v0: x = " << fc->vertex(0)->point().get_x() << " y = " << fc->vertex(0)->point().get_y() << "\n";
	if(fc->vertex(1) == infinite_vertex())
		std::cout << "v1: infinite vertex.\n";
	else
		std::cout << "v1: x = " << fc->vertex(1)->point().get_x() << " y = " << fc->vertex(1)->point().get_y() << "\n";
	if(fc->vertex(2) == infinite_vertex())
		std::cout << "v2: infinite vertex.\n";
	else
		std::cout << "v2: x = " << fc->vertex(2)->point().get_x() << " y = " << fc->vertex(2)->point().get_y() << "\n";
}
