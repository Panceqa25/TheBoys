#include <stdio.h>
#include <stdlib.h>

typedef struct nodeFila {
	int num;
	struct nodeFila *next;
}nodeFila_t;


int criaNoFila(nodeFila_t **novo, int num);

void insereFinal(nodeFila_t **fila, nodeFila_t **novo);

void removeInicio(nodeFila_t **fila, int *num);

void liberaFila(nodeFila_t **fila);

void imprimeFila(nodeFila_t *fila);




	
