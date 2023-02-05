#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "color.h"
#include <string.h>
#include <stdint.h>
#include <getopt.h>
#include "graph.h"
#include "server_functions.h"
#include "initialise.h"
#include "graph_functions.h"
#include"graph_type.h"
size_t board_size = 9;
size_t num_colors= 8;
size_t num_colors_forbidden= 0;
size_t seed=10;
size_t algo=0;
char type= 'c';


void parse_opts(int argc,char *argv[]){
  if(argc <2){
    printf("Missing arguments");
    exit(-1);
  }


  int opt;
  while ((opt = getopt(argc,argv, "m:t:c:a:f:s:")) != -1 ){
    switch (opt) {
    case 'm' :
      if (atoi(optarg) < 1 || (size_t) atoi(optarg) >SIZE_MAX){
        fprintf(stderr, "Usage: [-m size of the board] must be larger than 2 \n");
        exit(EXIT_FAILURE);
      }
      board_size=atoi(optarg);
      break;
    case 't' :
	
      if (strcmp(optarg,"c")!=0 && strcmp(optarg,"d")!=0 && strcmp(optarg,"h") && strcmp(optarg,"t")!=0){
        fprintf(stderr, "Usage: [-t type of the board] c for square ; t for toroidal ; h for a h form; d for donut \n");
        exit(EXIT_FAILURE);
      }

      type=*optarg;
      break;
    case 'c' :
      if (atoi(optarg) < 2 || (int) atoi(optarg) >MAX_COLOR-1){
        fprintf(stderr, "Usage: [-c number of colors] must be between 2 and MAX_COLOR-1 \n");
        exit(EXIT_FAILURE);
      }
      num_colors=atoi(optarg);
      break;
      
    case 'a' :
      if (atoi(optarg) < 0 ){
        fprintf(stderr, "Usage: [-a number of algorithm] must be bigger than 0 \n");
        exit(EXIT_FAILURE);
      }
      algo=atoi(optarg);
      break;
    case 's' :
      //initialisation du rand 
      seed=atoi(optarg);
      break;
    case 'f' :
      if (atoi(optarg) < 0 || (int) atoi(optarg) >MAX_COLOR-1){
	fprintf(stderr, "Usage: [-f number of colors forbidden] must be between 0 and MAX_COLOR-1 \n");
	exit(EXIT_FAILURE);
      }
      num_colors_forbidden=atoi(optarg);
      break;
      
    default :
      fprintf(stderr, "Usage: [-t type of the board] [-m size of the board] \n");
      exit(EXIT_FAILURE);
    }
  }
}





int main(int argc, char *argv[]){
  parse_opts(argc,argv);
  srand(seed);
  int n=board_size;//nombre de colonnes(ou lignes)
  struct graph_t* g=init_graph(n,type);
  enum color_t *colors;
  if(type=='t')
    colors=initialise(2*n*n, num_colors,algo/*,seed*/);
  else
    colors=initialise(n*n, num_colors,algo/*,seed*/);
  struct player_functions player_funcs[2];
  enum color_t *player_colors[2];
  struct color_set_t forbidden[2]={
				   {.t={"-1"}},
				   {.t={"-1"}}
  };
  printf("linking ...\n");
  for(int i = 0; i<2; i++){
    //initialisation des couleurs interdites pour les deux joueurs
    forbidden[i]=init_forbidden_colors(forbidden[(i+1)%2], num_colors_forbidden);
    printf("%s",argv[argc - i -1]);
    player_funcs[i]=init_player_functions(argv[argc - i -1]);
    player_colors[i]=array_copy( colors,n*n);
    player_funcs[i].initialize(i,init_graph(n,type),player_colors[i], forbidden);
    
    printf("%s\n",player_funcs[i].get_player_name());
    printf("Forbidden colors:");
    for(size_t j=0; j<num_colors_forbidden; j++){
      printf("%s ", forbidden[i].t[j]);
    }
    printf("\n");
  }
  
  //pour appeler une fonction dans player.c(initialize)pour le joueur i il faut appeler player_funcs[i].initialize(rhgrj) 
  
  int player=0;
  int comp=0;
  //pour vérifier si les deux move sont égaux à -1
  int last_move=0;
  struct move_t move={.c=-1};
  display(colors, n*n,type);
  printf("\n");
  while( comp < MAX_NUMBER_OF_MOVES ){
    
    player=compute_next_player(player);
    
    move=player_funcs[player].play(move);

    printf("%s chose %s\n",player_funcs[player].get_player_name()
	   , color_to_string(move.c));

    //verify the move is legal
    if(!verify_move(move,colors,player,forbidden[player],n)){
      printf("%s tried an illegal move the other player won \n",player_funcs[player].get_player_name());
      break;
    }

    //si le premier joueur a choisit un move # NO_COLOR et le deuxième a choisit NO_COLOR
    if(move.c==-1){
      if(last_move==1){
	is_winning(g, colors, (size_t) player, player_funcs,type);
	break;
      }
      last_move=1;
    }
   
    //si le premier joueur a choisit NO_COLOR et le deuxième a choisit un move valide # de NO_COLOR
    else
      last_move=0;
    

    if(move.c !=-1){
      color_graph(g, colors, player+player*(n*n-2),move.c,algo);
    }

    
    if(comp%2==1){
      display(colors, n*n,type); 
      printf("\n");
    }
    comp++;
  }
  
  //si on sort de la boucle et perssone ne gagne 
  if(comp==MAX_NUMBER_OF_MOVES)
    printf("No one won \n");

  free(colors);
  gsl_spmatrix_uint_free(g->t);
  free(g);
	       
  for(int i = 0; i<2; i++){
    //free(player_colors[i]);
    player_funcs[i].finalize();
  }
  for(int i = 0; i<2; i++){
    dlclose(player_funcs[i].player);
  }
  return 0;
  
}
