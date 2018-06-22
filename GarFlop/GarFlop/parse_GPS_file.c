//
//  parse_GPS_file.c
//  GarFlop
//
//  Created by Mark on 6/22/18.
//  Copyright © 2018 Mark McDonald. All rights reserved.
//

#include <stdio.h>

#include "parse_GPS_file.h"

//Open the file, return the FILE*
FILE* open_GPS_file(char* filename) {
	FILE* fp;
	
	if ( (fp = fopen(filename, "r")) == NULL )
		perror("Cannot open file.");
	
	return fp;
}

//This function should return the next GPS point's data
struct GPS_entry get_next_point(FILE* fp) {
	
	//read line
	//if line starts with <trkpt, capture it
	//Now, parse that line and grab the lat="xxx" and lon="xxx" data
	//Put that data into the struct that will be returned
	
	//Next, read lines until the line starts with <ele>
	//grab that line
	//parse the data between <ele> and </ele>
	//Put that elevation data into the struct to return
	
	//return the struct
	
}

