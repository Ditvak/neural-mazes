//
//  point.c
//  mazegener
//
//  Created by Groot Groot on 7/10/17.
//  Copyright © 2017 Harrison Downs. All rights reserved.
//

#include "point.h"
#include <stdlib.h>

Point_t newPoint(int xv, int yv){
    Point_t ret = malloc(sizeof(struct Point_t));
    ret->x = xv;
    ret->y = yv;
    return ret;
}
