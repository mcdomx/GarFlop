//
//  files_list.h
//  GarFlop
//
//  Created by Mark on 6/22/18.
//  Copyright © 2018 Mark McDonald. All rights reserved.
//

#ifndef files_list_h
#define files_list_h

#include <stdio.h>

#define MAX_NAMESIZE 256

//GPS file structure
struct GPS_file {
	int sequnce_ID;
	char* filename;		//filename only with extention
	char* filepath;		//filename with full path
	off_t* filesize;
	time_t* mod_date;
	double* route_distance;
	double* route_climb;
	double* route_descent;
    struct GPS_file* next;
};

//Linked list of GPS files
struct GPS_file_list {
	struct GPS_file* head;
};

void find_files(char*, struct GPS_file_list* );
void add_file (struct GPS_file_list* , struct stat*, char* , char* );
char* get_file (struct GPS_file_list*, char*);

#endif /* files_list_h */
