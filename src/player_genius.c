#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include <time.h>
#include "graph.h"
#include "graph_functions.h"
#include "genius_functions.h"
#include "initialise.h"
#include "graph_type.h" 


#define NUM_PLAYERS 2

struct player genius;
struct node_t *n;


char const* get_player_name(){
  return genius.name;
}



void initialize(size_t id, struct graph_t* graph,enum color_t* colors,struct color_set_t forbidden[NUM_PLAYERS]){
  n=malloc(sizeof( *n));
  genius.name="MR_genius";
  genius.graph=graph;
  genius.id = id;
  genius.colors = colors;
  genius.forbidden = forbidden[id];
  make_root(n, genius.colors);
  printf(" initialize PASSED taille de l'arbre=%d \n ", tree_length(n));  

  }


struct move_t play(struct move_t previous_move){
  struct node_t* next_node;
  struct move_t m;
  //initialisation de l'arbre à partir de genius si c'est lui qui commence ou de l'adversaire si l'adversaire commence
  if(n->parent==NULL){
    if(previous_move.c==-1){//moi qui commence
      make_tree(&genius,genius.id,n,6);
    }
    else{
      // printf("smart a commencé\n");
      make_tree(&genius,(genius.id+1)%2,n,6);
    }
  }
  //si le joueur a joué avant genius, on avance dans l'arbre 
  if(previous_move.c!=-1 && tree_length(n)>1){
    graph_coloration(genius.graph,genius.colors,
		     genius.graph->positions[(genius.id+1)%2],previous_move.c);
    free_tree(n, previous_move.c);
    n=n->children[previous_move.c];
    free(n->parent);
  }
  next_node=min_max(n, 1, 6);
  //on renvoie No_Color si le noeud renvoyé par min_max est NULL
  if(next_node==NULL){
    m.c=-1;
  }

  //sinon on avance dans l'arbre en appliquant la transition renvoyé par min_max et on etend l'arbre de 2 profondeurs
  else {
    int s=0;
    //on choisit le noeud dont la valeur du champ n->c->s est le plus grand c-à-d on choisit le move qui lui permetre de gangner plus de cases
    for(int i=0; i<8; i++){
      if(n->children[i]!=NULL && n->children[i]->is_initialised==1){
	//si le move choisit va prendre les cases de la dversaire on le choisit pas
	if(s<n->children[i]->c->s && n->children[i]->c->p!=50){
	  s=n->children[i]->c->s;
	  next_node=n->children[i];
	}
      }
    }
    
    if(s!=0){//si un move st choisit
      int position=genius.graph->positions[genius.id];
      enum color_t next=next_node->c->colors[position];
      free_tree(n, next);
      n=(n->children[next]);
      free(n->parent);
      m.c=next;
      graph_coloration(genius.graph,genius.colors,
		       genius.graph->positions[genius.id],m.c);
      tree_extension(&genius, (genius.id+1)%2, n, 2);
    }
    else
      m.c=-1;//si aucun move n'est choisit on ne fait passer notre tour
  }

  
  return m;
}

void free_table(struct graph_t * G){
  gsl_spmatrix_uint_free(G->t);
  free(G);
}

void finalize(){
  free_table(genius.graph);
  free(genius.colors);
  free_tree_aux(n);
  printf("finalize PASSED\n");
}

