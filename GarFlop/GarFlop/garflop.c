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
#include "distance_calc.h"

#define SETTINGS_FILE	"settings.txt"
#define SOURCE_DIR1 "/Users/mcdomx/Downloads/TEST"
#define SOURCE_DIR2 "/Users/markmcdonald/Downloads/TEST"


static struct settings_list* settings_list; //holds list of settings

void process_file ( char* );
void copy_point_data (struct GPS_entry* , struct GPS_entry* );
void free_point ( struct GPS_entry* );
struct GPS_entry* malloc_point ( void );


int main(int argc, const char * argv[]) {
    
    //process settings file
	settings_list = read_settings_file(SETTINGS_FILE);
	
    //search for files and store in array
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
	
	//loop through files in filelist
	while ( cur_GPS_file != NULL ) {
        
        process_file(cur_GPS_file->filepath);
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


void process_file (char* filepath) {
	
    FILE* fp;
    if ( (fp = open_GPS_file(filepath)) == NULL )
        return; //failed to open file - continue to next file

	//Malloc two points
	//TODO: Does this make room for the lat and lon?
	//TODO: May need to create a malloc function
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
    fclose(fp);
    
	free_point(point1);
	free_point(point2);
	
	printf("\nFile: %s\n", filepath);
	printf("Distance: %.1f km\n", ttl_distance/1000);
	printf("Climb: %.1lf m\n", ttl_climb);
	printf("Descent: %.1lf m\n", ttl_descent);
	
	
} //end process_file()

void copy_point_data (struct GPS_entry* from_point, struct GPS_entry* to_point) {
	
	*to_point->lat = *from_point->lat;
	*to_point->lon = *from_point->lon;
	*to_point->alt = *from_point->alt;
	
} // end copy_point_data

//Create memory space for a point
struct GPS_entry* malloc_point ( void ) {
	
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



