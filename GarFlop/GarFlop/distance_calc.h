//
//  distance_calc.h
//  GarFlop
//
//  Created by Mark on 6/22/18.
//  Copyright © 2018 Mark McDonald. All rights reserved.
//

#ifndef distance_calc_h
#define distance_calc_h

#include <stdio.h>
#include "parse_GPS_file.h"

double dist_between_points (struct GPS_entry*, struct GPS_entry*);

#endif /* distance_calc_h */
