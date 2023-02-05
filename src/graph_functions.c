#include "graph_functions.h"
#include "server_functions.h"
#include <math.h>
//dfs algorithm
void algo_coloration(struct graph_t *graph,enum color_t *colors,size_t id,enum color_t old_color,enum color_t new_color){
  size_t n = graph->num_vertices ;
  if(!((int)id<0 || (int)id>=(int)n || colors[id] != old_color )){
    colors[id] = new_color;
    for (int k = graph->t->p[id]; k < graph->t->p[id+1]; k++) {
      unsigned int j = graph->t->i[k];
      algo_coloration(graph,colors,j,old_color,new_color);
    }
  }
 
}


//return the element i,j in the matrix
int get_in_matrix(struct graph_t *graph,int i,int j){
  return gsl_spmatrix_uint_get(graph->t,i,j);
}


//color the graph with dfs algorithm
void graph_coloration(struct graph_t *graph,enum color_t *colors,size_t id,enum color_t new_color){
  enum color_t old_color = colors[id];
  if(old_color != new_color){
    algo_coloration(graph,colors,id,old_color,new_color);
  }
}


//change the value from 0 to 1 in the tab if the color in colors is equal to old_color or equal to color
void count_colors(struct graph_t *graph, enum color_t *colors,enum color_t old_color,enum color_t color ,size_t id,int * tab){
  size_t n = graph->num_vertices ;
  if(!(((int)id<0 || (int)id>=(int)n || (colors[id] != old_color  || tab[id]==1) )))  {
    tab[id]=1;
    for (int k = graph->t->p[id]; k < graph->t->p[id+1]; k++) {
      unsigned int j = graph->t->i[k];
      count_colors(graph,colors,old_color,color,j,tab);
    }
  }
  if(!( ((int)id<0 || (int)id>=(int)n || (colors[id] != color || tab[id]!=0 ))))  {
    tab[id]=1;
    for (int k = graph->t->p[id]; k < graph->t->p[id+1]; k++) {
      unsigned int j = graph->t->i[k];
      count_colors(graph,colors,color,color,j,tab);
    }
   
  }
}

//count the number of tiles a player with id have
int count(struct graph_t *graph, enum color_t *colors,enum color_t color,size_t id,size_t size){
  int c=0;
  enum color_t old_color = colors[id];
  int *t=malloc(sizeof(*t)*size);
  for(int i=0;i<(int)size;i++){;
    t[i]=0;
  }
  count_colors(graph,colors,old_color,color,id,t);
  for(int i=0;i<(int)size;i++){;
    if(t[i]==1 || t[i]==2)
      c++;
  }
  free(t);
  return c;
  
}


//copy the graph graph
struct graph_t *graph_copy(struct graph_t* graph){
  struct graph_t *G = malloc(sizeof(struct graph_t));
  G->t=gsl_spmatrix_uint_alloc(graph->t->size1,graph->t->size2);
  
  //gsl_spmatrix_uint_memcpy(G->t,graph->t);
  G->num_vertices=graph->num_vertices;
  G->positions[0]=graph->positions[0];
  G->positions[1]=graph->positions[1];
  return G;
}


 
// function to create a queue
// of given capacity.
// It initializes size of queue as 0
struct queue* create_queue(unsigned capacity)
{
    struct queue* queue = (struct queue*)malloc(
        sizeof(struct queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
 
    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(
        queue->capacity * sizeof(int));
    return queue;
}
 
 
// Queue is empty when size is 0
int is_empty(struct queue* queue)
{
    return (queue->size == 0);
}
 
// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct queue* queue, int item)
{
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}
 
// Function to remove an item from queue.
// It changes front and size
int dequeue(struct queue* queue)
{
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)
                   % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}
 
//update the colors array with bfs algorithm
void graph_coloration_bfs(struct graph_t *graph,enum color_t *colors,size_t id,enum color_t new_color){
  enum color_t old_color = colors[id];
  size_t n = graph->num_vertices ;
 
  if(old_color != new_color){
    struct queue* queue = create_queue(1000);
    enqueue(queue, id);
    while(!is_empty(queue)){
      int id_i = dequeue(queue);
      if(!((int)id_i<0 || (int)id_i>=(int)n || colors[id_i] != old_color )){
	colors[id_i] = new_color;
	for (int k = graph->t->p[id_i]; k < graph->t->p[id_i+1]; k++) {
	  unsigned int j = graph->t->i[k];
	  enqueue(queue, j);	  
	}
      }
    }
    free(queue->array);
    free(queue);
    
  }

  
}

//update the colors array
void color_graph(struct graph_t *graph,enum color_t *colors,size_t id,enum color_t new_color,size_t algo){
  if(algo == 1 || algo==0){
    graph_coloration_bfs(graph,colors,id,new_color);
  }
    
    
}







