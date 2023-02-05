#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include <math.h>
#include <time.h>



//transform the color to string
const char* color_to_string(enum color_t c){ 
  const char* b;//malloc(sizeof(*b));
  b="BLUE";
  const char* r;//malloc(sizeof(*r));
  r="RED";
  const char* g;//=malloc(sizeof(*g));
  g="GREEN";
  const char* y;//=malloc(sizeof(*y));
  y="YELLOW";
  const char* o;//=malloc(sizeof(*o));
  o="ORANGE";
  const char* v;//=malloc(sizeof(*v));
  v="VIOLET";
  const char* C;//=malloc(sizeof(*C));
  C="CYAN";
  const char* p;//=malloc(sizeof(*p));
  p="PINK";
  switch((int)c){
  case 0:
    return b;
    break;
  case 1:
    return r;
    break;
  case 2:
    return g;
    break;
  case 3:
    return y;
    break;
  case 4:
    return o;
    break;
  case 5:
    return v;
    break;
  case 6:
    return C;
    break;
  case 7:
    return p;
    break;
  }
  return "No Color";
}

/*
 int main(int argc, char* argv[]){
    srand(time(NULL));
    enum color_t d=BLUE;
    printf("%s\n",color_to_string(d));
    //afficher(initialise(100),100);
    //color_to_string(c);


    return 0;
    }*/

