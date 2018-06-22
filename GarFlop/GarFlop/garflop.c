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

#define LINESIZE 200
#define NUM_SETTINGS 3
#define DELIM ":"

struct settings {
    char* setting;
    char* value;
};

void read_settings(struct settings*);


int main(int argc, const char * argv[]) {
    
    //read setting file
    struct settings* settings = NULL;
    read_settings(settings);
    
    
    //search for files and store in array
    //present files to user
    //ask for input
    //if Q, or similar, exit
    //determine if Garmin is connected
    //when connected, copy/move file
    //present list of files found based on new search
    
    return 0;
}

void read_settings(struct settings* rv) {
    
    char line[LINESIZE];
    struct settings file_settings[NUM_SETTINGS];
    FILE* fp;
    if ( (fp = fopen("settings.txt", "r")) == NULL )
        perror("Can't read file.");
    
    
    while ( fgets(line, LINESIZE, fp) != NULL ) {
        
        //If line starts with a # or is a blank line (\n), skip it
        if ( line[0] == '#' || line[0] == '\n' ) continue;
        
        //iterate through elements of the line and put
        //a \0 where a # is found
        strcpy(file_settings.setting, line);
        strsep(line, DELIM);
        strcpy(file_settings.value, line);

        }
    
    fclose(fp);
} // end read_settings

struct settings* init_settings() {
    return malloc(sizeof(struct settings));
}

struct settings


