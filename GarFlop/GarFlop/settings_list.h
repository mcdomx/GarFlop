//
//  settings_list.h
//  GarFlop
//
//  Created by Mark on 6/22/18.
//  Copyright © 2018 Mark McDonald. All rights reserved.
//

#ifndef settings_list_h
#define settings_list_h

//Setting in linked list
struct setting {
	char* setting_name;
	char* setting_value;
	struct setting *next;
};

//Settings linked list
struct settings_list {
	struct setting *head;
};

struct settings_list* read_settings_file(char* );
void add_setting (struct settings_list* , char* , char* );
char* get_setting (struct settings_list*, char*);

#endif /* settings_list_h */
