#include "LinearRegression.hpp"
#include "Dataset.hpp"
#include "Regression.hpp"
#include <cassert>
#include <iostream>
#include <cmath>

LinearRegression::LinearRegression(Dataset *dataset, int col_regr) : Regression(dataset, col_regr) {
	SetCoefficients();
}

LinearRegression::~LinearRegression() {
	m_beta->resize(0);
	delete m_beta;
}

void LinearRegression::SetCoefficients() {
	Eigen::MatrixXd X(m_dataset->GetNbrSamples(), m_dataset->GetDim());
	Eigen::VectorXd y(m_dataset->GetNbrSamples());
	m_beta = new Eigen::VectorXd(m_dataset->GetDim());

	for (int i = 0; i < m_dataset->GetNbrSamples(); i++) {
		std::vector<double> instance = m_dataset->GetInstance(i);
		X(i, 0) = 1;
		y(i) = instance[m_col_regr];
		for (int j = 1; j < m_dataset->GetDim(); j++) {
			if (j - 1 < m_col_regr) {
				X(i, j) = instance[j - 1];
			} else {
				X(i, j) = instance[j];
			}
		}
	}

	*m_beta = (X.transpose() * X).ldlt().solve(X.transpose() * y);
}

const Eigen::VectorXd *LinearRegression::GetCoefficients() const {
	if (!m_beta) {
		std::cout << "Coefficients have not been allocated." << std::endl;
		return NULL;
	}
	return m_beta;
}

void LinearRegression::ShowCoefficients() const {
	if (!m_beta) {
		std::cout << "Coefficients have not been allocated." << std::endl;
		return;
	}

	if (m_beta->size() != m_dataset->GetDim()) { // ( beta_0 beta_1 ... beta_{d} )
		std::cout << "Warning, unexpected size of coefficients vector: " << m_beta->size() << std::endl;
	}

	std::cout << "beta = (";
	for (int i = 0; i < m_beta->size(); i++) {
		std::cout << " " << (*m_beta)[i];
	}
	std::cout << " )" << std::endl;
}

void LinearRegression::SumsOfSquares(Dataset *dataset, double &ess, double &rss, double &tss) const {
	assert(dataset->GetDim() == m_dataset->GetDim());
	// TODO Exercise 1.3

	Eigen::MatrixXd X(dataset->GetNbrSamples(), dataset->GetDim());
	Eigen::VectorXd y(dataset->GetNbrSamples());
	for (int i = 0; i < dataset->GetNbrSamples(); i++) {
		std::vector<double> instance = dataset->GetInstance(i);
		X(i, 0) = 1;
		y(i) = instance[m_col_regr];
		for (int j = 1; j < dataset->GetDim(); j++) {
			if (j - 1 < m_col_regr) {
				X(i, j) = instance[j - 1];
			} else {
				X(i, j) = instance[j];
			}
		}
	}

	Eigen::VectorXd predictions = X * (*m_beta);
	double empirical_mean = 0;

	for (int i = 0; i < dataset->GetNbrSamples(); i++) {
		empirical_mean += y[i];
	}
	empirical_mean /= (double)(dataset->GetNbrSamples());

	ess = rss = tss = 0;
	for (int i = 0; i < dataset->GetNbrSamples(); i++) {
		tss += std::pow(y[i] - empirical_mean, 2);
		ess += std::pow(predictions[i] - empirical_mean, 2);
		rss += std::pow(predictions[i] - y[i], 2);
	}
}

double LinearRegression::Estimate(const Eigen::VectorXd &x) const {
	// TODO Exercise 1.2
	Eigen::VectorXd v((*m_beta).size(), 1);
	v << 1, x;
	return (*m_beta).transpose() * v;
}
