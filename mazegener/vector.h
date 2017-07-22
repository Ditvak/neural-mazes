//
//  vector.h
//  mazegener
//
//  Created by Groot Groot on 7/10/17.
//  Copyright © 2017 Harrison Downs. All rights reserved.
//

#ifndef VECTOR1_H
#define VECTOR1_H

#define VECTOR_INIT_CAPACITY 4

#define VECTOR_INIT(vec) vector vec; vector_init(&vec)
#define VECTOR_ADD(vec, item) vector_add(&vec, (void *) item)
#define VECTOR_SET(vec, id, item) vector_set(&vec, id, (void *) item)
#define VECTOR_GET(vec, type, id) (type) vector_get(&vec, id)
#define VECTOR_DELETE(vec, id) vector_delete(&vec, id)
#define VECTOR_TOTAL(vec) vector_total(&vec)
#define VECTOR_FREE(vec) vector_free(&vec)


struct vector {
    void **items;
    int capacity;
    int total;
};

typedef struct vector* vector;


extern void vector_init(vector);
extern int vector_total(vector);
static void vector_resize(vector, int);
extern void vector_add(vector, void *);
extern void vector_set(vector, int, void *);
extern void *vector_get(vector, int);
extern void vector_delete(vector, int);
extern void vector_free(vector);

#endif
