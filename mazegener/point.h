//
//  point.h
//  mazegener
//
//  Created by Groot Groot on 7/10/17.
//  Copyright © 2017 Harrison Downs. All rights reserved.
//

#ifndef point_h
#define point_h

#include <stdio.h>
typedef struct Point_t* Point_t;

struct Point_t{
    int x;
    int y;
    Point_t parent;
};


#endif /* point_h */


extern Point_t newPoint(int xv, int yv);
