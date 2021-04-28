#include "graph.hpp" // This is the header for the class implemented here

#include "cloud.hpp" // Used in the constructor
#include "edge.hpp"	 // Used in almost all methods

#include <algorithm> // This provides the sort() method for the constructor
#include <cassert>
#include <iostream>

/* graph -- method implementations */

graph::graph(cloud &_c) {
	n = _c.get_n();
	size = n * (n - 1) / 2;

	node_names = new std::string[n];
	edges = new edge *[size];

    long c = 0;
	for (long i = 0; i < n; i++) {
		node_names[i] = _c.get_point(i).name;
		for (long j = i; j < n; j++) {
			if (i != j) {
				edges[c++] = new edge(i, j, _c.get_point(i).dist(_c.get_point(j)));
			}
		}
	}

    std::sort(edges, edges+size, edge::compare);

	iterator_pos = 0;
}

graph::graph(long _n, std::string _node_names[], double **dist_matrix) {
	n = _n;
	size = n * (n - 1) / 2;

	node_names = new std::string[n];
	edges = new edge *[size];

    long c =0;
	for (long i = 0; i < n; i++) {
		node_names[i] = _node_names[i];
		for (long j = i; j < n; j++) {
			if (i != j) {
				edges[c++] = new edge(i, j, dist_matrix[i][j]);
			}
		}
	}

    std::sort(edges, edges+size, edge::compare);

	iterator_pos = 0;
}

graph::~graph() {
	for (long i = 0; i < size; i++) {
		delete edges[i];
	}

	delete[] edges;
	delete[] node_names;
}

long graph::get_num_edges() { return size; }

std::string &graph::get_name(int i) {
	assert(i >= 0 && i < n);
	return node_names[i];
}

long graph::get_num_nodes() { return n; }

void graph::start_iteration() { iterator_pos = 0; }

edge *graph::get_next() {
	if (iterator_pos == size) {
		return NULL;
	} else {
		return edges[iterator_pos++];
	}
}

graph *graph::load_matrix(std::ifstream &is) {
	assert(is.is_open());

	int n;
	is >> n;
	std::string node_names[n];
	for (int i = 0; i < n; ++i) {
		is >> node_names[i];
	}

	std::cout << "Names read" << std::endl;
	double **dist_matrix = new double *[n];
	for (int i = 0; i < n; ++i) {
		dist_matrix[i] = new double[n];
		for (int j = 0; j < n; ++j) {
			is >> dist_matrix[i][j];
		}
	}

	graph *g = new graph(n, node_names, dist_matrix);
	for (int i = 0; i < n; ++i) {
		delete[] dist_matrix[i];
	}
	delete[] dist_matrix;

	return g;
}
