//
//  settings_list.c
//	Functions and structures needed to support settings
//  GarFlop
//
//  Created by Mark on 6/22/18.
//  Copyright © 2018 Mark McDonald. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "settings_list.h"

#define LINELEN			1024
#define	PARAM_LEN		128
#define	VAL_LEN			512


/* Function:   add_setting()
 * Desc:       Adds a new type and descirpiton to the type list.
 * Args:       List of types and the type to add
 * Return:     nothing -- only adds an element to the list supplied
 */
void add_setting (struct settings_list* settings_list, char* param, char* value) {
	
	// Create linked list entry - Add new items at head
	struct setting *prevHeadPtr = settings_list->head;   // save the old head
	
	// replace the head with the new item
	settings_list->head = malloc(sizeof(struct setting));
	
	settings_list->head->setting_name = malloc(sizeof(*param));
	strcpy(settings_list->head->setting_name, param);
	
	settings_list->head->setting_value = malloc(sizeof(*value));
	strcpy(settings_list->head->setting_value, value);
	
	
	if ( prevHeadPtr == NULL ) { //first entry
		settings_list->head->next = NULL;
	} else {                     //make the old header the new next
		settings_list->head->next = prevHeadPtr;
	}
	
	
}// end add_setting


/* Function:   get_setting()
 * Desc:       Returns the setting value of given setting name
 * Args:       List of settings and the name of the setting to get value of
 * Return:     The value of the setting as a string
 */
char* get_setting (struct settings_list* settings_list, char* find_setting) {
	
	char* rv = NULL;
	struct setting* s_ptr = settings_list->head;
	
	while ( 1 ) {    						// Look for setting in list
		
		if ( strcmp(s_ptr->setting_name, find_setting) == 0 ) {
			rv = s_ptr->setting_value;		// return the value if found
			break;
		}
		
		if  ( s_ptr->next == NULL ) {  		// value not found
			printf("Setting not found: %s", s_ptr->setting_name);
			break;
		}
		
		s_ptr = s_ptr->next; 				// check next item in list
		
	}
	
	return rv;
	
} // end get_setting()



/*
 * read_param()
 *   purpose -- read next parameter setting line from fp
 *   details -- parameter settings are on a line with the
 *				setting name + tab + setting value
 *     extra -- skip over lines that start with # and those
 *				that do not contain two strings
 *   returns -- EOF at eof and 1 on good data
 *				Indorect return to name, value
 *
 */
int read_param(FILE *fp, char *param, int plen, char *value, int vlen)
{
	char line[LINELEN];
	int	c;
	char fmt[100];
	
	sprintf(fmt, "%%%ds%%%ds", plen, vlen);
	
	/* read in next line and if the line is too long, read until \n */
	while( fgets(line, LINELEN, fp) != NULL ) {
		
		if ( line[strlen(line)-1] != '\n' )
			while( (c = getc(fp)) != '\n' && c != EOF );
		
		if ( sscanf(line, fmt, param, value ) >= 2 && *param != '#' )
			return 1;
	}
	
	return EOF;
}


/*
 * Function:	read_settings_file()
 * Desc:		Read elements of setting file into memory
 * Args:		filename of settings file
 * Return:		Pointer to header element in settings file
 *				NULL if file empty or not found
 */
struct settings_list* read_settings_file(char* filename) {
	
	FILE* fp;
	
	//Open file
	if ( (fp = fopen("settings.txt", "r")) == NULL )
		perror("Can't read file.");
	
	//Variables
	struct settings_list* rv = malloc(sizeof(struct settings_list));
	char param[PARAM_LEN];
	char value[VAL_LEN];
	
	//Read settings file parameters and add to list
	while ( read_param(fp, param, PARAM_LEN, value, VAL_LEN) != EOF ) {
		add_setting(rv, param, value);
	} // end while
	
	return rv;
} // end read_settings_file()

