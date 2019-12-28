#include "SimpleKalman.hpp"

SimpleKalman::SimpleKalman(int mean, int variance, int stdDeviMeasure, int stdDeviMove) {
	setMean(mean);
	setVariance(variance);
	setStdDeviMeasure(stdDeviMeasure);
	setStdDeviMove(stdDeviMove);
}

void SimpleKalman::update(int mean1, int variance1, int mean2, int variance2) {
	int updatedMean = SimpleKalman::updateMean(mean1, variance1, mean2, variance2);
	int updatedVariance = SimpleKalman::updateVariance(variance1, variance2);
	this->setMean(updatedMean);
	this->setVariance(updatedVariance);
}

void SimpleKalman::predict(int mean1, int variance1, int mean2, int variance2) {
	int predictedMean = SimpleKalman::predictMean(mean1, mean2);
	int predictedVariance = SimpleKalman::predictVariance(variance1, variance2);
	this->setMean(predictedMean);
	this->setVariance(predictedVariance);
}

int SimpleKalman::getMean() {
	return this->mean;
}

int SimpleKalman::getVariance() {
	return this->variance;
}

int SimpleKalman::getStdDeviMeasure() {
	return this->stdDeviMeasure;
}

int SimpleKalman::getStdDeviMove() {
	return this->stdDeviMove;
}

int SimpleKalman::updateMean(int mean1, int variance1, int mean2, int variance2) {
	float updatedMean = ((float)variance2 * (float)mean1 + (float)variance1 * (float)mean2) / ((float)variance1 + (float)variance2);
	return (int)updatedMean;
}

int SimpleKalman::updateVariance(int variance1, int variance2) {
	float updatedVariance = 1/ (1/(float)variance1 + 1/(float)variance2);
	return (int)updatedVariance;
}

int SimpleKalman::predictMean(int mean1, int mean2) {
	return mean1 + mean2;
}

int SimpleKalman::predictVariance(int variance1, int variance2) {
	return variance1 + variance2;
}

void SimpleKalman::setMean(int mean) {
	// possible check for legitimacy of value
	this->mean = mean;
}

void SimpleKalman::setVariance(int variance) {
	// possible check for legitimacy of value
	this->variance = variance;
}

void SimpleKalman::setStdDeviMeasure(int stdDeviMeasure) {
	// possible check for legitimacy of value
	this->stdDeviMeasure = stdDeviMeasure;
}
void SimpleKalman::setStdDeviMove(int stdDeviMove) {
	// possible check for legitimacy of value
	this->stdDeviMove = stdDeviMove;
}