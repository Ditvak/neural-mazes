//
//  mazegen.h
//  mazegener
//
//  Created by Groot Groot on 7/11/17.
//  Copyright © 2017 Harrison Downs. All rights reserved.
//

#ifndef mazegen_h
#define mazegen_h

#include <stdio.h>
#include <stdlib.h>
#include "pathfinder.h"

struct Stock{
    float currentVal;
    float prediction;
    float **data;
};
typedef struct Stock* Stock;



struct Neuron {
    int state;
    int inpMax;
    float threshold;
    float current;
    bool fired;
    int isOutput;
    int ID;
    vector connections;
};
typedef struct Neuron* Neuron;

struct NeuralNet {
    vector inputs;
    vector hiddenLayers;
    vector outputs;
    
 //   Neuron outputN;
    int output;
    int fitness;
    
    Map_T node;
    
    
};
typedef struct NeuralNet* NeuralNet;


struct Connection{
    float weight;
    //Neuron start;
    Neuron to;
};
typedef struct Connection* Connection;

enum TileType{
    space = 0,
    wall = 1,
    edge = 2
};

//int nodeSize = 5;

typedef enum TileType* TileType;

extern float fitnessStock(float zero, float one, float price, float nextPrice)

extern void predictStock(Stock stock);
extern void generateMaze(Map_T map, int nodeSize);
extern void generateNode(Map_T map, int x, int y, int nodeSize);
extern NeuralNet generateNeuralNode(Map_T map, int x, int y, int nodeSize);
extern void mutate(NeuralNet net);
extern void blankMutate(NeuralNet net);
void runNeuralNode(NeuralNet net, int nodeSize);
void genBlankNeuralNet(NeuralNet net, int nodeSize);
void combine(NeuralNet net, NeuralNet net2);
void copyNet(NeuralNet *newNet, NeuralNet net);
void clearNet(NeuralNet net);
void clearNeuron(Neuron n);
extern void makeNeuron(Neuron *n, int state, int inpMax, int isOutput);
extern void blankMap(Map_T map);
extern int fitness(Map_T map);
#endif /* mazegen_h */
