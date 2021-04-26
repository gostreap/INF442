#include <cmath>

#include "cloud.hpp"
#include "point.hpp"
#include "radial.hpp"

radial::radial(cloud *data_, double bandwidth_) : kernel(data_) { this->bandwidth = bandwidth_; }

double radial::density(point &p) {

	double sum = 0;
	for (int i = 0; i < data->get_n(); i++) {
		sum += profile(pow(p.dist(data->get_point(i)),2) / pow(bandwidth, 2));
	}

	double dens = 1 / (volume() * data->get_n() * pow(bandwidth, point::get_dim())) * sum;
	return dens;
}
