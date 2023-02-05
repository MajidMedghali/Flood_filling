#ifndef _SERVER_FUNCTIONS_H_
#define _SERVER_FUNCTIONS_H_

#include <dlfcn.h>
#include <string.h>
#include "graph.h"
#include "player.h"
#include "initialise.h"

#define MAX_NUMBER_OF_MOVES 100


struct player_functions {
  void * player;
  char const *(* get_player_name)() ;
  void (* initialize)();
  struct move_t (*play)();
  void (* finalize)();
};

struct player_functions init_player_functions(char * path);

size_t is_winning(struct graph_t *graph, enum color_t *colors, size_t id, struct player_functions func[2],char type);

struct graph_t* init_graph(size_t n,char type);

int compute_next_player(int playing);
void display(enum color_t *t,int size,char type);
int forbiden_color(enum color_t color, struct color_set_t forbidden);
int verify_move(struct move_t move,
		enum color_t *colors, int id, struct color_set_t forbidden, int n);
struct color_set_t init_forbidden_colors(struct color_set_t previous, int num_forbidden_colors);

//void apply_move(int move);
#endif //_SERVER_FUNCTIONS_H_
