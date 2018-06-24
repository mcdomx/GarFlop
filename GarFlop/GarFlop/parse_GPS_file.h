//
//  parse_GPS_file.h
//  GarFlop
//
//  Created by Mark on 6/22/18.
//  Copyright © 2018 Mark McDonald. All rights reserved.
//

#ifndef parse_GPS_file_h
#define parse_GPS_file_h

#include <stdio.h>

struct GPS_entry {
	double lat;
	double lon;
	double alt;
};

int open_GPS_file(char* filename, FILE* fp);
int get_next_point(FILE* fp, struct GPS_entry* next_point);
double dist_between_points (struct GPS_entry*, struct GPS_entry*);

#endif /* parse_GPS_file_h */
