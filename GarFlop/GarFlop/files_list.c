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

#include "files_list.h"

//Recursive search for files based on provided starting point
struct GPS_file_list* find_files(char* root_directory){
	
    struct GPS_file_list* rv = NULL;
    
	//search directory
	DIR* dp;
	struct dirent* de;
	
	if ( (dp = opendir(root_directory)) == NULL)
		perror("Cannot open directory");
	
	while ( (de = readdir(dp)) != NULL ) {
        // If item is a directory - recurse (type 4 is folder / 8 is a file)
        if (de->d_type == 4)
            find_files(de->d_name);
        
        char* file_type;
        file_type = &de->d_name[de->d_namlen - 3];
        
        //If filetype is of a GPS file, copy to file list
        if ( strcmp(file_type, "gpx") == 0 || strcmp(file_type, "fit") == 0 )
            rv = add_file(rv, de->d_name, root_directory);
        
        printf("%llu\n", de->d_ino);
        printf("%s\n", de->d_name);
        printf("%d\n", de->d_namlen);
        
        printf("%d\n", de->d_type); //4 is a folder -- 8 is a file
	}
	
	
	//if item is applicable file - add it to the list
	//if item is a direcotry - recurse
	
	closedir(dp);
    
    return NULL;
	
} // end find_files()

struct GPS_file_list* add_file (struct GPS_file_list* list, char* file, char* path) {
    
    // Create linked list entry - Add new items at head
    struct GPS_file *prevHeadPtr = list->head;   // save the old head
    
    // replace the head with the new item
    list->head = malloc(sizeof(struct GPS_file));
    
    list->head->filename = malloc(sizeof(*file));
    strcpy(list->head->filename, file);
    
    list->head->filepath = malloc(sizeof(*path));
    strcpy(list->head->filepath, path);
    
    if ( prevHeadPtr == NULL ) { //first entry
        list->head->next = NULL;
    } else {                     //make the old header the new next
        list->head->next = prevHeadPtr;
    }
    
    return list;
    
} // end add_file()

