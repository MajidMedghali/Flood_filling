#include "server_functions.h"
#include "graph_type.h"
#include "graph_functions.h"


//fill a struct player_functions when linking the libraries
struct player_functions init_player_functions(char * path)
{
  struct player_functions this_player;
  this_player.player = dlopen(path,RTLD_LAZY);


  this_player.get_player_name = dlsym(this_player.player,"get_player_name");
  this_player.play = dlsym(this_player.player,"play");
  this_player.initialize = dlsym(this_player.player,"initialize");
  this_player.finalize = dlsym(this_player.player,"finalize");

   if ((this_player.player ==NULL || this_player.get_player_name==NULL|| this_player.play==NULL
       ||this_player.initialize==NULL||this_player.finalize==NULL))
    {
      printf("dlerror_get of one of the players functions\n");
      exit(-1);
    }
  else
    {
      printf("Functions Initilized\n");

      }
  return(this_player);
}


//return the winner in the end as the percentage of tiles conquered for each player
size_t is_winning(struct graph_t *graph, enum color_t *colors,
		  size_t id, struct player_functions player_funcs[2],char type){
  int n = (int)sqrt( graph->num_vertices+1);
  size_t id2=compute_next_player( id );
  //tmp contient le nombre des cases supprimés pour obtenir le graphe
  int tmp=0;
  switch(type){
  case 'c':
    tmp=0;
    break;
  case 'd':
    tmp=(n/3)*(n/3);
    break;
  case 'h':
  tmp=2*(n/3)*(n/3);
  break;
  }
  //on calcul le nombre de cases colorées par chaque joueur 
  int num_colors=count( graph, colors, 8, id+id*(n*n-2), graph->num_vertices );
  int num_colors2=count( graph, colors, 8, id2+id2*(n*n-2), graph->num_vertices );
  if(num_colors > num_colors2){
    //if player1 won
     printf( "%s has won with %ld%% of space against ", player_funcs[id].get_player_name(), num_colors*100/(graph->num_vertices-tmp) );
     printf( "%s with %ld%% of space\n", player_funcs[id2].get_player_name(), num_colors2*100/(graph->num_vertices-tmp) );
  }
  else if(num_colors2 > num_colors){
    //if player2 won
     printf( "%s has won with %ld%% of space against ", player_funcs[id2].get_player_name(), num_colors2*100/(graph->num_vertices-tmp) );
     printf( "%s with %ld%% of space\n", player_funcs[id].get_player_name(), num_colors*100/(graph->num_vertices-tmp) );
  }
  else{
    //if the game is a tie
     printf( "No one won the game both of them have %ld%% of space \n", num_colors*100/(graph->num_vertices-tmp) );
  }
  
  
  return 1;
}

//initialise the graph
struct graph_t* init_graph(size_t n,char type){
  if(type =='c'){
    return(square_csr((int)n));
  }
  else if (type == 'd'){
    return(donut_csr(n));
  }
   else if (type == 'h'){
    return(hache_csr(n));
  }
  /*
  else if (type == 't'){
    return(torique(m));
    }*/
  printf("this is not one of the option available\n");
  return NULL;
}


//display the graph
void display(enum color_t *t,int size,char type){
  if(type =='c'){
   afficher_square(t,size);
  }
  else if (type == 'd'){
    afficher_donut(t,size);
  }
  else if (type == 'h'){
    afficher_hache(t, size);
  }/*
  else if (type == 't'){
    return(torique(m));
    }*/
}



//initialize forbidden colors
struct color_set_t init_forbidden_colors(struct color_set_t previous, int num_colors_forbidden){
  struct color_set_t forbidden={.t={"-1"}};
    for (int i=0; i<num_colors_forbidden; i++){
	int color=rand()%MAX_COLOR;
	if(! forbiden_color(color, previous)  && ! forbiden_color(color, forbidden)){
	  forbidden.t[i]=(char *)color_to_string(color);
	  forbidden.t[i+1]="-1";
	}
	else
	  i--;
      }
  return forbidden;
}

//if it's a forbiden color return 1 0 else
int forbiden_color(enum color_t color, struct color_set_t forbidden){
  int i=0;
  char *tmp=forbidden.t[i];
 
  while(strcmp(tmp, "-1")!=0 && strcmp(tmp,color_to_string(color))!=0){
    tmp=forbidden.t[i];
    i++;
    
  }
  if(strcmp(tmp,color_to_string(color))==0)
    return 1;
  return 0;
}


//verify the moves if it returns 1 the move is good 0 else
int verify_move(struct move_t move,
		enum color_t *colors,
		int id,
		struct color_set_t forbidden, int n){

  if ( move.c == colors[id+id*(n*n-2)])
    return 0;
  if (forbiden_color(move.c, forbidden))
    return 0;
  return 1;
}


int compute_next_player(int playing){
  return abs(playing-1);
}


