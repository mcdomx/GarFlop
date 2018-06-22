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
struct GPS_file_list* find_files(char* root_directory){
	
    struct GPS_file_list* list_ptr = malloc(sizeof(struct GPS_file_list));
    
	//search directory
	DIR* dp;
	struct dirent* de;
	
	if ( (dp = opendir(root_directory)) == NULL)
		perror("Cannot open directory");
	
	while ( (de = readdir(dp)) != NULL ) {
		
		if (de->d_name[0] == '.') continue; //skip hidden and . and ..
		
        // If item is a directory - recurse (type 4 is folder / 8 is a file)
		if (de->d_type == 4) {
			char fullpath[MAX_NAMESIZE];
			sprintf(fullpath, "%s/%s", root_directory, de->d_name);
            find_files(fullpath);  //recurse for new directory
		}
		
		//Get the 3 character file extension
        char* file_type;
        file_type = &de->d_name[de->d_namlen - 3];
        
        //If filetype is of a GPS file, copy to file list
        if ( strcmp(file_type, "gpx") == 0 || strcmp(file_type, "fit") == 0 )
            list_ptr = add_file(list_ptr, de->d_ino ,de->d_name, root_directory);

	} // end while - loop through directory
	
	closedir(dp);
    return list_ptr;
	
} // end find_files()



struct GPS_file_list* add_file (struct GPS_file_list* list, ino_t inode, char* file, char* path) {
	
	//Create a fullpath filename
	char fullpathname[MAX_NAMESIZE];
	sprintf(fullpathname, "%s/%s", path, file);
	
    // Create linked list entry - Add new items at head
    struct GPS_file *prevHeadPtr = list->head;   // save the old head
    
    // replace the head with the new item
    list->head = malloc(sizeof(struct GPS_file));
    
    list->head->filename = malloc(sizeof(*file));
    strcpy(list->head->filename, file);
    
    list->head->filepath = malloc(sizeof(*path));
    strcpy(list->head->filepath, path);
	
	//Get filesize and time stamp using stat()
	struct stat inostat;
	stat(fullpathname, &inostat);
	
	//list->head->filesize = malloc(sizeof(off_t));
	list->head->filesize = inostat.st_size;
	
	list->head->mod_date = inostat.st_mtimespec.tv_sec;
    
    if ( prevHeadPtr == NULL ) { //first entry
        list->head->next = NULL;
    } else {                     //make the old header the new next
        list->head->next = prevHeadPtr;
    }
    
    return list;
    
} // end add_file()

