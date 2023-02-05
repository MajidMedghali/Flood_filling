#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include <math.h>
#include <time.h>
#include "initialise.h"


#define ANSI_COLOR_RED     "\e[41m"
#define ANSI_COLOR_GREEN   "\e[42m"
#define ANSI_COLOR_ORANGE  "\e[43m"
#define ANSI_COLOR_YELLOW  "\e[0;103m"
#define ANSI_COLOR_BLUE    "\e[44m"
#define ANSI_COLOR_PINK    "\e[0;105m"
#define ANSI_COLOR_CYAN    "\e[46m"
#define ANSI_COLOR_VIOLET  "\e[45m"
#define ANSI_COLOR_BLACK  "\e[40m"
#define ANSI_COLOR_RESET   "\e[0m"


void coloration_1(int size, int nbre_colors,enum color_t *t){
  printf("You chose algorithm of coloration n° 0 (random)\n");
   for(int i=0;i<size;i++){
    t[i]=rand()%nbre_colors;
  }
}


void coloration_2(int size, int nbre_colors,enum color_t *t){
  int taille=(int)sqrt(size);
  printf("You chose algorithm of coloration n° 1 (random with blocs)\n");
  for(int i=0;i<size;i++){
    t[i]=rand()%nbre_colors;
  }
  int nomber_de_blocs=rand()%(taille);
  for(int k=0;k<nomber_de_blocs;k++){
    int couleur_de_bloc=rand()%nbre_colors;
    int taille_ligne=rand()%(taille-3);
    int taille_colonne=rand()%(taille-3);
    int debut_ligne=rand()%taille;
    int debut_colonne=rand()%(taille-taille_ligne);
    int id=debut_colonne+debut_ligne*taille;
    for(int i=0;i<taille_ligne;i++){
      t[id+i]=couleur_de_bloc;
    }
    couleur_de_bloc=rand()%nbre_colors;
    debut_ligne=rand()%(taille-taille_colonne);
    debut_colonne=rand()%(taille);
    id=debut_colonne+debut_ligne*taille;
    for(int i=0;i<taille_colonne;i++){
      t[i*taille+id]=couleur_de_bloc;
    }
  }
}

//initialise the colors array
enum color_t* initialise(int size, int nbre_colors,size_t algo){
  enum color_t *t=malloc(sizeof(*t)*size);
  if(algo ==0){
    coloration_1(size,nbre_colors,t);
  }
  else{
    coloration_2(size,nbre_colors,t);
  }
  return t;
}


//display square graph
void afficher_square(enum color_t *t,int size){
  //  int n=sqrt(size);
  for(int i=0;i<size;i++){
      if(i%((int)sqrt(size))==0 && i!=0) printf("\n");
      if(*(t+i) == 1)
	printf(ANSI_COLOR_RED "  " ANSI_COLOR_RESET);
      if(*(t+i) == 2)
	printf(ANSI_COLOR_GREEN "  " ANSI_COLOR_RESET);
      if(*(t+i) == 3)
	printf(ANSI_COLOR_YELLOW "  " ANSI_COLOR_RESET);
      if(*(t+i) == 0)
	printf(ANSI_COLOR_BLUE "  " ANSI_COLOR_RESET);
      if(*(t+i) == 7)
	printf(ANSI_COLOR_PINK "  " ANSI_COLOR_RESET);
      if(*(t+i) == 6)
	printf(ANSI_COLOR_CYAN "  " ANSI_COLOR_RESET);
      if(*(t+i) == 5)
	printf(ANSI_COLOR_VIOLET "  " ANSI_COLOR_RESET);
      if(*(t+i) == 4)
	printf(ANSI_COLOR_ORANGE "  " ANSI_COLOR_RESET);
      if(*(t+i) == -1)
	printf(ANSI_COLOR_BLACK "  " ANSI_COLOR_RESET);
  }
  printf("\n");
}


//display donut graph
void afficher_donut(enum color_t *t,int size){
  int n=sqrt(size);
  for(int i=0;i<size;i++){
    if((i/n<n/3 || i%n<n/3 || i/n>=2*n/3 || i%n>=2*n/3)){
      if(i%((int)sqrt(size))==0 && i!=0) printf("\n");
      if(*(t+i) == 1)
	printf(ANSI_COLOR_RED "  " ANSI_COLOR_RESET);
      if(*(t+i) == 2)
	printf(ANSI_COLOR_GREEN "  " ANSI_COLOR_RESET);
      if(*(t+i) == 3)
	printf(ANSI_COLOR_YELLOW "  " ANSI_COLOR_RESET);
      if(*(t+i) == 0)
	printf(ANSI_COLOR_BLUE "  " ANSI_COLOR_RESET);
      if(*(t+i) == 7)
	printf(ANSI_COLOR_PINK "  " ANSI_COLOR_RESET);
      if(*(t+i) == 6)
	printf(ANSI_COLOR_CYAN "  " ANSI_COLOR_RESET);
      if(*(t+i) == 5)
	printf(ANSI_COLOR_VIOLET "  " ANSI_COLOR_RESET);
      if(*(t+i) == 4)
	printf(ANSI_COLOR_ORANGE "  " ANSI_COLOR_RESET);
    }
    else
      printf(ANSI_COLOR_RESET "  " ANSI_COLOR_RESET);
  }
  printf("\n");
}
  

//display hache graph
void afficher_hache(enum color_t *t,int size){
  int n=sqrt(size);
  for(int i=0;i<size;i++){
    if((i%n<n/3 ||((i%n<2*n/3 && i%n>=n/3)&&(i/n<2*n/3 &&i/n>=n/3)) || i%n>=2*n/3 )){
      if(i%((int)sqrt(size))==0 && i!=0) printf("\n");
      if(*(t+i) == 1)
	printf(ANSI_COLOR_RED "  " ANSI_COLOR_RESET);
      if(*(t+i) == 2)
	printf(ANSI_COLOR_GREEN "  " ANSI_COLOR_RESET);
      if(*(t+i) == 3)
	printf(ANSI_COLOR_YELLOW "  " ANSI_COLOR_RESET);
      if(*(t+i) == 0)
	printf(ANSI_COLOR_BLUE "  " ANSI_COLOR_RESET);
      if(*(t+i) == 7)
	printf(ANSI_COLOR_PINK "  " ANSI_COLOR_RESET);
      if(*(t+i) == 6)
	printf(ANSI_COLOR_CYAN "  " ANSI_COLOR_RESET);
      if(*(t+i) == 5)
	printf(ANSI_COLOR_VIOLET "  " ANSI_COLOR_RESET);
      if(*(t+i) == 4)
	printf(ANSI_COLOR_ORANGE "  " ANSI_COLOR_RESET);
      if(*(t+i) == -1)
	printf(ANSI_COLOR_BLACK "  " ANSI_COLOR_RESET);
    }
    else
      printf(ANSI_COLOR_RESET "  " ANSI_COLOR_RESET);
  }
   printf("\n");
}
  



//copy the colors arry and return a new one
enum color_t* array_copy(enum color_t* t,int size){
  enum color_t* m=malloc(sizeof(enum color_t)*size);
  for(int i=0;i<size;i++){
    m[i]=t[i];
  }
  return m;
}

