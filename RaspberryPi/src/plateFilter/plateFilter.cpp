#include "plateFilter.h"

PlateFilter::PlateFilter() {
}

std::vector<double> PlateFilter::process(std::vector<int> rawSensorData) {
	return convertValuesToCm(filter(rawSensorData));
}

std::vector<int> PlateFilter::filter(std::vector<int> rawSensorData)
{
	return std::vector<int>();
}

/**
* Helper function that converts the filtered sensor data in x and y value in cm, stored in a vector.
*
* @param filteredSensorData already filtered sensor data
*/
std::vector<double> PlateFilter::convertValuesToCm(std::vector<int> filteredSensorData)
{
	std::vector<double> convertedValues;
	
	// x value 
	filteredSensorData[0] -= MIN_SENSOR_VALUE;
	convertedValues.push_back(filteredSensorData[0] / TILESIZE_X);

	// y value 
	filteredSensorData[1] -= MIN_SENSOR_VALUE;
	convertedValues.push_back(filteredSensorData[1] / TILESIZE_Y);

	return convertedValues;
}
