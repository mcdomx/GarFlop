//
//  distance_calc.c
//  GarFlop
//
//  Created by Mark on 6/22/18.
//  Copyright © 2018 Mark McDonald. All rights reserved.
// 	ref: https://stackoverflow.com/questions/33111529/calculating-distance-between-two-points-using-latitude-longitude-and-altitude-e#33111741

#include <math.h>
#include "distance_calc.h"

double calc_distance (double lat1, double lon1, double alt1, double lat2, double lon2, double alt2) {

	lat1 = lat1 * (M_PI / 180);
	lon1 = lon1 *(M_PI/180);

	lat2 = lat2 * (M_PI / 180);
	lon2 = lon2 * (M_PI / 180);

	const double r = 6376.5 * 1000;	//redius of earth in meters

	double x1 = r * sin(lon1) * cos(lat1);
	double y1 = r * sin(lon1) * sin(lat1);
	double z1 = r * cos(lon1);

	double x2 = r * sin(lon2) * cos(lat2);
	double y2 = r * sin(lon2) * sin(lat2);
	double z2 = r * cos(lon2);

	double dist = sqrt( (x2 - x1) * (x2 - x1) + (y2 - y1) *
						(y2 - y1) + (z2 - z1) * (z2 - z1)   );

	return dist;
	
}
