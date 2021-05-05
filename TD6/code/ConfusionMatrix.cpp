#include "ConfusionMatrix.hpp"
#include <iostream>

using namespace std;

ConfusionMatrix::ConfusionMatrix() {
	// TODO Exercise 2.1
	// Populate 2x2 matrix with 0s
	m_confusion_matrix[0][0] = 0;
	m_confusion_matrix[0][1] = 0;
	m_confusion_matrix[1][0] = 0;
	m_confusion_matrix[1][1] = 0;
}

ConfusionMatrix::~ConfusionMatrix() {
	// Attribute m_confusion_matrix is deleted automatically
}

void ConfusionMatrix::AddPrediction(int true_label, int predicted_label) {
	if (true_label == 1) {
		if (true_label == predicted_label) { // true positive
			m_confusion_matrix[1][1]++;
		} else { // false negative
			m_confusion_matrix[1][0]++;
		}
	} else {
		if (true_label == predicted_label) { // true negative
			m_confusion_matrix[0][0]++;
		} else { // false positive
			m_confusion_matrix[0][1]++;
		}
	}
}

void ConfusionMatrix::PrintEvaluation() const {
	// Prints the confusion matrix
	cout << "\t\tPredicted\n";
	cout << "\t\t0\t1\n";
	cout << "Actual\t0\t" << GetTN() << "\t" << GetFP() << endl;
	cout << "\t1\t" << GetFN() << "\t" << GetTP() << endl << endl;
	// Prints the estimators
	cout << "Error rate\t\t" << error_rate() << endl;
	cout << "False alarm rate\t" << false_alarm_rate() << endl;
	cout << "Detection rate\t\t" << detection_rate() << endl;
	cout << "F-score\t\t\t" << f_score() << endl;
	cout << "Precision\t\t" << precision() << endl;
}

int ConfusionMatrix::GetTP() const { return m_confusion_matrix[1][1]; }

int ConfusionMatrix::GetTN() const { return m_confusion_matrix[0][0]; }

int ConfusionMatrix::GetFP() const { return m_confusion_matrix[0][1]; }

int ConfusionMatrix::GetFN() const { return m_confusion_matrix[1][0]; }

double ConfusionMatrix::f_score() const {
	double precision = this->precision();
	double recall = this->detection_rate();
	return 2 * precision * recall / (precision + recall);
}

double ConfusionMatrix::precision() const {
	return (double)GetTP() / (GetTP() + GetFP());
}

double ConfusionMatrix::error_rate() const {
	return ((double)GetFP() + GetFN()) /
		   (GetTP() + GetFP() + GetTN() + GetFN());
}

double ConfusionMatrix::detection_rate() const {
	return (double)GetTP() / (GetTP() + GetFN());
}

double ConfusionMatrix::false_alarm_rate() const {
	return (double)GetFP() / (GetFP() + GetTN());
}
