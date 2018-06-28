//
//  distance_calc.c
//  GarFlop
//
//  Created by Mark on 6/22/18.
//  Copyright © 2018 Mark McDonald. All rights reserved.
// 	ref: https://stackoverflow.com/questions/33111529/calculating-distance-between-two-points-using-latitude-longitude-and-altitude-e#33111741

#include <math.h>
#include <stdlib.h>
#include "distance_calc.h"

double calc_distance (double lat1, double lon1, double alt1,
					  double lat2, double lon2, double alt2);
double calc_distance2 (double lat1, double lon1, double alt1,
					   double lat2, double lon2, double alt2);
double calc_distance3 (double lat1, double lon1, double alt1,
					   double lat2, double lon2, double alt2);



double dist_between_points (struct GPS_entry* pt1, struct GPS_entry* pt2) {
	
	return calc_distance3(*pt1->lat, *pt1->lon, *pt1->alt, *pt2->lat, *pt2->lon, *pt2->alt);
	
} // end dist_between_points()

//returns distance in meters between 2 points
double calc_distance (double lat1, double lon1, double alt1, double lat2, double lon2, double alt2) {

	lat1 = lat1 * (M_PI / 180);
	lon1 = lon1 * (M_PI / 180);

	lat2 = lat2 * (M_PI / 180);
	lon2 = lon2 * (M_PI / 180);

	const double r = 6376.5 * 1000;	//radius of earth in meters

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

//returns distance in meters between 2 points
double calc_distance2 (double lat1, double lon1, double alt1, double lat2, double lon2, double alt2) {
	
	lat1 = lat1 * (M_PI / 180);
	lon1 = lon1 * (M_PI / 180);
	
	lat2 = lat2 * (M_PI / 180);
	lon2 = lon2 * (M_PI / 180);
	
	//const double r = 6376.5 * 1000;	//radius of earth in meters
	
	double x1 = alt1 * cos(lat1) * sin(lon1);
	double y1 = alt1 * sin(lat1);
	double z1 = alt1 * cos(lat1) * cos(lon1);
	
	double x2 = alt2 * cos(lat2) * sin(lon2);
	double y2 = alt2 * sin(lat2);
	double z2 = alt2 * cos(lat2) * cos(lon2);
	
	double dist = sqrt( (x2 - x1) * (x2 - x1) + (y2 - y1) *
					   (y2 - y1) + (z2 - z1) * (z2 - z1)   );
	
	return dist;
	
}

/*
 ###===================================================================================================
 ### get distance in metres between 2 points:
 ### Vincenty Formula http://www.movable-type.co.uk/scripts/latlong-vincenty.html
 SOURCE: https://github.com/janantala/GPS-distance/blob/master/java/Distance.java
 */

double calc_distance3 (double lat1, double lon1, double alt1, double lat2, double lon2, double alt2) {
	
		double a = 6378137, b = 6356752.314245, f = 1 / 298.257223563;
		double L = (lon2 - lon1) * (M_PI / 180);
		double U1 = atan( (1 - f) * tan(lat1 * (M_PI / 180)) );
		double U2 = atan( (1 - f) * tan(lat2 * (M_PI / 180)) );
		double sinU1 = sin(U1), cosU1 = cos(U1);
		double sinU2 = sin(U2), cosU2 = cos(U2);
		double cosSqAlpha;
		double sinSigma;
		double cos2SigmaM;
		double cosSigma;
		double sigma;
		
		double lambda = L, lambdaP, iterLimit = 100;
		do
		{
			double sinLambda = sin(lambda), cosLambda = cos(lambda);
			sinSigma = sqrt(	(cosU2 * sinLambda)
								 * (cosU2 * sinLambda)
								 + (cosU1 * sinU2 - sinU1 * cosU2 * cosLambda)
								 * (cosU1 * sinU2 - sinU1 * cosU2 * cosLambda)
								 );
			if (sinSigma == 0)
			{
				return 0;
			}
			
			cosSigma = sinU1 * sinU2 + cosU1 * cosU2 * cosLambda;
			sigma = atan2(sinSigma, cosSigma);
			double sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma;
			cosSqAlpha = 1 - sinAlpha * sinAlpha;
			cos2SigmaM = cosSigma - 2 * sinU1 * sinU2 / cosSqAlpha;
			
			double C = f / 16 * cosSqAlpha * (4 + f * (4 - 3 * cosSqAlpha));
			lambdaP = lambda;
			lambda = 	L + (1 - C) * f * sinAlpha
			* 	(sigma + C * sinSigma
				 * 	(cos2SigmaM + C * cosSigma
					 * 	(-1 + 2 * cos2SigmaM * cos2SigmaM)
					 )
				 );
			
		} while ( fabs(lambda - lambdaP) > 1e-12 && --iterLimit > 0);
		
		if (iterLimit == 0)
		{
			return 0;
		}
		
		double uSq = cosSqAlpha * (a * a - b * b) / (b * b);
		double A = 1 + uSq / 16384
		* (4096 + uSq * (-768 + uSq * (320 - 175 * uSq)));
		double B = uSq / 1024 * (256 + uSq * (-128 + uSq * (74 - 47 * uSq)));
		double deltaSigma =
		B * sinSigma
		* (cos2SigmaM + B / 4
		   * (cosSigma
			  * (-1 + 2 * cos2SigmaM * cos2SigmaM) - B / 6 * cos2SigmaM
			  * (-3 + 4 * sinSigma * sinSigma)
			  * (-3 + 4 * cos2SigmaM * cos2SigmaM)));
		
		double s = b * A * (sigma - deltaSigma);
		
		return s;
	}
