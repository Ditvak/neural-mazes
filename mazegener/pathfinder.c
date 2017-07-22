//
//  pathfinder.c
//  mazegener
//
//  Created by Groot Groot on 7/10/17.
//  Copyright © 2017 Harrison Downs. All rights reserved.
//

//#include <stdio.h>
#include "pathfinder.h"
#include <stdlib.h>
#include "booleans.h"


int findPath(int startx, int starty, int endx, int endy, Map_T map, bool mark){
    
    
    vector toCheck = malloc(sizeof(vector));
    vector checked = malloc(sizeof(vector));
    vector_init(toCheck);
    vector_init(checked);
    
    
    Point_t directions[4] = {newPoint(1, 0), newPoint(-1, 0), newPoint(0, 1), newPoint(0, -1)};
    
    Point_t startP = newPoint(startx, starty);
    startP->parent = NULL;
    if (map->map[startx][starty] != '#'){
            vector_add(toCheck, startP);
    }
    else{
        free(startP);
    }
    int ret = -1;
    
    
    int count = 0;
    while(vector_total(toCheck) > 0){
    //    printf("-------\n");
     //   printf("vector_total is %d\n", vector_total(toCheck));
      //  printf("count = %d\n", count);
        count++;
        Point_t current = vector_get(toCheck, 0);
        
        
        vector_delete(toCheck, 0);
        
        if (current->x == endx && current->y == endy){
            Point_t pCur = current;
            if (mark == true){
                map->map[pCur->x][pCur->y] = 'X';
            }
            pCur = pCur->parent;
        //    free(current);
         //   printf("\n FOUND IT!\n");
            ret = 1;
            while(pCur != NULL){
                if (mark == true){
                    map->map[pCur->x][pCur->y] = '.';
                }
                
                ret++;
               // Point_t temp = pCur;
                pCur = pCur->parent;
               // free(temp);
                
                for (int j = 0; j < vector_total(toCheck); j++){
                    free(vector_get(toCheck, 0));
                    vector_delete(toCheck, 0);
                }
            }
            if (mark == true){
                map->map[startx][starty] = 'S';
            }
            
            
        }
        else{
       //     printf("current point is %d, %d\n", current->x, current->y);
            for (int i = 0; i < 4; i++){
                Point_t dir = directions[i];
                Point_t newP = newPoint(current->x + dir->x, current->y + dir->y);
                newP->parent = current;
                
                if (mapCheck(newP, map) == true){
                    if (map->map[newP->x][newP->y] == ' '){
                        if (checkVector(checked, newP) == false && checkVector(toCheck, newP) == false){
                            
                //            printf("adding to toCHECK\n");
                            
                            vector_add(toCheck, newP);
                        }
                        else{
                            free(newP);
                            //printf("woo i = %d, char is \'%c\' at position %d, %d\n", i, map->map[newP->x][newP->y], newP->x, newP->y);
                           // free(newP);
                        }
                    }
                    else{
                        vector_add(checked, newP);
                    }
                }
                else{
                    free(newP);
                }
                
            }
            
        }
        vector_add(checked, current);
        //printf("rippo in my hippo");
    }
    
    for (int i = 0; i < 4; i++){
        free(directions[i]);
    }
    
    for (int i = 0; i < vector_total(checked); i++){
        Point_t p = vector_get(checked, i);
        free(p);
    }
    for (int i = 0; i < vector_total(toCheck); i++){
        Point_t p = vector_get(toCheck, i);
        free(p);
    }
    vector_free(checked);
    vector_free(toCheck);
    free(checked);
    free(toCheck);
  //  printf("what?");
  //  printMap(map);
    
    return ret;
}




void printMap(Map_T map){
    
    for (int j = 0; j < map->height; j++){
        printf("\n%4d:", j);
        for (int i = 0; i < map->width; i++){
            printf("%c", map->map[i][j]);
        }
        
    }
    printf("\n");
}



bool checkVector(vector v, Point_t e){
    for (int i = 0; i < vector_total(v); i++){
        Point_t p = (Point_t)(vector_get(v, i));
      //  fprintf(stderr, "woopde %p", p);
        
        if (p != NULL && (p->x == e->x && p->y == e->y)){
            return true;
        }
    }
    return false;
}


int mapCheck(Point_t point, Map_T map){
    if (point->x < 0 || point->y < 0 || point->x >= map->width || point->y >= map->height){
        return false;
    }
    return true;
}

void freeMap(Map_T map){
    for (int i = 0; i < map->height; i++){
        free(map->map[i]);
    }
    free(map->map);
   // free(map);
}

