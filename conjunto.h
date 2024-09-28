#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int num;
	struct node *prox;
}node_t;


int criaNo(node_t **novo, int num);

void insereNo(node_t **lista, node_t **novo);

int removeNo(node_t **lista, int num);

void removeRepetidos(node_t **lista);

int uniaoLista(node_t *lista1, node_t *lista2, node_t **uniao);

int intersecLista(node_t *lista1, node_t *lista2, node_t **intersec);

int existe(node_t *lista1, int num);

int igual(node_t *lista1, node_t *lista2);

int contem(node_t *lista1, node_t *lista2);

void duplicado(node_t *lista);

void imprimeLista(node_t *lista);

void liberaLista(node_t **lista);
