#ifndef _FLOOD_G_FUNC_H_
#define _FLOOD_G_FUNC_H_
#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include <string.h>
#include <stdint.h>
#include "graph.h"




void algo_coloration(struct graph_t *graph,enum color_t *colors,size_t id,enum color_t old_color,enum color_t new_color);
void graph_coloration(struct graph_t *graph,enum color_t *colors,size_t id,enum color_t new_color);
void count_colors(struct graph_t *graph, enum color_t *colors,enum color_t old_color,enum color_t color,size_t id,int * tab);
int count(struct graph_t *graph, enum color_t *colors,enum color_t color,size_t id,size_t size);




struct graph_t *graph_copy(struct graph_t* graph);

int get_in_matrix(struct graph_t *graph,int i,int j);


void graph_coloration_bfs(struct graph_t *graph,enum color_t *colors,size_t id,enum color_t new_color);
void color_graph(struct graph_t *graph,enum color_t *colors,size_t id,enum color_t new_color,size_t algo);
struct queue* create_queue(unsigned capacity);
struct queue{
    int front, rear, size;
    unsigned capacity;
    int* array;
};
int is_empty(struct queue* queue);
void enqueue(struct queue* queue, int item);
int dequeue(struct queue* queue);


#endif
