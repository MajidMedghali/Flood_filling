#ifndef _FLOOD_INITIALISE_H_
#define _FLOOD_INITIALISE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include <math.h>
#include <time.h>
enum color_t* initialise(int size, int nbre_colors,size_t algo);

void afficher_square(int* t,int size);
void afficher_donut(int* t,int size);
void afficher_hache(int* t,int size);

enum color_t* array_copy(enum color_t* t,int size);
#endif
