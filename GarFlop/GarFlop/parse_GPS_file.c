//
//  parse_GPS_file.c
//  GarFlop
//
//  Created by Mark on 6/22/18.
//  Copyright © 2018 Mark McDonald. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_GPS_file.h"
#include "distance_calc.h"

#define LINELEN		1024
#define TAGLEN		30

//Open the file, return the FILE*
//return 0 for success; 1 for failure
//indurect return of FILE* through paramter
FILE* open_GPS_file(char* filename) {
	
	FILE* fp;
	
	if ( (fp = fopen(filename, "r")) == NULL )
		perror("Cannot open file.");
	
	return fp;
}

//Create memory for a GPS point and return the pointer to it
struct GPS_entry* malloc_GPS_entry() {
	
	struct GPS_entry* rv = malloc(sizeof(struct GPS_entry));
	
	return rv;
	
}

//TODO:  I may not need this function
//This function will parse a lat or lon meta tag
//format must be lat="123456"
//returns a double of the value in quotes
double parse_latlon( char tag[TAGLEN] ) {
	
	char fmt[100];
	double tag_val = 0.0;
	sprintf(fmt, "lat=\"%%%dlf\"", TAGLEN);
	sscanf(tag, fmt, tag_val);
	
	return tag_val;
	
} // end parse_latlon()

//TODO:  I may not need this function
//This function will parse out the elevation value between
//elevation tags <ele>12345</ele>
//returns a double of the elevaion value
double parse_elevation( char tag[TAGLEN] ) {
	
	char fmt[100];
	double ele_val = 0.0;
	sprintf(fmt, "<ele>%%%dlf</ele>", TAGLEN);
	sscanf(tag, fmt, ele_val);
	
	return ele_val;
	
} // end parse_elevation


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
	
	
	char ele_fmt[150];
	sprintf(ele_fmt, " <ele>%%%dlf</ele>", TAGLEN);
	
	//read line
	char line[LINELEN];
	while ( fgets(line, LINELEN, fp) != NULL ) {
		
		if (strstr(line, "</trkpt>") != NULL) // found end of record
			return rv;
		
		//See if line is a trackpoint grouping
		if ( sscanf(line, latlon_fmt, &lat_val, &lon_val) == 2 ) {
			//store these values in the point
			next_point->lat = malloc(sizeof(double));
			*next_point->lat = lat_val;
			
			next_point->lon = malloc(sizeof(double));
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
					next_point->alt = malloc(sizeof(double));
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

double dist_between_points (struct GPS_entry* pt1, struct GPS_entry* pt2) {
	
	return calc_distance(*pt1->lat, *pt1->lon, *pt1->alt, *pt2->lat, *pt2->lon, *pt2->alt);
	
	
} // end dist_between_points()
