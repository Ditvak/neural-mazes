//
//  input_stock.c
//  mazegener
//
//  Created by Groot Groot on 7/23/17.
//  Copyright © 2017 Harrison Downs. All rights reserved.
//

#include "input_stock.h"
#include "string.h"

//int numRows = 8;
int dataLength = 365;

/* returns data array. 365 columns (x), 5 rows:
   Rows: Open, High, Low, Close, Volume */ 
void loadData(double *** data, char * fname){
    (*data) = (double**)malloc(sizeof(double*) * dataLength);
    
    FILE *f = fopen(fname, "rb");
    size_t size = 0;
    
    char *line = NULL;

    
    double** array = *data;
    
    for (int i = 0; i < dataLength; i++){
        array[i] = (double*)malloc(sizeof(double) * 5);
    }
    
    
    // ignore top line (labels)
    getline(&line, &size, f);
    
    char *delim = ",";
    
    for (int k = 0; k < dataLength; k++){
        getline(&line, &size, f);
        char *token;
        token = strtok(line, delim);
        int counter = 0;
      
        while (token != NULL){
            token = strtok(NULL, delim);
            
            if (counter >= 0 && counter < 5){
                array[k][counter] = atof(token);
            }
            
            counter++;
        }
    }
    for (int k = 0; k < dataLength; k++){
        for (int i = 0; i < 5; i++){
            printf("%f ", array[k][i]);
        }
        printf("\n");
    }

    printf("%s\n", line);

    fclose(f);
}
