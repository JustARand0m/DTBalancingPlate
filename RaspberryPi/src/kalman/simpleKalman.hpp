#pragma once

class SimpleKalman {
public:
	SimpleKalman(int mean, int variance, int stdDeviMeasure, int stdDeviMove);
	void update(int mean1, int variance1, int mean2, int variance2);
	void predict(int mean1, int variance1, int mean2, int variance2);
	int getMean();
	int getVariance();
	int getStdDeviMeasure();
	int getStdDeviMove();
private:
	int mean;
	int variance;
	int stdDeviMeasure;
	int stdDeviMove;
	static int updateMean(int mean1, int variance1, int mean2, int variance2);
	static int updateVariance(int variance1, int variance2);
	static int predictMean(int mean1, int mean2);
	static int predictVariance(int variance1, int variance2);
	void setMean(int mean);
	void setVariance(int variance);
	void setStdDeviMeasure(int stdDeviMeasure);
	void setStdDeviMove(int stdDeviMove);
};
