#include "KnnClassification.hpp"

#include <ANN/ANN.h>
#include <iostream>

KnnClassification::KnnClassification(int k, Dataset *dataset, int col_class) : Classification(dataset, col_class) {
	m_k = k;

	m_dataPts = annAllocPts(dataset->getNbrSamples(), dataset->getDim() - 1);

	for (int i = 0; i < dataset->getNbrSamples(); i++) {
		int j = 0;
		for (double coord : dataset->getInstance(i)) {
			if (j != col_class) {
				m_dataPts[i][((j < col_class)?j:j-1)] = coord;
			}
			j++;
		}
	}

	m_kdTree = new ANNkd_tree(m_dataPts, dataset->getNbrSamples(), dataset->getDim() - 1);
}

KnnClassification::~KnnClassification() {
	annDeallocPts(m_dataPts);
	delete m_kdTree;
	annClose();
}

int KnnClassification::Estimate(const ANNpoint &x, double threshold) {
	double eps = 0;
	ANNidxArray nnIdx = new ANNidx[m_k];
	ANNdistArray dists = new ANNdist[m_k];

	m_kdTree->annkSearch(x, m_k, nnIdx, dists, eps);

    double p = 0;
    for(int i = 0; i < m_k; i++) {
        double label = m_dataset->getInstance(nnIdx[i])[m_col_class];
        p += label / m_k;
    }

	delete[] nnIdx;
	delete[] dists;

    return p > threshold;
}

int KnnClassification::getK() { return m_k; }

ANNkd_tree *KnnClassification::getKdTree() { return m_kdTree; }
