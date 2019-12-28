#include <iostream>
#include <vector>
#include "SimpleKalman.hpp"

int main() {
	SimpleKalman sk = SimpleKalman(1500, 750, 50, 50);
	int messungen[5] = { 1550,380,1500,1500,1560 };
	int bewegungen[5] = { 20,20,20,20,20 };
	std::vector<int> testVector;

	for (int i = 0; i < 5; i++) {
		sk.predict(sk.getMean(), sk.getVariance(), bewegungen[i], sk.getStdDeviMove());
		std::cout << "Predict: Mean: " << sk.getMean() << ", Variance: " << sk.getVariance() << std::endl;
		testVector.push_back(sk.getMean());
		sk.update(sk.getMean(), sk.getVariance(), messungen[i], sk.getStdDeviMeasure());
		std::cout << "Update: Mean: " << sk.getMean() << ", Variance: " << sk.getVariance() << std::endl;
		testVector.push_back(sk.getMean());
	}

	int testValues = 0;

	for (int j = 0; j < testVector.size(); j++) {
		testValues += testVector[j];
	}

	int testMean = testValues / testVector.size();
	std::cout << testMean << std::endl;

	return 0;
}