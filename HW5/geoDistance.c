/**
	Homework 5
	geoDistance
	Author: Douglas Skrypa
	Date: 2014.04.24
*/

#include <math.h>
#include "geoDistance.h"

#define EARTH_RADIUS 3959
#define KM_PER_MILE 1.609344

/**
	Compute the distance between two points on Earth
	@param lat1 latitude of the first point (in degrees)
	@param long1 longitude of the first point (in degrees)
	@param lat2 latitude of the second point (in degrees)
	@param long2 longitude of the second point (in degrees)
	@return the distance between points 1 and 2 in miles
*/
double geodistance (double lat1, double lon1, double lat2, double lon2){
	double latr1 = degToRad(lat1);
	double latr2 = degToRad(lat2);
	double lonr1 = degToRad(lon1);
	double lonr2 = degToRad(lon2);
	
	double alpha = pow(sin((latr2 - latr1)/2), 2) + cos(latr1) * cos(latr2) * pow(sin((lonr2 - lonr1)/2), 2);
	double distance = 2 * EARTH_RADIUS * atan2(sqrt(alpha), sqrt(1 - alpha));
	return distance;
}

/**
	Convert the given angle from degrees to radians
	@param degrees an angle, measured in degrees
	@return the given angle, measured in radians
*/
double degToRad(double degrees){
	return degrees * M_PI / 180.0;
}

/**
	Convert the given angle from radians to degrees
	@param radians an angle, measured in radians
	@return the given angle, measured in degrees
*/
double radToDeg(double radians){
	return radians * 180.0 / M_PI;
}

/**
	Convert miles to kilometers
	@param miles a distance measured in miles
	@return the given distance measured in kilometers
*/
double milesToKm(double miles){
	return miles * KM_PER_MILE;
}

/**
	Convert kilometers to miles
	@param km a distance measured in kilometers
	@return the given distance measured in miles
*/
double kmToMiles(double km){
	return km / KM_PER_MILE;
}
