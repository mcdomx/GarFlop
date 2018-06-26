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
#include "parse_GPS_file.h"

#define SETTINGS_FILE	"settings.txt"


static struct settings_list* settings_list; //holds list of settings

void process_file ( FILE* , char* );

int main(int argc, const char * argv[]) {
    
    //process settings file
	settings_list = read_settings_file(SETTINGS_FILE);
	
    //search for files and store in array
	struct GPS_file_list* filelist = malloc(sizeof(struct GPS_file_list));
    find_files("/Users/markmcdonald/Downloads/TEST", filelist);
	
	//read each file and collect relevant file data
	struct GPS_file* cur_GPS_file = NULL; //current file being processed
	
	if (filelist->head != NULL)
		cur_GPS_file = filelist->head;
	else {
		printf("No valid GPS files found.\n");
		exit(0);
	}
	
	//loop through files in filelist
	FILE* fp;
	while ( 1 ) {
		if ( (fp = open_GPS_file(cur_GPS_file->filepath)) == NULL )  {
			continue; // file failed to open - continue to next file
		}
		
			//file opened successfully
			process_file(fp, cur_GPS_file->filepath);
			
		if (cur_GPS_file->next == NULL)
			break; //last file in list was processed
		
		cur_GPS_file = cur_GPS_file->next;
		
	} // end while loop
	
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


void process_file (FILE* fp, char* filepath) {
	
	//Malloc two points
	//TODO: Does this make room for the lat and lon?
	//TODO: May need to create a malloc function
	struct GPS_entry* point1 = malloc(sizeof(struct GPS_entry));
	struct GPS_entry* point2 = malloc(sizeof(struct GPS_entry));
	
	double ttl_climb = 0;
	double ttl_descent = 0;
	double ttl_distance = 0;
	double elevation_chg = 0;
	
	//get point 1
	//if lat/lon and elevation aren't part of point
	//consider the point corrupt and skip it
	if ( get_next_point(fp, point1) == 0 ) {
		//EOF found without finding an initial point
		printf("No points found in file: %s", filepath);
		return; // search next file
	} // end while look for point 1
	
	//Loop through GPS file
	while ( get_next_point(fp, point2) != 0 ) {
		
		//measure distances and add to totals
		ttl_distance += dist_between_points(point1, point2);
		elevation_chg = point2->alt - point1->alt;
		
		if (elevation_chg > 0)
			ttl_climb += elevation_chg;
		else
			ttl_descent += elevation_chg;
		
		//copy point2 to to point1
		point1 = point2;
		
	} // end loop through GPS file to get points
	
	//Finished loop because EOF was found
	free(point1);
	free(point2);
	
} //end process_file()



