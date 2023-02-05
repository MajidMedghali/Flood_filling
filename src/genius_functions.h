#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "graph.h"
#include "graph_functions.h"
#include "graph_type.h"
#include "initialise.h"


struct content{
  enum color_t *colors;
  int p;
  int s;
};

struct node_t{
  struct node_t* parent;
  struct node_t* children[8];
  struct content* c;
  int is_initialised;
};

struct player{
  char *name;
  size_t id;
  struct graph_t *graph ;
  enum color_t *colors;
  struct color_set_t forbidden;
};




void make_root(struct node_t *n, enum color_t* c);
void make_tree(struct player *player, int id
	       ,struct node_t* n, int depth);
struct node_t*  min_max(struct node_t* tree, int joueur, int depth); 
int tree_length(struct node_t *n);
void free_tree(struct node_t* n, int child);
void tree_extension(struct player* p, int id,
		    struct node_t* n , int depth);
void free_tree_aux(struct node_t* n);
void is_connexe(struct graph_t *graph, enum color_t *colors,
		enum color_t old_color,int id,int * tab);
void color_exist(struct graph_t *graph, enum color_t *colors,
		 enum color_t old_color,int id,int * tab);

