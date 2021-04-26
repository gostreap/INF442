#include <cmath>
#include <iostream>

#include "cloud.hpp"
#include "gaussian.hpp"
#include "point.hpp"

gaussian::gaussian(cloud *data_, double bandwidth_) : radial(data_, bandwidth_) {}

double gaussian::volume() { return pow(2 * M_PI, (double)point::get_dim() / 2); }

double gaussian::profile(double t) { return exp(-t / 2); }

void gaussian::guess_bandwidth() { bandwidth = 1.06 * data->standard_deviation() / pow(data->get_n(), 1.0 / 5.0); }
