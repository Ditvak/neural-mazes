//
//  main.c
//  mazegener
//
//  Created by Groot Groot on 7/10/17.
//  Copyright © 2017 Harrison Downs. All rights reserved.
//

//#include <stdio.h>

#include <stdlib.h>
#include "pathfinder.h"
#include "mazegen.h"
#include "input_stock.h"
#include <unistd.h>

int main(int argc, const char * argv[]) {
    //time_t t;
    srand(27);
    // insert code here...
    printf("Hello, World!\n");
    
   float **data;
    
    loadData(&data, "EOD-AAPL.csv");
    
    char buf[2048];
    getcwd(buf, sizeof(buf));
  /*  fprintf(stderr, "Dir is: %s\n", buf);
    
    for (int i = 0; i < 25; i++){
        printf("ey\n");
    }
    */
    
    /*
    Map_T map = malloc(sizeof(Map_T));
    
    map->width = 70;
    map->height = 55;
    int nodeSize = 5;
    
    map->map = (char**)malloc(sizeof(char*) * map->width);
    
    for (int i = 0; i < map->width; i++){
        map->map[i] = (char*)malloc(sizeof(char) * map->height);
    }
   // fprintf(stderr, "crap");
    for (int j = 0; j < map->height; j++){
        for (int i = 0; i < map->width; i++){
            if (i == 0 || i == map->width - 1 || j == 0 || j == map->height - 1){
                map->map[i][j] = '#';
            }
            else{
                //map->map[i][j] = ' ';
               // printf("%d\n", rand());
                if (rand() % 5 == 0 && i != 40 && j != 40 && i != 10 && j != 10){
                    map->map[i][j] = '#';
                }
                else{
                    map->map[i][j] = ' ';
                }
            }
        }
    }
    
  //  fprintf(stderr, "ohlord");
    
    generateMaze(map, nodeSize);    //(map, 0, 0);
    /*printf("\n------------------------------------------\n");
    findPath(2, 0, 47, 49, map, true);
    printMap(map);
    printf ("\n DONE! \n");*/
    return 0;
}



