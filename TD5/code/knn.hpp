#pragma once

#include "point.hpp"
#include "cloud.hpp"
#include "kernel.hpp"

class knn : public kernel {

protected:
	int k;
    double V;

public:
	knn(cloud *data_, int k_, double V_);

    double density(point &p);
};