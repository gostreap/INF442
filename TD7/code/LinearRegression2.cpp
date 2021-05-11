#include "LinearRegression.hpp"
#include "Dataset.hpp"
#include "Regression.hpp"
#include<iostream>
#include<cassert>


LinearRegression::LinearRegression( Dataset* dataset, int col_regr ) 
: Regression(dataset, col_regr) {
	SetCoefficients();
}

LinearRegression::~LinearRegression() {
	m_beta->resize(0);
	delete m_beta;
}

void LinearRegression::SetCoefficients() {
	// TODO Exercise 1.1
	Eigen::MatrixXd X(m_dataset ->GetNbrSamples(),m_dataset->GetDim());
	Eigen::VectorXd y(m_dataset ->GetNbrSamples(),1);
	Eigen::VectorXd beta(m_dataset-> GetDim(),1);
	for(int i =0 ; i< m_dataset ->GetNbrSamples(); i++){
		X(i,0)=1;
		y(i)=m_dataset->GetInstance(i)[m_col_regr];
		for(int j=1; j< m_dataset->GetDim();j++){
			if(j-1 < m_col_regr){
				X(i,j)=m_dataset->GetInstance(i)[j-1];
			}
			else{
				X(i,j)=m_dataset->GetInstance(i)[j];
			}
		}
	}
	std::cerr << X << std::endl;
	beta =(X.transpose() * X).ldlt().solve(X.transpose() * y);
	m_beta= new Eigen::VectorXd(m_dataset-> GetDim(),1);
	*m_beta = beta;
}

const Eigen::VectorXd* LinearRegression::GetCoefficients() const {
	if (!m_beta) {
		std::cout<<"Coefficients have not been allocated."<<std::endl;
		return NULL;
	}
	return m_beta;
}

void LinearRegression::ShowCoefficients() const {
	if (!m_beta) {
		std::cout<<"Coefficients have not been allocated."<<std::endl;
		return;
	}
	
	if (m_beta->size() != m_dataset->GetDim()) {  // ( beta_0 beta_1 ... beta_{d} )
		std::cout<< "Warning, unexpected size of coefficients vector: " << m_beta->size() << std::endl;
	}
	
	std::cout<< "beta = (";
	for (int i=0; i<m_beta->size(); i++) {
		std::cout<< " " << (*m_beta)[i];
	}
	std::cout << " )"<<std::endl;
}

void LinearRegression::SumsOfSquares( Dataset* dataset, double& ess, double& rss, double& tss ) const {
	assert(dataset->GetDim()==dataset->GetDim());
	// TODO Exercise 1.3
	Eigen::MatrixXd X(dataset ->GetNbrSamples(),m_dataset->GetDim());
	Eigen::VectorXd y(dataset ->GetNbrSamples(),1);
	Eigen::VectorXd beta =*m_beta;
	for(int i =0 ; i< dataset ->GetNbrSamples(); i++){
		X(i,0)=1;
		y(i)=m_dataset->GetInstance(i)[m_col_regr];
		for(int j=1; j< dataset->GetDim();j++){
			if(j-1 < m_col_regr){
				X(i,j)=dataset->GetInstance(i)[j-1];
			}
			else{
				X(i,j)=dataset->GetInstance(i)[j];
			}
		}
	}
	Eigen::VectorXd y_hat=X*beta;
	double y_barre = 0.0;
	for(int i =0 ; i < dataset->GetNbrSamples(); i++){
		y_barre+= dataset->GetInstance(i)[m_col_regr];
	}
	y_barre/=(double)dataset->GetNbrSamples();
	tss=0.0;
	ess=0.0;
	rss=0.0;
	for(int i =0 ;  i < dataset->GetNbrSamples(); i++){
		tss+=(dataset->GetInstance(i)[m_col_regr]-y_barre)*(dataset->GetInstance(i)[m_col_regr]-y_barre);
		ess+=(y_hat[i]-y_barre)*(y_hat[i]-y_barre);
		rss+=(y_hat[i]-dataset->GetInstance(i)[m_col_regr])*(y_hat[i]-dataset->GetInstance(i)[m_col_regr]);
	}


}

double LinearRegression::Estimate( const Eigen::VectorXd & x ) const {
	// TODO Exercise 1.2
	Eigen::VectorXd v(x.size()+1,1);
	v << 1,x;
	return (*m_beta).transpose()*v;

}
