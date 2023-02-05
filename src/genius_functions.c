#include"genius_functions.h"

//initialise la racine de l'arbre
void make_root(struct node_t* n,enum color_t *c  ){
  n->parent=NULL;
  n->c=malloc(sizeof(*n->c));
  n->is_initialised=1;
  n->c->colors=c;
  for(int i=0; i<8 ;i++){
    n->children[i]=NULL;
  }
}

//initialise un noeud 
void initialise_child(struct node_t* n, enum color_t c,
		      struct player *player,int id){
  int position=player->graph->positions[id];
  int position_adversaire=player->graph->positions[(id+1)%2];
  int size=player->graph->num_vertices;
  n->c=malloc(sizeof(*n->c));
  //pour eviter d'initialiser un enfant plusieurs fois dans make_tree et pour savoir s'il a ete initialise ou pas
  n->is_initialised=1;
  n->c->colors=malloc(sizeof(*n->c->colors)*size);
  for(int i=0; i<size; i++)
    n->c->colors[i]=n->parent->c->colors[i];
  graph_coloration(player->graph,n->c->colors,position,c );
  //p est le pourcentage des cases conquises par le joueur parmi les cases conquises des deux joueurs
  int a=count(player->graph,n->c->colors,n->c->colors[position],position,size);
  n->c->p=(a*100)/
	    (a+count(player->graph,n->c->colors,n->c->colors[position_adversaire],position_adversaire,size));
  for(int i=0; i<8 ;i++){
    n->children[i]=NULL;
  }
}
/*
int heuristique(struct node_t* n, struct player *p, int size){
  n->c->p=count(player->graph,n->c->colors,7,position,size)*100/
    (count(player->graph,n->c->colors,7,position,size)+
    count(player->graph,n->c->colors,7,position_adversaire,size));
}
*/
void color_exist(struct graph_t *graph, enum color_t *colors,
		 enum color_t old_color,int id,int * tab){
  size_t n = graph->num_vertices ;
  if((int)id>=0 && (int)id<(int)n && colors[id] == old_color && tab[id]!=1 )  {
    tab[id]=1;
    for (unsigned int k = graph->t->p[id]; (int)k < graph->t->p[id+1]; k++) {
      unsigned int j = graph->t->i[k];
      color_exist(graph,colors,old_color,j,tab);
    }
  }
  //quand la couleur de id != de celle des cases du joueur on l'initialise à 2
  else if(colors[id]!=old_color)
    tab[id]=2;
}





void is_connexe(struct graph_t *graph, enum color_t *colors,
	       enum color_t old_color,int id,int * tab){
  size_t n = graph->num_vertices ;
  if((int)id>=0 && (int)id<(int)n && colors[id] == old_color
                && tab[id]!=1 )  {
    tab[id]=1;
    for (unsigned int k = graph->t->p[id]; (int)k < graph->t->p[id+1]; k++) {
      unsigned int j = graph->t->i[k];
      is_connexe(graph,colors,old_color,j,tab);
    }
  }
}



void make_tree(struct player *player, int id,
	       struct node_t* n, int depth){
  /* //les enfants des feuilles sont NULL si l'arbre ne contient q'un noeud
  if(depth==1){
    for(int i=0;i<8; i++){
      free(n->children[i]);
      n->children[i]=NULL;
    }
    }*/
  if(depth>1){
    //printf("profondeur: %d\n",3-depth );
    int size=player->graph->num_vertices;
    int id_adversaire=(id+1)%2;
    int position_adversaire=player->graph->positions[id_adversaire];
    int position=player->graph->positions[id];
    enum color_t *colors=n->c->colors;
    int* couleurs_voisins=malloc(sizeof(int)*size);
    int* cases_adversaires=malloc(sizeof(int)*size);;
    for(int i=0; i<size; i++){
      couleurs_voisins[i]=0;
      cases_adversaires[i]=0;
    }
    //remplace dans couleurs_voisines les cases voisines(ou connexe) du joueur player1 par 2 (comme ca on sait les couleurs de children)
    color_exist(player->graph,colors,colors[position],
		position,couleurs_voisins);
    is_connexe(player->graph,colors,colors[position_adversaire],
		position_adversaire,cases_adversaires);
    for(int i=0; i<size; i++){
      if(n->children[colors[i]]==NULL){
	n->children[colors[i]]=malloc(sizeof(struct node_t));
	n->children[colors[i]]->is_initialised=0;
	n->children[colors[i]]->parent=n;
      }
      if(couleurs_voisins[i]==2 && cases_adversaires[i]!=1 &&
	 n->children[colors[i]]->is_initialised!=1){
	// initialise l'enfant correspondant à la transition de couleur colors[i] une seule fois en initialisant is_initialised à 1
	initialise_child(n->children[colors[i]], colors[i],
			 player, id);
	/*afficher_square(n->children[colors[i]]->c->colors, 144);
	  printf("\n");*/
      }
     
    }
    free(couleurs_voisins);
    free(cases_adversaires);
    for(int i=0; i<8; i++){
      if(n->children[i]!=NULL && n->children[i]->is_initialised==1){
	//on rappelle la fonction aux enfant de player1 mais en inversant player1 et player2 (puisque c'est l'autre joueur qui va jouer)et on diminuant depth 
	make_tree(player, (id+1)%2,n->children[i],depth-1);
      }
    }
  }
}

//noeud qui n'est pas une feuille de l'arbre mais qui n'a pas d'enfant car il n'y a plus de transitions possibles
int is_final_node(struct node_t* n){
  int c=0;
  for(int i=0; i<8; i++){
    if(n->children[i]==NULL || (n->children[i]!=NULL && n->children[i]->is_initialised==0)){
      c++;
    }
  }
  if(c==8)
    return 1;
  return 0;
}

int tree_length_aux(struct node_t* n, int length){
  if(n==NULL)
    return length;
  if(n->children[0]==NULL || is_final_node(n)==1 )
    return length+1;
  for(int i=0; i<8; i++){
    if( n->children[i]!=NULL && n->children[i]->is_initialised==1){
      return tree_length_aux(n->children[i], length+1);
    }
  }
  return length;
}

int tree_length(struct node_t* n){
  return tree_length_aux(n, 0); 
}


struct node_t* min_max(struct node_t* tree, int joueur, int depth){
  if(is_final_node(tree) ){
    tree->c->s=tree->c->p;
    return NULL;
    }
  if(depth==0/*||tree_length(tree)==1*/ ){
    tree->c->s=tree->c->p;
    return tree;
  }
  struct node_t* next=tree;
  if(joueur==1){
    int value=-1;
    int m=-1;
    for(int i=0; i<8; i++){
      if( tree->children[i]!=NULL && tree->children[i]->is_initialised==1
	  && !is_final_node(tree->children[i])){
	m= min_max(tree->children[i],0,depth-1)->c->p;
	//tree->children[i]->c->s=m;
      }
      else if( tree->children[i]!=NULL && tree->children[i]->is_initialised==1
	       && is_final_node(tree->children[i])){
	m=tree->children[i]->c->p;
	tree->children[i]->c->s=m;
      }
      if(value<m){
	value=m;
	next=tree->children[i];
	tree->c->s=value;
      }
    }
  }
  else{
    int value=100000000;
    int s=value;
    for(int i=0; i<8; i++){
      if(tree->children[i]!=NULL && tree->children[i]->is_initialised==1
	 && !is_final_node(tree->children[i]) ){
	s=min_max(tree->children[i],1,depth-1)->c->p;
      }
      else if(tree->children[i]!=NULL && tree->children[i]->is_initialised==1
	      && is_final_node(tree->children[i]) ){
	s=tree->children[i]->c->p;
	tree->children[i]->c->s=s;
      }
      if( value>s){
	value=s;
	next=tree->children[i];
	tree->c->s=value;
      }
    }
  }
  return next;
}

void tree_extension(struct player* p, int id,
		    struct node_t* n , int depth){
  if(tree_length(n)==1){
    for(int i=0; i<8; i++){
      if(n->children[i]==NULL){
	n->children[i]=malloc(sizeof(*n));
	n->children[i]->is_initialised=0;
	n->children[i]->parent=n;
      }
    }
    make_tree(p, id, n, depth+1);
  }
  else{
    for(int i=0; i<8; i++){
      if(n->children[i]!=NULL && n->children[i]->is_initialised==1)
	tree_extension(p, (id+1)%2, n->children[i], depth);
    }
  }
}

void free_tree_aux(struct node_t* n){
  for(int i=0; i<8; i++){
    if(n->children[i]!=NULL && n->children[i]->is_initialised==1)
      free_tree_aux(n->children[i]);
    else if (n->children[i]!=NULL && n->children[i]->is_initialised==0){
      free(n->children[i]);
      n->children[i]=NULL;
    }
  }
  free(n->c->colors);
  free(n->c);
  free(n);
  n=NULL;
}

void free_tree(struct node_t* n, int child){
  for(int i=0; i<8; i++){
    if(n->children[i]!=NULL && n->children[i]->is_initialised==1 && i!=child)
      free_tree_aux(n->children[i]);
    else if (n->children[i]!=NULL && n->children[i]->is_initialised==0 && i!=child){
      /*for(int j=0; j<8; j++){
	printf("p%d:        %d\n", j, n->children[i]->children[j]==NULL);
	}*/
      free(n->children[i]);
      n->children[i]=NULL;
    }
  }
  if(n->parent!=NULL){
    free(n->c->colors);
  }
  free(n->c);
  n->c=NULL;
}
