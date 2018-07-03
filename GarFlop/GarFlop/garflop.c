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
#include "files_array.h"
#include "distance_calc.h"

#define SETTINGS_FILE	"settings.txt"
#define SOURCE_DIR2 "/Users/mcdomx/Downloads/TEST"
#define SOURCE_DIR1 "/Users/markmcdonald/Downloads/TEST"


static struct settings_list* settings_list; //holds list of settings
//void display_files (struct GPS_file_list* filelist);
void display_files (GPS_FILE* []);

int main(int argc, const char * argv[]) {
    
    //process settings file
	settings_list = read_settings_file(SETTINGS_FILE);
	
    //search for files and store in linked list
	//struct GPS_file_list* filelist = malloc_filelist();
	GPS_FILE* GPS_file_array[1];
	
    //find_files(SOURCE_DIR1, filelist);
	find_files(SOURCE_DIR1, GPS_file_array);
	
	//read each file and collect relevant file data
//	struct GPS_file* cur_GPS_file = NULL; //current file being processed
	
//	if (filelist->head != NULL)
//		cur_GPS_file = filelist->head;
//	else {
//		printf("No valid GPS files found.\n");
//		exit(0);
//	}
	
	if ( sizeof(GPS_file_array) == 1 ) {
		printf("No valid GPS files found.\n");
		exit(0);
	}
	
	//loop through files in filelist and create new linked list with file details
	int i = 0;
	int num_elements  = (int)sizeof(GPS_file_array);
	while ( i++ < num_elements ) {
        
        calc_file_distance(GPS_file_array[i]);
		
	} // end while loop
	
	// All files have been processed
	// present files to user
	display_files(GPS_file_array);
	
    // ask for input
	int max_ID = num_elements;
	int user_input = 0;
	while ( scanf("%d", &user_input) == 0 ) {
		printf("You typed %d.\n", user_input);
		if ( (user_input == 0) || (user_input > max_ID) ) break;
	};
	
    //if Q, or similar, exit
    //determine if Garmin is connected
    //when connected, copy/move file
    //present list of files found based on new search
    
    return 0;
}


void display_files (GPS_FILE* GPS_file_array[]) {
	
	//read each file and collect relevant file data
	if ( sizeof(*GPS_file_array) == 1 ) {
		printf("No valid GPS files found.\n");
		exit(0);
	}
	
	int i = 0;
	int num_elements  = (int)sizeof(*GPS_file_array);
	
	//loop through files in filelist and create new linked list with file details
	while ( i < num_elements ) {
		
		printf("\nRoute Number: %d", i);
		printf("\nFile: %s\n", GPS_file_array[i]->filepath);
		printf("Distance: %.1f km\n", *GPS_file_array[i]->route_distance/1000);
		printf("Climb: %.1lf m\n", *GPS_file_array[i]->route_climb);
		printf("Descent: %.1lf m\n", *GPS_file_array[i]->route_descent);
		
	} // end while loop
	
} // end display_files()




void display_files_old (struct GPS_file_list* filelist) {
	
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
		
		printf("\nRoute Number: %d", cur_GPS_file->sequnce_ID);
		printf("\nFile: %s\n", cur_GPS_file->filepath);
		printf("Distance: %.1f km\n", *cur_GPS_file->route_distance/1000);
		printf("Climb: %.1lf m\n", *cur_GPS_file->route_climb);
		printf("Descent: %.1lf m\n", *cur_GPS_file->route_descent);
		
		cur_GPS_file = cur_GPS_file->next;
		
	} // end while loop
	
	
} // end display_files()

