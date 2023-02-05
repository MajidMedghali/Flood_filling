#include <stdio.h>
#include <stdlib.h>
#include"graph.h"
#include"graph_type.h"

void  make_hole(struct graph_t *g, int id, int size){
  int vertices=g->num_vertices;
  int n=sqrt(vertices);
  for(int i=0; i<vertices; i++){
    for(int j=0;j<vertices ;j++){
      if( ((j%n>=id%n && j%n<(id+size)%n) &&//colonnes de j appartient au trou
	   (j/n>=id/n && j/n<id/n+size))){//ligne de j appartient au trou
	 
	  gsl_spmatrix_uint_set(g->t, i, j,0);
	  gsl_spmatrix_uint_set(g->t, j, i,0);
      }
    }
  }
}

struct graph_t* square_coo(int n){
  struct graph_t *g=malloc(sizeof(*g));
  g->num_vertices=n*n;
  g->positions[0]=0;
  g->positions[1]=g->num_vertices-1;
  g->t=gsl_spmatrix_uint_alloc(g->num_vertices, g->num_vertices);
  for(size_t  i=0; i<g->num_vertices; i++){
    for(size_t  j=0; j<g->num_vertices; j++){
      if(( (i/n==j/n) && (abs((int)(j-i))==1) ) //i et j sont en meme ligne et voisins droit au gauche
	 || abs((int)(j-i))==n) //voisins dessous ou dessus
	gsl_spmatrix_uint_set(g->t, i, j,1);
	
    }
  }
  return g;
}

struct graph_t* square_csr(int n){
  struct graph_t* c=square_coo(n);
  gsl_spmatrix_uint *csr=gsl_spmatrix_uint_compress(c->t, GSL_SPMATRIX_CSR);
  gsl_spmatrix_uint_free(c->t);
  c->t=csr;
  return c;
}

void gsl_copy(struct graph_t *g, struct graph_t *g1){
  for(size_t  i=0; i<g1->num_vertices; i++){
    for(size_t  j=0; j<g1->num_vertices; j++){
      size_t a=gsl_spmatrix_uint_get(g1->t, i, j);
      if(a==1)
	gsl_spmatrix_uint_set(g->t, i, j,1);
    }
  }
}


struct graph_t* donut_coo(int n){
  struct graph_t *g=square_coo(n);
  int ligne=n/3;
  int id=ligne*n+n/3;
  make_hole(g,id,n/3 );
  return g;  
}

struct graph_t* donut_csr(int n){
  struct graph_t *g=donut_coo(n);
  struct graph_t *g1=malloc(sizeof(*g1));
  g1->num_vertices=g->num_vertices;
  g1->positions[0]=0;
  g1->positions[1]=g->num_vertices-1;
  g1->t=gsl_spmatrix_uint_alloc(g1->num_vertices, g1->num_vertices);
  gsl_copy(g1, g);
  g1->t=gsl_spmatrix_uint_compress(g1->t, GSL_SPMATRIX_CSR);
  gsl_spmatrix_uint_free(g->t);
  free(g);
  return g1;
}

struct graph_t* hache_coo(int n){ 
  struct graph_t *g=square_coo(n);
  int id1=n/3;
  int ligne2=2*n/3;
  int id2=ligne2*n+n/3;
  make_hole(g,id1,n/3);
  make_hole(g,id2,n/3);
  return g;  
}

struct graph_t* hache_csr(int n){
  struct graph_t *h=hache_coo(n);
  struct graph_t *g1=malloc(sizeof(*g1));
  g1->num_vertices=h->num_vertices;
  g1->positions[0]=0;
  g1->positions[1]=h->num_vertices-1;
  g1->t=gsl_spmatrix_uint_alloc(g1->num_vertices, g1->num_vertices);
  gsl_copy(g1, h);
  g1->t=gsl_spmatrix_uint_compress(g1->t, GSL_SPMATRIX_CSR);
  gsl_spmatrix_uint_free(h->t);
  free(h);
  return g1;
}

void torique_link(struct graph_t* g){
  int lig_i, lig_j, col_i, col_j=0;
  int n=sqrt(g->num_vertices/2);
  for(int i=0; i<(int)g->num_vertices; i++){
    for(int j=0; j<(int)g->num_vertices; j++){
      lig_i=i/n;
      lig_j=j/n;
      col_i=i%n;
      col_j=j%n;
      if((lig_i==(2*n-1)-lig_j &&
	  (col_i==col_j && ((col_i==0) || (col_i==n-1)))) ||
	 (abs(lig_i-lig_j)==2*n-1 && col_j==col_i )){
	gsl_spmatrix_uint_set(g->t, i,j ,1);
      }
    }
  }
}

struct graph_t* torique_coo(int n){
  struct graph_t *g=malloc(sizeof(*g));
  int vertices=n*n;
  g->num_vertices=vertices*2;
  g->positions[0]=0;
  g->positions[1]=vertices/2-1;
  g->t=gsl_spmatrix_uint_alloc(g->num_vertices, g->num_vertices);
  for(size_t  i=0; i<g->t->size1; i++){
    for(size_t  j=0; j<g->t->size2; j++){
      if(( (i/n==j/n) && (abs((int)(j-i))==1) ) //i et j sont en meme ligne et voisins droit au gauche
	 || abs((int)(j-i))==n) //voisins dessous ou dessus
	gsl_spmatrix_uint_set(g->t, i, j,1);
    }
  }
  torique_link(g);
  return g;
}

struct graph_t* torique_csr(int n){
  struct graph_t* g=torique_coo(n);
  struct graph_t *g1=malloc(sizeof(*g1));
  g1->num_vertices=g->num_vertices;
  g1->positions[0]=0;
  g1->positions[1]=g->num_vertices/4-1;
  g1->t=gsl_spmatrix_uint_alloc(g->num_vertices, g->num_vertices);
  gsl_copy(g1, g);
  g1->t=gsl_spmatrix_uint_compress(g1->t, GSL_SPMATRIX_CSR);
  gsl_spmatrix_uint_free(g->t);
  free(g);
  return g1;
}


void print_csr(gsl_spmatrix_uint* csr){
  
  for (size_t i = 0; i < csr->size1; i++) {
    for (int k = csr->p[i]; k<csr->p[i+1]; k++) {
      unsigned int j = csr->i[k];
      printf(" %d", j);
    }
    printf("\n");
    } 
}

void print_coo(gsl_spmatrix_uint* coo){
  for(size_t i=0; i<coo->size1; i++){
    for(size_t j=0; j<coo->size2; j++){
      printf(" %d",gsl_spmatrix_uint_get(coo, i, j) );
    }
    printf("\n");
  }
}


/*
int main()
{
  int n=9;//nombre de colonnes
  struct graph_t *d=donut_csr(n);
  print_csr(d->t);
  printf("\n");
  return 0;
<<<<<<< HEAD
}*/

