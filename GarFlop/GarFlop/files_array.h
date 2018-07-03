//
//  files_array.h
//  GarFlop
//
//  Created by Mark on 6/22/18.
//  Copyright © 2018 Mark McDonald. All rights reserved.
//

#ifndef files_array_h
#define files_array_h

#include <stdio.h>

#define MAX_NAMESIZE 256



//GPS file structure
struct GPS_file {
	int sequnce_ID;
	char* filename;				//filename only with extention
	char* filepath;				//filename with full path
	off_t* filesize;
	time_t* mod_date;
	double* route_distance;
	double* route_climb;
	double* route_descent;
	struct GPS_file* next;
};
typedef struct GPS_file GPS_FILE;

void find_files(char* root_directory, GPS_FILE* array_list[] );
void add_file (GPS_FILE* array_list[],
			   struct stat* file_stat,
			   char* filename,
			   char* fullpathname);

#endif /* files_list_h */
