//
//  distance_calc.c
//  GarFlop
//
//  Created by Mark on 6/22/18.
//  Copyright © 2018 Mark McDonald. All rights reserved.
// 	ref: https://stackoverflow.com/questions/33111529/calculating-distance-between-two-points-using-latitude-longitude-and-altitude-e#33111741

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "distance_calc.h"

#define LINELEN		1024
#define TAGLEN		30

double calc_distance (double lat1, double lon1, double lat2, double lon2);

FILE* 	open_GPS_file(char* filename);
struct 	GPS_entry* malloc_point ( void );
void 	free_point ( struct GPS_entry* point );
void 	copy_point_data (struct GPS_entry* from_point, struct GPS_entry* to_point);
int 	get_next_point(FILE* fp, struct GPS_entry* next_point);


//accepts two GPS_entry points and passes relevant lat/lon data to calculation function
double dist_between_points (struct GPS_entry* pt1, struct GPS_entry* pt2) {
	
	return calc_distance(*pt1->lat, *pt1->lon, *pt2->lat, *pt2->lon);
	
} // end dist_between_points()


/*
 ===============================================================================
 get distance in metres between 2 points:
 Vincenty Formula http://www.movable-type.co.uk/scripts/latlong-vincenty.html
 SOURCE: https://github.com/janantala/GPS-distance
 */
double calc_distance (double lat1, double lon1, double lat2, double lon2) {
	
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

//process a file to calculate the distance between all GPS points in the file
void calc_file_distance (struct GPS_file* GPS_file) {
	
	FILE* fp;
	if ( (fp = open_GPS_file(GPS_file->filepath)) == NULL )
		return; //failed to open file - continue to next file
	
	//Malloc two points
	struct GPS_entry* point1 = malloc_point();
	struct GPS_entry* point2 = malloc_point();
	
	double ttl_climb = 0;
	double ttl_descent = 0;
	double ttl_distance = 0;
	double elevation_chg = 0;
	
	//get point 1
	//if lat/lon and elevation aren't part of point
	//consider the point corrupt and skip it
	if ( get_next_point(fp, point1) == 0 ) {
		
		perror("No valid points found");
		
	} else {
		
		//1st point found - look for next pt and loop through GPS file
		while ( get_next_point(fp, point2) != 0 ) {
			
			//measure distances and add to totals
			ttl_distance += dist_between_points(point1, point2);
			elevation_chg = *point2->alt - *point1->alt;
			
			if (elevation_chg > 0)
				ttl_climb += elevation_chg;
			else
				ttl_descent += elevation_chg;
			
			//copy point2 data to to point1
			copy_point_data(point2, point1);
			
		} // end loop through GPS file to get points
		
	} // end if
	
	//Finished loop because no more points found
	//Assing totals to file structure
	*GPS_file->route_distance = ttl_distance;
	*GPS_file->route_climb = ttl_climb;
	*GPS_file->route_descent = ttl_descent;
	
	
	fclose(fp);
	
	free_point(point1);
	free_point(point2);
	
} //end calc_file_distance()

void copy_point_data (struct GPS_entry* from_point, struct GPS_entry* to_point) {
	
	*to_point->lat = *from_point->lat;
	*to_point->lon = *from_point->lon;
	*to_point->alt = *from_point->alt;
	
} // end copy_point_data

//Create memory space for a point
struct GPS_entry* malloc_point () {
	
	struct GPS_entry* point = malloc(sizeof(struct GPS_entry));
	point->lat = malloc(sizeof(double));
	point->lon = malloc(sizeof(double));
	point->alt = malloc(sizeof(double));
	
	return point;
	
} // end malloc_point

//Free memory space for a point
void free_point ( struct GPS_entry* point ) {
	free(point->lat);
	free(point->lon);
	free(point->alt);
	free(point);
} // end free_point



//Open the file, return the FILE*
//return 0 for success; 1 for failure
//indurect return of FILE* through paramter
FILE* open_GPS_file(char* filename) {
	
	FILE* fp;
	
	if ( (fp = fopen(filename, "r")) == NULL )
		perror("Cannot open file.");
	
	return fp;
}

//This function should return the next GPS point's data
//returns 0 for EOF
//returns 1 if full point was not read
//returns 2 if both elevation and lat/lon are captured for a point
int get_next_point(FILE* fp, struct GPS_entry* next_point) {
	
	double lat_val = 0.0;
	double lon_val = 0.0;
	double ele_val = 0.0;
	int rv = 0;
	
	//Create a format specifier that include a maximum tag
	//length to prevent a runaway assignment that has a
	//length of infinity.
	char latlon_fmt[LINELEN];
	sprintf(latlon_fmt, " <trkpt lat=\"%%%dlf\" lon=\"%%%dlf\">", TAGLEN, TAGLEN);
	
	char ele_fmt[LINELEN];
	sprintf(ele_fmt, " <ele>%%%dlf</ele>", TAGLEN);
	
	//read line
	char line[LINELEN];
	while ( fgets(line, LINELEN, fp) != NULL ) {
		
		//if (strstr(line, "</trkpt>") != NULL) // found end of record
		//	return rv;
		
		//See if line is a trackpoint grouping
		if ( sscanf(line, latlon_fmt, &lat_val, &lon_val) == 2 ) {
			//store these values in the point
			*next_point->lat = lat_val;
			*next_point->lon = lon_val;
			rv++;
			
			//Find corresponding elevation point
			while ( fgets(line, LINELEN, fp) != NULL ) {
				//Test to see if closing </trkpt> tag was
				//found before elevation was found
				if (strstr(line, "</trkpt>") != NULL) { // found end of record
					break; //elevation not found
				} // end if premature closing tag found
				
				//see if the line is an elevation line
				if ( sscanf(line, ele_fmt, &ele_val) == 1 ) {
					//store this values in the point
					*next_point->alt = ele_val;
					rv++;
					break;  //exit the find elevation loop
				} // end if elevation line
			} // end while loop to find elevation line
			
			if (rv == 2) // got lat/lon and elevation so finish
				break;
			
		} // end if trackpoint grouping found
		
	} // end while loop to read lines in a file
	
	return rv; //0 = no points found at EOF; 2 = complete record found
}
