#include "knn.hpp"
#include "cloud.hpp"
#include "kernel.hpp"
#include "point.hpp"

knn::knn(cloud *data_, int k_, double V_) : kernel(data_) {
	this->k = k_;
	this->V = V_;
}

double knn::density(point &p) { return (double)k / (2.0 * data->get_n() * data->knn_dist(p, k) * V); }
