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

#define SETTINGS_FILE	"settings.txt"





static struct settings_list* settings_list; //holds list of settings

int main(int argc, const char * argv[]) {
    
    //process settings file
	settings_list = read_settings_file(SETTINGS_FILE);
	
    //search for files and store in array
	
	
    //present files to user
    //ask for input
    //if Q, or similar, exit
    //determine if Garmin is connected
    //when connected, copy/move file
    //present list of files found based on new search
    
    return 0;
}




