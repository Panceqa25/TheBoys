#include <stdio.h>
#include <stdlib.h>

typedef struct lef {
	int evento;
	int idHeroi;
	int idBase;
	int idMissao;
	int horario;
	struct lef *prox;
}lef_t;


int criaEvento(lef_t **novo, int evento, int horario, int idHeroi, int idBase, int idMissao);

void insereEvento(lef_t **lista, lef_t **novo);

void removeEvento(lef_t **lista);

void liberaEventos(lef_t **lista);

void imprimeEventos(lef_t *lista);

	
