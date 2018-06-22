//
//  files_list.c
//	Functions that support searching storing and retirieving GPS files
//  GarFlop
//
//  Created by Mark on 6/22/18.
//  Copyright © 2018 Mark McDonald. All rights reserved.
//

#include <dirent.h>
#include "files_list.h"

//Recursive search for files based on provided starting point
struct GPS_file_list* find_files(char* root_directory){
	
	//search directory
	DIR* dp;
	struct dirent* de;
	
	if ( (dp = opendir(root_directory)) == NULL)
		perror("Cannot open directory");
	
	while ( (de = readdir(dp)) != NULL ) {
			
	}
	
	
	//if item is applicable file - add it to the list
	//if item is a direcotry - recurse
	
	closedir(dp);
	
} // end find_files()

