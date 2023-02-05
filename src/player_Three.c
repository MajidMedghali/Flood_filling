#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include <time.h>
#include "graph.h"
#include "graph_functions.h"
#include "server_functions.h"

struct player{
  char *name;
  size_t id;
  struct graph_t *graph;
  enum color_t *colors;
  struct color_set_t forbidden;
};

struct player three;

char const* get_player_name(){
  three.name="Ninja";
  return three.name;
}

/* int color_in_forbidden(enum color a){ */
/*   int i=0; */
/*   while(i<MAX_COLOR && a!=tree.forbidden[i]) */
/*     i++; */
/*   if(i<MAX_COLOR) return 1; */
/*   return 0; */
/* } */




void initialize(size_t id, struct graph_t* graph,enum color_t* colors,struct color_set_t forbidden[NUM_PLAYERS]){
  three.name="Ninja";
  three.graph=graph;
  three.id = id;
  three.colors = colors;
  three.forbidden = forbidden[id];
  printf(" initialize PASSED \n ");
}


void count_colors1(struct graph_t *graph, enum color_t *colors,enum color_t old_color,enum color_t color ,size_t id,int * tab){
  size_t n = graph->num_vertices ;
  if(!(((int)id<0 || (int)id>=(int)n || (colors[id] != old_color  || tab[id]==1) ) /*&& ((int)id<0 || (int)id>=n || (colors[id] != color || tab[id]!=0 ))*/))  {
    tab[id]=1;
    for (unsigned int k = graph->t->p[id]; k < (unsigned int)graph->t->p[id+1]; k++) {
      unsigned int j = graph->t->i[k];
      count_colors1(graph,colors,old_color,color,j,tab);
    }
  }
  if(!( ((int)id<0 || (int)id>=(int)n || (colors[id] != color || tab[id]!=0 ))))  {
    tab[id]=1;
    for (unsigned int k = graph->t->p[id]; k < (unsigned int)graph->t->p[id+1]; k++) {
      unsigned int j = graph->t->i[k];
      count_colors1(graph,colors,color,color,j,tab);
    }
  }
}

int is_connexe(struct graph_t *graph, enum color_t *colors,int id,int id_ad,size_t size){
  enum color_t color = colors[id];
  enum color_t color_ad = colors[id_ad];

  int *t=malloc(sizeof(*t)*size);
  for(int i=0;i<(int)size;i++){;
    t[i]=0;
  }
  count_colors1(graph,colors,color,color_ad,id,t);
  if(t[id_ad]==1){
    free(t);
    return 1;
  }
  free(t);
  return 0;
  
}
// function to create a queue
// of given capacity.
// It initializes size of queue as 0

int color_possible(enum color_t* colors, int id){
  //size_t size=Three.graph->num_vertices;
  return ( colors[three.graph->positions[three.id]]!=colors[id]  );
  
}

struct move_t play(struct move_t previous_move){
  
  struct move_t m;
  if(previous_move.c != -1){
    graph_coloration(three.graph,three.colors,three.graph->positions[(three.id+1)%2],previous_move.c);
  }
  
  m.c=-1;
  size_t n = three.graph->num_vertices ;
  int *t=malloc(sizeof(*t)*n);
  for(int i=0;i<(int)n;i++){;
    t[i]=0;
  }
  int id=three.graph->positions[three.id];
  int id_ad=three.graph->positions[(three.id+1)%2];
  
  struct queue* queue = create_queue(1000);
  enqueue(queue, id);
  while(!is_empty(queue)){
    int id_i = dequeue(queue);
     t[id_i]=1;
     if( (is_connexe(three.graph,three.colors,id,id_ad,n  ) && three.colors[id_i]==three.colors[id_ad] ) || forbiden_color( three.colors[id_i], three.forbidden)){
       m.c=-1;
       
     }
     else if(!((int)id_i<0 || (int)id_i>=(int)n || color_possible(three.colors,id_i))){
      //printf("%d ",id_i);
      
      for (unsigned int k =three.graph->t->p[id_i]; k < (unsigned int)three.graph->t->p[id_i+1]; k++) {
	unsigned int j =three.graph->t->i[k];
	if(t[j]!=1){
	  enqueue(queue, j);
	  t[j]=1;
	}
      }
    }
    
    else{
      m.c=three.colors[id_i];
      graph_coloration(three.graph,three.colors,three.graph->positions[three.id],m.c);
      break;
    }
  }
  free(queue->array);
  free(t);
  free(queue);
  return m;
}
        
void free_table(struct graph_t * G){
  gsl_spmatrix_uint_free(G->t);
  free(G);
}

void finalize(){
  free_table(three.graph);
  free(three.colors);
  printf("finalize PASSED\n");
}
