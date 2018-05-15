#include "HODT.h"

Vertex_iterator HODT::insert(Pinfo& p)
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

Vertex_iterator HODT::insert_dimension_2(Pinfo& p)
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

Vertex_iterator HODT::insert_on_edge_2(Face_iterator fc, Pinfo& p, Vertex_location lc, int li)
{
	Vertex_iterator v = vertices().insert(Vertex(p));

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

Vertex_iterator HODT::insert_outside_convex_hull_2(Face_iterator fc, Pinfo& p, Vertex_location lc, int li)
{
	std::list<Face_iterator> list;
	bool done = false;
	Vertex_iterator v = vertices().insert(Vertex(p));
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

Vertex_iterator HODT::insert_in_face(Face_iterator fc, Pinfo& p, Vertex_location lc, int li)
{
	Vertex_iterator v = vertices().insert(Vertex(p));

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
}

int HODT::face_order(Face_iterator fc)
{
	if(is_infinite(fc))
		return 0;

	typedef std::pair<Face_iterator, int> Edge;
	int order = 0;

	std::stack<Edge> stack;

	// Adding edges to the stack. Each edge will check if the opposite vertex on the neighbor
	// (mirror_index(fc, i)) lies inside the fc's circumcircle.
	if(!is_infinite(fc->neighbor(0)))
		stack.push(std::make_pair(fc, 0));
	if(!is_infinite(fc->neighbor(1)))
		stack.push(std::make_pair(fc, 1));
	if(!is_infinite(fc->neighbor(2)))
		stack.push(std::make_pair(fc, 2));

	if(!stack.empty())
		std::cout << "Stack is not empty.\n";
	else
		std::cout << "oh, hey! Stack Empty.\n";

	while(!stack.empty()){
		Edge e = stack.top();
		stack.pop();

		Face_iterator temp_face = std::get<0>(e);
		int temp_i = std::get<1>(e);
		Face_iterator ff = temp_face->neighbor(temp_i);
		int i = mirror_index(temp_face, temp_i);

		// If positive, order of the face increases.
		if(incircle_test(fc->vertex(0), fc->vertex(1), fc->vertex(2), ff->vertex(i)) > 0){
			order++;

			// Adding to the stack.
			if(!is_infinite(ff->neighbor(ccw(i))))
				stack.push(std::make_pair(ff, ccw(i)));
			if(!is_infinite(ff->neighbor(cw(i))))
				stack.push(std::make_pair(ff, cw(i)));
		}
	}

	return order;
}

int HODT::brute_order()
{
	int order = 0;
	for(Face_iterator it = faces().begin(); it != faces().end(); ++it){
		if(!is_infinite(it)){
			int f_order = 0;
			for(Vertex_iterator vi = vertices().begin(); vi != vertices().end(); ++vi){
				if(vi != infinite_vertex()){
					if(incircle_test(it->vertex(0), it->vertex(1), it->vertex(2), vi) > 0)
						f_order++;
				}
			}
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
		int f_order = face_order(it);
		if(f_order > t_order)
			t_order = f_order;
	}

	return t_order;
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
