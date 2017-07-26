//
//  neural_net.h
//  mazegener
//
//  Created by Groot Groot on 7/23/17.
//  Copyright © 2017 Harrison Downs. All rights reserved.
//

#ifndef neural_net_h
#define neural_net_h

#include <stdio.h>
#include "vector.h"
#include "booleans.h"


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
    
};
typedef struct NeuralNet* NeuralNet;


struct Connection{
    float weight;
    //Neuron start;
    Neuron to;
};
typedef struct Connection* Connection;





#endif /* neural_net_h */
