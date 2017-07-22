//
//  test.c
//  mazegener
//
//  Created by Groot Groot on 7/10/17.
//  Copyright © 2017 Harrison Downs. All rights reserved.
//

#include <stdio.h>

#ifndef VECTOR_H
#define VECTOR_H
#include "point.h"
#include "vector.h"
#include "booleans.h"
#include <string.h>
typedef struct Map_T* Map_T;

struct Map_T{
    char **map;
    int width;
    int height;
    int type;
};
extern void printMap(Map_T map);
extern bool checkVector(vector v, Point_t e);
extern int mapCheck(Point_t point, Map_T map);
extern int findPath(int startx, int starty, int endx, int endy, Map_T map, bool mark);
extern void freeMap(Map_T map);


#endif
