#include "point.hpp"

#include <iostream>
#include <cmath>
#include <cassert> 

int point::d = 0; 

point::point() {
    coords = new double[d];

    for (int m = 0; m < d; m++)
        coords[m] = 0.0;
}

point::~point() {
    delete[] coords;
}

bool point::set_dim(int _d) {
    assert (_d > 0);
    if (d > 0)
        return false;

    d = _d;
    return true;
}

int point::get_dim() {
    return d;
}

void point::print() {
    for (int j = 0; j < d; j++)
        std::cout << coords[j] << ' ';
    std::cout << std::endl;
}

double point::squared_dist(point &q) {
    double sqd = 0.0;

    for (int m = 0; m < d; m++)
        sqd += (coords[m] - q.coords[m]) * (coords[m] - q.coords[m]);

    return sqd;
}