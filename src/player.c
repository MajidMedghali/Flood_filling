#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include <time.h>
#include "graph.h"
#include "server_functions.h"

#define NUM_PLAYERS 2

struct player{
  char *name;
  size_t id;
  struct graph_t *graph ;
  enum color_t *colors;
  struct color_set_t forbidden;
};

struct player one;

char const* get_player_name(){
  one.name = "To_The_Moon";
  return one.name;
}

//dfs algorithm 
void algo_coloration1(struct graph_t *graph,enum color_t *colors,size_t id,enum color_t old_color,enum color_t new_color){
  size_t n = graph->num_vertices ;
  if(!((int)id<0 || (int)id>=(int)n || colors[id] != old_color )){
    colors[id] = new_color;
    for (int k = graph->t->p[id]; k < graph->t->p[id+1]; k++) {
      unsigned int j = graph->t->i[k];
      algo_coloration1(graph,colors,j,old_color,new_color);
    }
  }
 
}

//update colors array with a new color
void graph_coloration1(struct graph_t *graph,enum color_t *colors,size_t id,enum color_t new_color){
  enum color_t old_color = colors[id];
  if(old_color != new_color){
    algo_coloration1(graph,colors,id,old_color,new_color);
  }
}

//count the colors that player have
void count_colors1(struct graph_t *graph, enum color_t *colors,enum color_t old_color,enum color_t color ,size_t id,int * tab){
  size_t n = graph->num_vertices ;
  if(!(((int)id<0 || (int)id>=(int)n || (colors[id] != old_color  || tab[id]==1) ) /*&& ((int)id<0 || (int)id>=n || (colors[id] != color || tab[id]!=0 ))*/))  {
    tab[id]=1;
    for (int k = graph->t->p[id]; k < graph->t->p[id+1]; k++) {
      unsigned int j = graph->t->i[k];
      count_colors1(graph,colors,old_color,color,j,tab);
    }
  }
  if(!( ((int)id<0 || (int)id>=(int)n || (colors[id] != color || tab[id]!=0 ))))  {
    tab[id]=1;
    for (int k = graph->t->p[id]; k < graph->t->p[id+1]; k++) {
      unsigned int j = graph->t->i[k];
      count_colors1(graph,colors,color,color,j,tab);
    }
  }
}


//return 1 if both players are connected else return 0
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


//count number of tiles the player will have if he chose color in his move
int count1(struct graph_t *graph, enum color_t *colors,enum color_t color,size_t id,size_t size){
  int c=0;
  enum color_t old_color = colors[id];
  int *t=malloc(sizeof(*t)*size);
  for(int i=0;i<(int)size;i++){;
    t[i]=0;
  }
  count_colors1(graph,colors,old_color,color,id,t);
  for(int i=0;i<(int)size;i++){;
    if(t[i]==1 || t[i]==2)
      c++;
  }
  free(t);
  return c;
  
}


//find the best color the player can play
enum color_t find_best_color1(struct graph_t *graph, enum color_t *colors,size_t id,size_t id_ad,size_t size){
  enum color_t colormax=-1;
  int countmax=count1(graph,colors,colors[id],id,size);
  for(int i = 0; i<8;i++){
    if(!(is_connexe(graph,colors,id,id_ad,size) && (enum color_t)i == colors[id_ad]) && countmax < count1(graph,colors,(enum color_t)i,id,size)){
      colormax= i ;
      countmax=count1(graph,colors,(enum color_t)i,id,size);
      
    }
  }
  return colormax;
}

void initialize(size_t id, struct graph_t* graph,enum color_t* colors,struct color_set_t forbidden[NUM_PLAYERS]){
  one.name="To_The_Moon";
  one.graph=graph;
  one.id = id;
  one.colors = colors;
  one.forbidden = forbidden[id];
  printf(" initialize PASSED\n ");
 
  
}


struct move_t play(struct move_t previous_move){
  struct move_t m;
  if(previous_move.c != -1){
    //update the colors graph with the other player color
    graph_coloration1(one.graph,one.colors,one.graph->positions[(one.id+1)%2],previous_move.c);
    m.c=find_best_color1(one.graph,one.colors,one.graph->positions[one.id],one.graph->positions[(one.id+1)%2],one.graph->num_vertices);
    graph_coloration1(one.graph,one.colors,one.graph->positions[one.id],m.c);
  }
  else{
    m.c=find_best_color1(one.graph,one.colors,one.graph->positions[one.id],one.graph->positions[(one.id+1)%2],one.graph->num_vertices);
    //update the colors array with his color
    graph_coloration1(one.graph,one.colors,one.graph->positions[one.id],m.c);
  }
  return m;
}


void free_table(struct graph_t * G){
  gsl_spmatrix_uint_free(G->t);
  free(G);
  }

void finalize(){
  free_table(one.graph);
  free(one.colors);
  printf("finalize PASSED\n");
}
