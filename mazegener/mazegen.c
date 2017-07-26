//
//  mazegen.c
//  mazegener
//
//  Created by Harrison Downs on 7/11/17.
//  Copyright © 2017 Harrison Downs. All rights reserved.
//
//  mazegen.c is designed to create and run a neural network that generates a maze of variable size using variably sized "nodes"
//            each node runs its own neural network.

#include "mazegen.h"
#include <stdlib.h>
#include "booleans.h"
#include "pathfinder.h"
#include "point.h"

int connRand = 256; // number of possible connection weights.
float connRandF = 0.0f; // float value for number of possible weights. Ignore 0.0f;



/* genMap() - initializes a map variable with blank characters and such */
void genMap(Map_T map, int nodeW, int nodeH){
    
    map->width = nodeW;
    map->height = nodeH;
    
    map->map = (char**)malloc(sizeof(char*) * map->width);
    
    for (int i = 0; i < map->width; i++){
        map->map[i] = (char*)malloc(sizeof(char) * map->height);
    }
    blankMap(map);
    
}

float fitnessStock(float zero, float one, float price, float nextPrice){
    float difference = nextPrice - price;
    float total = zero + one;
    float high = 0.0;
    
    if (zero > one){
        high = zero;
    }
    else{
        high = one;
    }
    
    float stockMod = high / total;
   
    
    
    return stockMod * difference;
}

/* fitness() - calculates the fitness of a particular node */
int fitness(Map_T map){
    int fit = 0;
    Point_t dir[8] = {newPoint(1, 0), newPoint(-1, 0), newPoint(0, 1), newPoint(0, -1),
                      newPoint(1, 1), newPoint(-1, -1), newPoint(1, -1), newPoint(-1, 1)};
    
    int ud = 0;//5 * findPath(map->width / 2, 0, map->width / 2, map->height - 1, map, false);
    int lr = 5 * findPath(0, map->height / 2, map->width - 1, map->height / 2, map, false);
    
    
   // fit += 5 * findPath(map->width / 2, 0, map->width / 2, map->height - 1, map, false);
   // fit += 5 * findPath(0, map->height / 2, map->width - 1, map->height / 2, map, false);
    
    /*if (fit == -5){
        fit = -30;
    }*/
    
    if (ud == -5 || lr == -5){
        fit = map->width * -10;
    }
    
    int wall = 0;
    int floor = 0;
    
    for (int i = 0; i < map->width; i++){
        for (int j = 0; j < map->height; j++){
            if (map->map[i][j] == ' '){
                floor++;
            }
            else if (map->map[i][j] == '#'){
                wall++;
            }
        }
    }
    
    fit -= abs(wall - floor);
    
    if (wall == map->height * map->width){
        fit -= 20;
    }
    else if (floor == map->height * map->width){
        fit -= 60;
    }
    
    for (int i = 0; i < 8; i++){
        free(dir[i]);
    }
    return fit;
}

/* blankMap() - blanks out an entire map / node */
void blankMap(Map_T map){
    for (int j = 0; j < map->height; j++){
        for (int i = 0; i < map->width; i++){
            map->map[i][j] = ' ';
        }
    }
}

/* generateMaze() - external function that generates a map made out of nodes */
void generateMaze(Map_T map, int nodeSize){
    
    connRandF = (float)connRand;
    
    
    for (int j = 0; j < map->height / nodeSize; j++){
        for (int i = 0; i < map->width / nodeSize; i++){
            srand(j * map->width + i);
            NeuralNet top = generateNeuralNode(map, 0, 0, nodeSize);
            runNeuralNode(top, nodeSize);
            
            for (int x = 0; x < nodeSize; x++){
                for (int y = 0; y < nodeSize; y++){
                    map->map[i * nodeSize + x][j * nodeSize + y] = top->node->map[x][y];
                    
                }
            }
            
            printMap(top->node);
            int connTotal = 0;
            for (int t = 0; t < vector_total(top->hiddenLayers); t++){
                Neuron n = vector_get(top->hiddenLayers, t);
                connTotal += vector_total(n->connections);
            }
            for (int l = 0; l < vector_total(top->inputs); l++){
                Neuron n = vector_get(top->inputs, l);
                connTotal += vector_total(n->connections);
            }
            
            printf("i is %d, j is %d, hidden layer is %d, totalConn is %d\n", i, j, vector_total(top->hiddenLayers), connTotal);
            
            freeMap(top->node);
            
        }
    }
    printMap(map);
    
}

int compareNet(const void* a, const void* b){
    NeuralNet netA = *(NeuralNet*)a;
    NeuralNet netB = *(NeuralNet*)b;
    
    if (netA->fitness > netB->fitness){
        return 1;
    }
    else if (netA->fitness == netB->fitness){
        return 0;
    }
    else{
        return -1;
    }
    
}



/* generateNeuralNode() - generates a neural network in a set amount of iterations */
NeuralNet generateNeuralNode(Map_T map, int x, int y, int nodeSize){
    int iterations = 180;
    int perIteration = 100;
    
    // inputs to the neural network
    // inputs: node info relative to coordinate (all 8 directions)
    // inputs: last placed node
    // inputs: positionx
    // inputs: positiony
    // possible inputs to add: greater than or less than x and y coordinates?
    
    NeuralNet netArray[perIteration];
    int fitMax = 0;
    for (int i = 0; i < perIteration; i++){
        // Step 1 Create Neural Network
        NeuralNet net = malloc(sizeof(struct NeuralNet));
        genBlankNeuralNet(net, nodeSize);
        
        for (int i = 0; i < 9; i++){
            blankMutate(net);
        }
        
        netArray[i] = net;
    
        // Step 2 Run Node Builder Simulation using Neural Network
    
        runNeuralNode(net, nodeSize);
        net->fitness = fitness(net->node);
        if (net->fitness > fitMax){
            fitMax = net->fitness;
        }
        freeMap(net->node);
        free(net->node);
    }

    for (int iter = 0; iter < iterations; iter++){
        qsort((void*)netArray, perIteration, sizeof(NeuralNet), compareNet);
        NeuralNet top = netArray[perIteration - 1];

        for (int i = 0; i < perIteration; i++){
            if (i < 3 * perIteration / 4){
                int ran = rand() % (perIteration / 4) + (3 * perIteration / 4);
                clearNet(netArray[i]);
                NeuralNet nn;
                NeuralNet oneToCopy = netArray[ran];
                copyNet(&nn, oneToCopy);
                netArray[i] = nn;
                mutate(nn);
         
            }
            
            NeuralNet newNet = netArray[i];
            runNeuralNode(newNet, nodeSize);
            newNet->fitness = fitness(newNet->node);
            if (newNet->fitness > fitMax){
                fitMax = newNet->fitness;
            }
            freeMap(newNet->node);
            free(newNet->node);
        }
    }
    
    qsort((void*)netArray, perIteration, sizeof(NeuralNet), compareNet);
    NeuralNet top = netArray[perIteration - 1];
    
    for (int i = 0; i < perIteration - 1; i++){
        clearNet(netArray[i]);
    }

    return top;
    
}


void copyNeuron(Neuron newNeuron, Neuron neuron){
    newNeuron->current = 0.0f;
    newNeuron->inpMax = neuron->inpMax;
    newNeuron->state = neuron->state;
    newNeuron->isOutput = neuron->state;
    newNeuron->state = neuron->state;
    newNeuron->threshold = neuron->threshold;
}

void copyConnections(Neuron newNeuron, Neuron neuron, NeuralNet newNet){
    for (int i = 0; i < vector_total(neuron->connections); i++){
        Connection c = vector_get(neuron->connections, i);
        int ID = c->to->ID;
        Neuron toN;
        if (ID >= 2){
            toN = vector_get(newNet->hiddenLayers, ID - 2);
        }
        else if (ID >= 0){ // output Neuron
            toN = vector_get(newNet->outputs, ID);
        }
        else{
            toN = vector_get(newNet->inputs, (ID + 1) * -1);
        }
        Connection newC = malloc(sizeof(struct Connection));
        newC->to = toN;
        newC->weight = c->weight;
        vector_add(newNeuron->connections, newC);
    }
}

/*copyNet() - creates a copy of a given neuralNet at a specific position*/
void copyNet(NeuralNet *newNet, NeuralNet net){
    (*newNet) = malloc(sizeof(struct NeuralNet));
    
    NeuralNet n = (*newNet);
    n->fitness = 0;

    
    genBlankNeuralNet(n, net->node->height);
    
    
    /* adds hidden layer neurons */
    for (int i = 0; i < vector_total(net->hiddenLayers); i++){
        Neuron neuron;
        makeNeuron(&neuron, 0, 0, 0);
        neuron->ID = i + 2;
        vector_add(n->hiddenLayers, neuron);
        copyNeuron(neuron, vector_get(net->hiddenLayers, i));
        
    }
    
    /* copies input neurons*/
    for (int i = 0; i < vector_total(net->inputs); i++){
        Neuron neuron = vector_get(net->inputs, i);
        Neuron newNeuron = vector_get(n->inputs, i);
        copyNeuron(newNeuron, neuron);
        copyConnections(newNeuron, neuron, n);
    }
    
    /* copies hidden layer connections*/
    for (int i = 0; i < vector_total(n->hiddenLayers); i++){
        Neuron newNeuron = vector_get(n->hiddenLayers, i);
        Neuron neuron = vector_get(net->hiddenLayers, i);
        copyConnections(newNeuron, neuron, n);
    }
    
 
    
}


/* clearNeurons() - resets a neural network to it's default state*/
void clearNeurons(NeuralNet net){
    vector h = net->hiddenLayers;
    vector in = net->inputs;
    vector o = net->outputs;
    
    for (int i = 0; i < vector_total(h); i++){
        Neuron n = vector_get(h, i);
        n->current = 0.0f;
        n->fired = false;
    }
    
    for (int i = 0; i < vector_total(in); i++){
        Neuron n = vector_get(in, i);
        n->current = 0.0f;
        n->fired = false;
    }
    
    for (int i = 0; i < vector_total(o); i++){
        Neuron n = vector_get(o, i);
        n->current = 0.0f;
        n->fired = false;
    }
    
}

/* clearNet() - deletes a given Neural Network by clearing all it's neurons and then freeing it's memory*/
void clearNet(NeuralNet net){
    for (int i = 0; i < vector_total(net->inputs); i++){
        clearNeuron(vector_get(net->inputs, i));
    }
    for (int i = 0; i < vector_total(net->hiddenLayers); i++){
        clearNeuron(vector_get(net->hiddenLayers, i));
    }
    for (int i = 0; i < vector_total(net->outputs); i++){
        clearNeuron(vector_get(net->outputs, i));
    }
    vector_free(net->inputs);
    free(net->inputs);
    vector_free(net->hiddenLayers);
    free(net->hiddenLayers);
    vector_free(net->outputs);
    free(net->outputs);
    free(net);
    
}

/*clearNeuron() - not to be confused with clearNeurons(), this function is reponsible for taking a given neuron and freeing all of 
                  it's memory by deleting all of it's connections and then freeing the memory it takes up.*/
void clearNeuron(Neuron n){
    for (int i = 0; i < vector_total(n->connections); i++){
        Connection c = vector_get(n->connections, i);
        free(c);
    }
    vector_free(n->connections);
    free(n->connections);
    free(n);
}



/* runNeuralNode() - the actual simulation of the neural network. Takes in a given NeuralNet (and a size property) and then
                     simulates the node. It runs the network and then returns a fitness which it puts in the NeuralNet object.*/
void runNeuralNode(NeuralNet net, int nodeSize){
    Map_T node = malloc(sizeof(struct Map_T));
    genMap(node, nodeSize, nodeSize);
    Point_t directions[8] = {newPoint(-1, -1), newPoint(0, -1), newPoint(1, -1), newPoint(-1, 0), newPoint(1, 0), newPoint(-1, 1), newPoint(0, 1), newPoint(1, 1), };
    int lastPlace = -1;
    
    clearNeurons(net);
    
    for (int i = 0; i < nodeSize; i++){
        for (int j = 0; j < nodeSize; j++){
            int inputs[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            
            for (int k = 0; k < 8; k++){
                Point_t dir = directions[k];
                Point_t rel = newPoint(dir->x + i, dir->y + j);
                
                if (mapCheck(rel, node) == true){
                    if (node->map[rel->x][rel->y] == ' '){
                        inputs[k] = 0;
                    }
                    else {
                        inputs[k] = 1;
                    }
                }
                else{
                    inputs[k] = 2;
                }
                
                free(rel);
            }
            inputs[8] = lastPlace;
            inputs[9] = i;
            inputs[10] = j;
            
            vector conns = malloc(sizeof(struct vector));
            vector_init(conns);
            
            vector toCheck = malloc(sizeof(struct vector));
            vector_init(toCheck);
            
            vector checked = malloc(sizeof(struct vector));
            vector_init(checked);
            
            
            // Input vector is now complete with 1 or 0 to represent values and i/j for x and y coordinates
            // ########
            
            //First, grab all the connections from the inputs and put them in a vector to check called conns
            
            for (int k = 0; k < 11; k++){
                Neuron inputN = vector_get(net->inputs, k);
                
                for (int o = 0; o < vector_total(inputN->connections); o++){
                    Connection inpC = vector_get(inputN->connections, o);
                    if (inpC->to->state == inputs[k]){
                        inpC->to->current += inpC->weight;
                        vector_add(checked, inpC);
                    }
                    
                }
                
                
            }
            
            /* check if any of the neurons have fired*/
            for (int k = 0; k < vector_total(net->hiddenLayers); k++){
                Neuron hN = vector_get(net->hiddenLayers, k);
                
                if (hN->current >= hN->threshold && hN->fired == false){
                    hN->fired = true;
                    for (int l = 0; l < vector_total(hN->connections); l++){
                        Connection hC = vector_get(hN->connections, l);
                        vector_add(toCheck, hC);
                    }
                    
                }
                
            }
            
            /* run through and simulate the entire network*/
            while(vector_total(toCheck) > 0){
                for (int o = 0; o < vector_total(toCheck); o++){
                    Connection inpC = vector_get(toCheck, o);
                    
                    inpC->to->current += inpC->weight;
                    
                    vector_add(checked, inpC);
                    
                    
                }
                for (int o = 0; o < vector_total(toCheck); o++){
                    vector_delete(toCheck, 0);
                }
                
                for (int k = 0; k < vector_total(net->hiddenLayers); k++){
                    Neuron hN = vector_get(net->hiddenLayers, k);
                    
                    if (hN->current >= hN->threshold && hN->fired == false){
                        hN->fired = true;
                        for (int l = 0; l < vector_total(hN->connections); l++){
                            Connection hC = vector_get(hN->connections, l);
                            vector_add(toCheck, hC);
                        }
                        
                    }
                    
                }
                
            }
            
            /* figure out which input is higher*/
            Neuron zero = vector_get(net->outputs, 0);
            Neuron one = vector_get(net->outputs, 1);
            
            if (zero->current > one->current){
                node->map[i][j] = ' ';
                lastPlace = 0;
            }
            else{
                node->map[i][j] = '#';
                lastPlace = 1;
            }
            
            
            // free associated memory
            vector_free(conns);
            vector_free(toCheck);
            vector_free(checked);
            free(conns);
            free(toCheck);
            free(checked);
        }
    }
    
    // free directions array
    for (int i = 0; i < 8; i++){
        free(directions[i]);
    }
    
    net->node = node;
}


/* makeNeuron() - creates a new neuron object when given a double pointer ** Neuron */
void makeNeuron(Neuron *n, int state, int inpMax, int isOutput){
    *n = malloc(sizeof(struct Neuron));
    (*n)->state = state;
    (*n)->inpMax = inpMax;
    (*n)->isOutput = isOutput;
    (*n)->connections = malloc(sizeof(struct vector));
    (*n)->current = 0.0f;
    (*n)->threshold = (float)(rand() % connRand) / connRandF;
    vector_init((*n)->connections);
    
}




/* genBlankNeuralNet() - generates an empty neutral network */
void genBlankNeuralNet(NeuralNet net, int nodeSize){
    net->inputs = malloc(sizeof(struct vector));
    net->hiddenLayers = malloc(sizeof(struct vector));
    net->outputs = malloc(sizeof(struct vector));
    

    
    Neuron outputSpace;
    Neuron outputWall;
    
    makeNeuron(&outputSpace, 0, 2, 1);
    makeNeuron(&outputWall, 1, 2, 1);
    
    vector_init(net->inputs);
    vector_init(net->hiddenLayers);
    vector_init(net->outputs);
    
    vector_add(net->outputs, outputSpace);
    vector_add(net->outputs, outputWall);
    
    outputWall->threshold = 1000.0f;
    outputWall->ID = 1;
    outputSpace->threshold = 1000.0f;
    outputSpace->ID = 0;
    
    
    net->output = 0;
    net->fitness = 1;
    
    /* generates the 8 directional inputs*/
    for (int i = 0; i < 8; i++){
        Neuron directional;
        makeNeuron(&directional, 0, 3, 0);
        vector_add(net->inputs, directional);
        directional->ID = (i * -1) - 1;
    }
    
    Neuron lastPlace;
    Neuron posX;
    Neuron posY;
    
    makeNeuron(&lastPlace, -1, 4, 0);
    lastPlace->ID = -9;
    makeNeuron(&posX, -1, nodeSize, 0);
    posX->ID = -10;
    makeNeuron(&posY, -1, nodeSize, 0);
    posY->ID = -11;
    
    vector_add(net->inputs, lastPlace);
    vector_add(net->inputs, posX);
    vector_add(net->inputs, posY);

}

void mutate(NeuralNet net){
    if (rand() % 3 != 0){
        // adds a random neuron with two connections (one input one output)
        blankMutate(net);
    }
    else{
        if (rand() % 3 == 0){
            // add connection between an input node and an output node
            int inpInd = rand() % vector_total(net->inputs);
            Neuron inpN = vector_get(net->inputs, inpInd);
            
            Connection inpC = malloc(sizeof(struct Connection));
            inpC->weight = (float)(rand() % connRand) / connRandF;
            inpC->to = (Neuron)(vector_get(net->outputs, rand() % 2)); //net->outputN;
            
            vector_add(inpN->connections, inpC);
        }
        else if (rand() % 3 == 0){
            // add a connection between two random nodes
            int hInd = rand() % vector_total(net->inputs);
            Neuron hN = vector_get(net->inputs, hInd);
            
            Connection ranC = malloc(sizeof(struct Connection));
            ranC->weight = (float)(rand() % connRand) / connRandF;
            
            int rInd = rand() % (2 + vector_total(net->hiddenLayers));
            if (rInd < 2){
                ranC->to = (Neuron)(vector_get(net->outputs, rInd));
                vector_add(hN->connections, ranC);
            }
            else{
                Neuron n = vector_get(net->hiddenLayers, rInd - 2);
                if (n->threshold != hN->threshold){
                    ranC->to = n;
                    vector_add(hN->connections, ranC);
                    
                }
                else{
                    free(ranC);
                }
            }
        }
        
        else{
            // randomly disable a connection or neuron, or change the weight/threshold of a connection or neuron
            int connTotal = 0;
            int nodeTotal = 0;
            for (int i = 0; i < vector_total(net->inputs); i++){
                Neuron n = vector_get(net->inputs, i);
                connTotal += vector_total(n->connections);
            }
            for (int i = 0; i < vector_total(net->hiddenLayers); i++){
                Neuron n = vector_get(net->hiddenLayers, i);
                connTotal += vector_total(n->connections);
            }
            
            nodeTotal += vector_total(net->hiddenLayers);
            
            // get the total number of nodes and connections
            int total = nodeTotal + connTotal;
            
            int random = rand() % total;
            bool deathMutation = (rand() % 6 == 0);
            if (random < connTotal){ // mutate connection weight
                bool done = false;
               
                for (int i = 0; i < vector_total(net->inputs); i++){ // first check the input nodes
                    Neuron n = vector_get(net->inputs, i);
                    if (random - vector_total(n->connections) < 0){
                        Connection c = vector_get(n->connections, random);
                        if (deathMutation == false){
                            c->weight = (float)(rand() % connRand) / connRandF;
                        }
                        else{
                            vector_delete(n->connections, random);
                        }
                        
                        done = true;
                        break;
                    }
                    else{
                        random -= vector_total(n->connections);
                    }
                }
                if (done == false){ // then check the hidden nodes
                    for (int i = 0; i < vector_total(net->hiddenLayers); i++){
                        Neuron n = vector_get(net->hiddenLayers, i);
                        if (random - vector_total(n->connections) < 0){
                            Connection c = vector_get(n->connections, random);
                            if (deathMutation == true){
                                c->weight = (float)(rand() % connRand) / connRandF;
                            }
                            else{
                                vector_delete(n->connections, random);
                            }
                            done = true;
                            break;
                        }
                        else{
                            random -= vector_total(n->connections);
                        }
                    }
                }
                
            }
            else{ // mutate node threshold
                random -= connTotal;
                Neuron n = vector_get(net->hiddenLayers, random);
                if (deathMutation == false){
                    n->threshold = (float)(rand() % connRand) / connRandF;
                }
                else{
                    n->threshold = 10000.0f;
                }
            }
            
            
        }
    }
}

/* combine() - combines the nodes and connections of two separate neural networks*/
void combine(NeuralNet net, NeuralNet net2){

    // checks and adds any nodes or connections (note: NOT A DEEP COPY)
    for (int i = 0; i < vector_total(net2->inputs); i++){
        Neuron inp2 = vector_get(net2->inputs, i);
        Neuron inp1 = vector_get(net->inputs, i);
        for (int j = 0; j < vector_total(inp2->connections); j++){
            bool alreadyIn = false;
            
            Connection c2 = vector_get(inp2->connections, j);
            for (int k = 0; k < vector_total(inp1->connections); k++){
                Connection c1 = vector_get(inp1->connections, k);
                if (c1->weight == c2->weight){
                    alreadyIn = true;
                }
            }
            
            if (alreadyIn == false){
                vector_add(inp1->connections, c2);
            }
        }
    }
    for (int i = 0; i < vector_total(net2->hiddenLayers); i++){
        
        Neuron hl2 = vector_get(net2->hiddenLayers, i);
        bool inN1 = false;
        
        for (int j = 0; j < vector_total(net->hiddenLayers); j++){
            Neuron hl1 = vector_get(net->hiddenLayers, j);
            if (hl1->threshold == hl2->threshold){
                inN1 = true;
            }
        }
        if (inN1 == false){
            vector_add(net->hiddenLayers, hl2);
        }

    }
}

/* blankMutate() - create a mutation for the first round of neural networks. Adds a random neuron and two connections (one i one o)*/
void blankMutate(NeuralNet net){
    if (2 % 2 == 0){
        // add Neuron with two connections, one to input and one to an output
        Neuron n = malloc(sizeof(struct Neuron));
        n->connections = malloc(sizeof(struct vector));
        
       
        vector_init(n->connections);
        
        
        int inpInd = rand() % vector_total(net->inputs);
        
        Neuron inpN = vector_get(net->inputs, inpInd);

        n->inpMax = inpN->inpMax;
        n->state = rand() % n->inpMax;
        n->threshold = (float)(rand() % connRand) / connRandF;
        n->ID = vector_total(net->hiddenLayers) + 2;
        vector_add(net->hiddenLayers, n);
        
        Connection inpC = malloc(sizeof(struct Connection));
        inpC->weight = (float)(rand() % connRand) / connRandF;
        inpC->to = n;
        
        Connection outC = malloc(sizeof(struct Connection));
        outC->weight = (float)(rand() % connRand) / connRandF;
        
        outC->to = (Neuron)(vector_get(net->outputs, rand() % 2));//net->outputN;
        
        vector_add(n->connections, outC);
        vector_add(inpN->connections, inpC);
        
    }
    
}

/* generateNode() - function that generates a single node using random chance (NOT USED IN CURRENT FILE)*/
void generateNode(Map_T map, int x, int y, int nodeSize){
    
    int halfSize = nodeSize / 2;
    int maxSize = nodeSize - 1;
    
    bool canFind = false;
    
    Map_T node = malloc(sizeof(struct Map_T));
    genMap(node, nodeSize, nodeSize);
    
    Point_t paths[2][6] =
        {{newPoint(halfSize, 0), newPoint(halfSize, 0), newPoint(halfSize, 0),
            newPoint(0, halfSize), newPoint(0, halfSize), newPoint(maxSize, halfSize)},
        {newPoint(maxSize, halfSize), newPoint(0, halfSize), newPoint(halfSize, maxSize),
            newPoint(maxSize, halfSize), newPoint(halfSize, maxSize), newPoint(halfSize, maxSize)}};

    int c = 0;
    while(canFind == false){
        c++;
        blankMap(node);
        canFind = true;
        for (int i = 0; i < node->width; i++){
            for(int j = 0; j < node->height; j++){
                if (rand() % 4 == 0 ){
                    node->map[i][j] = ' ';
                }
                else{
                    node->map[i][j] = '#';
                }
            }
        }
        
        
        for (int i = 0; i < 6; i++){
            Point_t start = paths[0][i];
            Point_t end = paths[1][i];
            
            bool f = findPath(start->x, start->y, end->x, end->y, node, false);
            
            if (f == -1){
                canFind = false;
            }
        }

        
    }
    
    printf("------\n");
    printMap(node);
    
    
    printf("---\n");
    int i = fitness(node);
    printf("fitness is %d\n", fitness(node));
    
    
}
