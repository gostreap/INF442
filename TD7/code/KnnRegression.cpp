
#include "KnnRegression.hpp"
#include <ANN/ANN.h>
#include <iostream>

KnnRegression::KnnRegression(int k, Dataset *dataset, int col_regr) : Regression(dataset, col_regr) {
	m_k = k;
	// TODO Exercise 2.1
	m_dataPts = annAllocPts(dataset->GetNbrSamples(), dataset->GetDim() - 1);

	for (int i = 0; i < dataset->GetNbrSamples(); i++) {
		int j = 0;
		for (double coord : dataset->GetInstance(i)) {
			if (j != col_regr) {
				m_dataPts[i][((j < col_regr) ? j : j - 1)] = coord;
			}
			j++;
		}
	}

	m_kdTree = new ANNkd_tree(m_dataPts, dataset->GetNbrSamples(), dataset->GetDim() - 1);
}

KnnRegression::~KnnRegression() {
	// TODO Exercise 2.1
	annDeallocPts(m_dataPts);
	delete m_kdTree;
	annClose();
}

double KnnRegression::Estimate(const Eigen::VectorXd &x) const {
	assert(x.size() == m_dataset->GetDim() - 1);
	// TODO Exercise 2.2
	ANNidxArray nnIdx = new ANNidx[m_k];
	ANNdistArray dists = new ANNdist[m_k];
	ANNpoint ax = new ANNcoord[x.size()];
	for(int i = 0; i < x.size(); i++) {
		ax[i] = x(i);
	}

	m_kdTree->annkSearch(ax, m_k, nnIdx, dists);

    double p = 0;
    for(int i = 0; i < m_k; i++) {
        double y = m_dataset->GetInstance(nnIdx[i])[m_col_regr];
        p += y / m_k;
    }

	delete[] nnIdx;
	delete[] dists;
	delete[] ax;

	return p;
}

int KnnRegression::GetK() const { return m_k; }

ANNkd_tree *KnnRegression::GetKdTree() const { return m_kdTree; }
