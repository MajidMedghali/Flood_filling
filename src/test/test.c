#include <stdio.h>
#include "../color.h"
#include <assert.h>
#include <string.h>
#include "../initialise.h"
#include "../graph_type.h"
#include "../genius_functions.h"
#include "../graph_functions.h"


static void test_color_to_string(void){
  enum color_t r=1;
  enum color_t b=0;
  enum color_t g=2;
  enum color_t y=3;
  enum color_t o=4;
  enum color_t v=5;
  enum color_t c=6;
  enum color_t p=7;
  
  assert(strcmp(color_to_string(b),"BLUE")==0);
  assert(strcmp(color_to_string(r),"RED")==0);
  assert(strcmp(color_to_string(g),"GREEN")==0);
  assert(strcmp(color_to_string(y),"YELLOW")==0);
  assert(strcmp(color_to_string(o),"ORANGE")==0);
  assert(strcmp(color_to_string(v),"VIOLET")==0);
  assert(strcmp(color_to_string(c),"CYAN")==0);
  assert(strcmp(color_to_string(p),"PINK")==0);
  printf("test_color_to_string valide\n");
}
static void test_array_copy(void){
  enum color_t* t=initialise(4,7,1);
  enum color_t* copy=array_copy(t,4);
  assert(t[0]==copy[0]);
  assert(t[1]==copy[1]);
  assert(t[2]==copy[2]);
  assert(t[3]==copy[3]);

  free(t);
  free(copy);
  printf("array_copy valide\n");
}  

static void test_initialise(void){
  int size=9  ;
  int nmbre=7;
  enum color_t *colors=initialise(size,nmbre,1);
  assert((int)colors[0]>=0);
  assert((int)colors[0]<=7);
  afficher_hache(colors,size);
  printf("afficher_hach valide\n");
  afficher_donut(colors,size);
  printf("afficher_donut valide\n");
  afficher_square(colors,size);
  printf("afficher_square valide\n");
  //assert(strlen(colors)==10);
  printf("initialise : OK\n");
  free(colors);
}


static void test_graph_square(void){
  int n=2;
  struct graph_t* s=square_coo(n);
  assert(gsl_spmatrix_uint_get(s->t, 0, 0)==0);
  assert(gsl_spmatrix_uint_get(s->t, 0, 1)==1);
  assert(gsl_spmatrix_uint_get(s->t, 0, 2)==1);
  assert(gsl_spmatrix_uint_get(s->t, 0, 3)==0);
  assert(gsl_spmatrix_uint_get(s->t, 1, 0)==1);
  assert(gsl_spmatrix_uint_get(s->t, 1, 1)==0);
  assert(gsl_spmatrix_uint_get(s->t, 1, 2)==0);
  assert(gsl_spmatrix_uint_get(s->t, 1, 3)==1);
  assert(gsl_spmatrix_uint_get(s->t, 2, 0)==1);
  assert(gsl_spmatrix_uint_get(s->t, 2, 1)==0);
  assert(gsl_spmatrix_uint_get(s->t, 2, 2)==0);
  assert(gsl_spmatrix_uint_get(s->t, 2, 3)==1);
  assert(gsl_spmatrix_uint_get(s->t, 3, 0)==0);
  assert(gsl_spmatrix_uint_get(s->t, 3, 1)==1);
  assert(gsl_spmatrix_uint_get(s->t, 3, 2)==1);
  assert(gsl_spmatrix_uint_get(s->t, 3, 3)==0);
  printf("test_carré: OK\n");
  gsl_spmatrix_uint_free(s->t);
  free(s);
}

static void test_donut(void){
  int size=3;
  struct graph_t* s=donut_coo(size);
  assert(gsl_spmatrix_uint_get(s->t, 1, 4)==0);
  assert(gsl_spmatrix_uint_get(s->t, 4, 1)==0);
  assert(gsl_spmatrix_uint_get(s->t, 3, 4)==0);
  assert(gsl_spmatrix_uint_get(s->t, 4, 3)==0);
  assert(gsl_spmatrix_uint_get(s->t, 5, 4)==0);
  assert(gsl_spmatrix_uint_get(s->t, 4, 5)==0);
  assert(gsl_spmatrix_uint_get(s->t, 7, 4)==0);
  assert(gsl_spmatrix_uint_get(s->t, 4, 7)==0);
  printf("test_donut: OK\n");
  gsl_spmatrix_uint_free(s->t);
  free(s);

}

static void test_hache(void){
  int size=3;
  struct graph_t* s=hache_coo(size);
  assert(gsl_spmatrix_uint_get(s->t, 1, 0)==0);
  assert(gsl_spmatrix_uint_get(s->t, 0, 1)==0);
  assert(gsl_spmatrix_uint_get(s->t, 1, 2)==0);
  assert(gsl_spmatrix_uint_get(s->t, 2, 1)==0);
  assert(gsl_spmatrix_uint_get(s->t, 1, 4)==0);
  assert(gsl_spmatrix_uint_get(s->t, 4, 1)==0);
  assert(gsl_spmatrix_uint_get(s->t, 7, 4)==0);
  assert(gsl_spmatrix_uint_get(s->t, 4, 7)==0);
  assert(gsl_spmatrix_uint_get(s->t, 6, 7)==0);
  assert(gsl_spmatrix_uint_get(s->t, 7, 6)==0);
  printf("test_hache: OK\n");
  gsl_spmatrix_uint_free(s->t);
  free(s);

}


void player_initialise(struct player* p, int id,
		       struct graph_t* g, enum color_t* colors){
  p->id=id;
  p->graph=g;
  p->colors=colors;
}

static void test_genius_functions(){
  printf("test de fonctions de min-max\n");
  int size=9  ;
  int nmbre=7;
  struct player genius;
  struct node_t* tree=malloc(sizeof(struct node_t));
  struct graph_t* s=square_csr(size);
  enum color_t* colors=initialise(size*size,nmbre,1);
  player_initialise(&genius, 0, s, colors);
  afficher_square(colors,size*size);
  
  make_root(tree, colors);
  assert(tree_length(tree)==1);
  printf(" test_make_root: OK\n");

  int tab_voisins[size*size];
  for(int i=0; i<size*size; i++)
    tab_voisins[i]=0;
  color_exist(genius.graph,colors,colors[genius.graph->positions[genius.id]],genius.graph->positions[genius.id],tab_voisins);
  assert(tab_voisins[2]==2);
  assert(tab_voisins[9]==2);
  assert(tab_voisins[10]==2);
  printf(" test_color_exist: OK\n");

  int tab_adversaire[size*size];
  for(int i=0; i<size*size; i++)
    tab_adversaire[i]=0;
  is_connexe(genius.graph,colors,colors[genius.graph->positions[(genius.id+1)%2]],genius.graph->positions[(genius.id+1)%2],tab_adversaire);
  assert(tab_adversaire[80]==1);
  assert(tab_adversaire[71]==1);
  assert(tab_adversaire[62]==1);
  assert(tab_adversaire[70]==1);
  printf(" test_is_connexe: OK\n");

  make_tree(&genius,0, tree, 3);
  assert(tree_length(tree)==3);
  printf(" test_tree_length: OK\n");
  
  tree_extension(&genius, 0, tree, 2);
  assert(tree_length(tree)==5);
  printf(" test_tree_extension: OK\n");
  
  free_tree_aux(tree);
  gsl_spmatrix_uint_free(s->t);
  free(s);
}


int main(){
  test_color_to_string();
  test_array_copy();
  test_initialise();
  test_graph_square();
  test_donut();
  test_hache();
  test_genius_functions();
  return 0;
}
