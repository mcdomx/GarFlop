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


