//
//  garflop.c
//  GarFlop
//
//  Created by Mark McDonald on 6/21/18.
//  Copyright © 2018 Mark McDonald. All rights reserved.
//

#include <stdio.h>

struct settings {
    int folder_search;
    int units;
    char* path_to_garmin;
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

void read_settings(struct settings* settings) {
    
    char* line;
    FILE* fd = fopen("settings.txt","r");
    
    while ( fread(line++, 500, 1, fd) != '\n' ){
        if (line[0] == "#") continue;
        fprintf(stdout, "%s", line);
    }
    
    fclose(fd);
}
