#pragma once

#include "cloud.hpp"
#include "kernel.hpp"
#include "point.hpp"

class radial : public kernel {

protected:
	double bandwidth;

public:
	radial(cloud *data_, double bandwidth_);

	virtual double volume() = 0;
	virtual double profile(double t) = 0;

    double density(point &p);
};