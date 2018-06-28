//
//  garflop.c
//  GarFlop
//
//  Created by Mark McDonald on 6/21/18.
//  Copyright © 2018 Mark McDonald. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <string.h>

#include "settings_list.h"
#include "files_list.h"
#include "distance_calc.h"

#define SETTINGS_FILE	"settings.txt"
#define SOURCE_DIR1 "/Users/mcdomx/Downloads/TEST"
#define SOURCE_DIR2 "/Users/markmcdonald/Downloads/TEST"


static struct settings_list* settings_list; //holds list of settings
void display_files (struct GPS_file_list* filelist);


int main(int argc, const char * argv[]) {
    
    //process settings file
	settings_list = read_settings_file(SETTINGS_FILE);
	
    //search for files and store in linked list
	struct GPS_file_list* filelist = malloc(sizeof(struct GPS_file_list));
    find_files(SOURCE_DIR2, filelist);
	
	//read each file and collect relevant file data
	struct GPS_file* cur_GPS_file = NULL; //current file being processed
	
	if (filelist->head != NULL)
		cur_GPS_file = filelist->head;
	else {
		printf("No valid GPS files found.\n");
		exit(0);
	}
	
	//loop through files in filelist and create new linked list with file details
	while ( cur_GPS_file != NULL ) {
        
        calc_file_distance(cur_GPS_file);
        cur_GPS_file = cur_GPS_file->next;
		
	} // end while loop
	
	display_files(filelist);
	
	// All files have been processed
    // present files to user
		// loop through files and present data
    // ask for input
    //if Q, or similar, exit
    //determine if Garmin is connected
    //when connected, copy/move file
    //present list of files found based on new search
    
    return 0;
}



void display_files (struct GPS_file_list* filelist) {
	
	//read each file and collect relevant file data
	struct GPS_file* cur_GPS_file = NULL; //current file being processed
	
	if (filelist->head != NULL)
		cur_GPS_file = filelist->head;
	else {
		printf("No valid GPS files found.\n");
		exit(0);
	}
	
	//loop through files in filelist and create new linked list with file details
	while ( cur_GPS_file != NULL ) {
		
		printf("\nFile: %s\n", cur_GPS_file->filepath);
		printf("Distance: %.1f km\n", *cur_GPS_file->route_distance/1000);
		printf("Climb: %.1lf m\n", *cur_GPS_file->route_climb);
		printf("Descent: %.1lf m\n", *cur_GPS_file->route_descent);
		
		cur_GPS_file = cur_GPS_file->next;
		
	} // end while loop
	
	
} // end display_files()

