#ifndef _PLATE_FILTER_
#define _PLATE_FILTER_
#include <vector>

class PlateFilter {
public:
	PlateFilter();
	std::vector<double> process(std::vector<int> rawSensorData);
private:
	const double PLATE_LENGTH_CM_X = 28;
	const double PLATE_LENGTH_CM_Y = 20.4;
	const int MAX_SENSOR_VALUE = 1500;
	const int MIN_SENSOR_VALUE = 0;
	const int SENSOR_VALUE_LENGHT = MAX_SENSOR_VALUE - MIN_SENSOR_VALUE;
	// each tilesize advances x or y pos by 1 cm
	const int TILESIZE_X = (SENSOR_VALUE_LENGHT) / PLATE_LENGTH_CM_X;
	const int TILESIZE_Y = (SENSOR_VALUE_LENGHT) / PLATE_LENGTH_CM_Y;

	std::vector<int> filter(std::vector<int> rawSensorData);
	std::vector<double> convertValuesToCm(std::vector<int> filteredSensorData);
};

#endif //_PLATE_FILTER_
