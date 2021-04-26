#include <cmath>

#include "flat.hpp"
#include "point.hpp"

#include <iostream>

flat::flat(cloud *data_, double bandwidth_) : radial(data_, bandwidth_) {}

double flat::volume() { return pow(M_PI, (double)point::get_dim() / 2) / tgamma((double)point::get_dim() / 2 + 1); }

double flat::profile(double t) { return (t <= 1) ? 1 : 0; }