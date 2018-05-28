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
		if(vi != infinite_vertex()){
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
		int f_order = face_order(it);
		if(f_order > order)
			order = f_order;
	}

	return order;
}

int HODT::order()
{
	int t_order = 0;

	for(Face_iterator it = faces().begin(); it != faces().end(); ++it){
		int f_order = face_order(it);
		if(f_order > t_order)
			t_order = f_order;
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

// Weighted angle between normals, where the edge length is the weight.
double HODT::wabn(Face_iterator fc, int i)
{
	return wabn(fc, fc->neighbor(i));
}

double HODT::wabn(Face_iterator fc, Face_iterator ff)
{
	return edge_length(fc, fc->index(ff)) * abn(fc, ff);
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
				if(!is_infinite(it->neighbor(i))){
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
				if(!is_infinite(it->neighbor(i))){
					double abn_result = criteria_result(it, it->neighbor(i), c);
					if(abn_result > max_face)
						max_face = abn_result;
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

double HODT::optimize(Optimization_criteria& c)
{
	int flips_count = 0;
	double max = 0;
	std::queue<std::pair<Face_iterator, int>> queue;

	for(Face_iterator it = faces().begin(); it != faces().end(); ++it){
		if(!is_infinite(it)){
			for(int i = 0; i < 3; ++i){
				if(!is_infinite(it->neighbor(i))){
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
		if(!is_infinite(fc->neighbor(i))){
			if(convex_polygon(fc, fc->neighbor(i))){
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
				if(fc_order > max_order() || fn_order > max_order() || after_value > initial_value){
					flip(fc, fc->index(fn));
					if(initial_value > max_face) max_face = initial_value;
				}
				else{ // Then, edge was optmized.
					if(after_value < max_face)
						max_face = after_value;
					int j = fc->index(fn);
					queue.push(std::make_pair(fc, ccw(fc->index(fn))));
					queue.push(std::make_pair(fc, cw(fc->index(fn))));
					queue.push(std::make_pair(fn, ccw(j)));
					queue.push(std::make_pair(fc, cw(j)));
					flips_count++;
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
