//
//  files_list.c
//	Functions that support searching storing and retirieving GPS files
//  GarFlop
//
//  Created by Mark on 6/22/18.
//  Copyright © 2018 Mark McDonald. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "files_list.h"
#include "distance_calc.h"

//Recursive search for files based on provided starting point
void find_files(char* root_directory, struct GPS_file_list* filelist ){
	
	//search directory
	DIR* dp;
	struct dirent* de;
	struct stat file_stat;
	char fullpathname[MAXNAMLEN];
	
    //TODO: Make an env reader to get $NAME
    
    if ( (dp = opendir(root_directory)) == NULL) {
		perror("Cannot open directory");
        exit(0);
    }
	
	while ( (de = readdir(dp)) != NULL ) {
		
		if (de->d_name[0] == '.') continue; //skip hidden and . and ..
		
		//Get stat on the selected file
		sprintf(fullpathname, "%s/%s", root_directory, de->d_name);
		stat(fullpathname, &file_stat);
		
		// If item is a directory - recurse
		if (S_ISDIR(file_stat.st_mode)) { //directory
		
			char new_fullpathname[MAX_NAMESIZE];
			sprintf(new_fullpathname, "%s/%s", root_directory, de->d_name);
            find_files(new_fullpathname, filelist);  //recurse for new directory
		
		} else {
		
			//Not a directory - determine file type and add to list if GPS file
			char* file_type;
			file_type = &de->d_name[de->d_namlen - 3];
			
			//If filetype is of a GPS file, copy to file list
			if ( strcmp(file_type, "gpx") == 0 || strcmp(file_type, "fit") == 0 )
				add_file(filelist, &file_stat ,de->d_name, fullpathname);
			
		}  // end if else checking for directory or file
		
	} // end while - loop through directory
	
	closedir(dp);
    return;
	
} // end find_files()

void add_file (struct GPS_file_list* filelist,
								struct stat* file_stat,
								char* filename,
								char* fullpathname) {
	
    struct GPS_file* malloc_GPS_file ( void );
    
    //Store previous head in a new variable
    struct GPS_file* prevhead = filelist->head;
    
	//Make space for new GPS file and put the pointers in the header
    filelist->head = malloc_GPS_file();
	strcpy(filelist->head->filename, filename);
	strcpy(filelist->head->filepath, fullpathname);
	filelist->head->filesize = (off_t*)file_stat->st_size;
	filelist->head->mod_date = (__darwin_time_t*)file_stat->st_mtimespec.tv_sec;
	calc_file_distance(filelist->head); //populates distance, climb and descent
	
	if ( prevhead == NULL) { // this is the first file in the list
		filelist->head->sequnce_ID = 1;
	} else { // if an entry already exists, move current head to next
		filelist->head->sequnce_ID = prevhead->sequnce_ID + 1;
	}
    
    filelist->head->next = prevhead;

    return;
    
} // end add_file()


struct GPS_file_list* malloc_filelist () {
	
	struct GPS_file_list* filelist = malloc(sizeof(struct GPS_file_list));
	return filelist;
	
}

struct GPS_file* malloc_GPS_file () {

    struct GPS_file* new_file = malloc(sizeof(struct GPS_file));
	new_file->sequnce_ID = (int)malloc(sizeof(int));
	new_file->filename = malloc(sizeof(char*));
    new_file->filepath = malloc(sizeof(char*));
    new_file->filesize = malloc(sizeof(off_t*));
    new_file->mod_date =malloc(sizeof(__darwin_time_t*));
    new_file->route_distance = malloc(sizeof(double));
    new_file->route_climb = malloc(sizeof(double));
    new_file->route_descent = malloc(sizeof(double));
    
    return new_file;
    
} // end malloc_GPS_file()

