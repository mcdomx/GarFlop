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

//Recursive search for files based on provided starting point
void find_files(char* root_directory, struct GPS_file_list* filelist ){
	
	//search directory
	DIR* dp;
	struct dirent* de;
	struct stat file_stat;
	char fullpathname[MAXNAMLEN];
	
	if ( (dp = opendir(root_directory)) == NULL)
		perror("Cannot open directory");
	
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
	
	//Create space for GPS_file elements and add variables to them
	struct GPS_file* newGPSfile = malloc(sizeof(struct GPS_file));
	
	newGPSfile->filename = malloc(sizeof(*filename));
	strcpy(newGPSfile->filename, filename);
	
	newGPSfile->filepath = malloc(sizeof(*fullpathname));
	strcpy(newGPSfile->filepath, fullpathname);
	
	newGPSfile->filesize = malloc(sizeof(file_stat->st_size));
	*newGPSfile->filesize = file_stat->st_size;
	
	newGPSfile->mod_date = malloc(sizeof(file_stat->st_mtimespec.tv_sec));
	*newGPSfile->mod_date = file_stat->st_mtimespec.tv_sec;
	
	newGPSfile->route_distance = malloc(sizeof(double));
	*newGPSfile->route_distance = 99.9;
	
	if ( filelist->head == NULL) { // this is the first file in the list
	
		newGPSfile->sequnce_ID = 1;
		filelist->head = newGPSfile;
		filelist->head->next = NULL;
	
	} else { // if an entry already exists, move current head to next
	
		newGPSfile->sequnce_ID = filelist->head->sequnce_ID + 1;
		struct GPS_file* prevhead = filelist->head;
		filelist->head = newGPSfile;
		filelist->head->next = prevhead;
	
	}

    return;
    
} // end add_file()

