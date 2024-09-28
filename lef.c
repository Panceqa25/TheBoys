#include "lef.h"

//Cria um novo evento em um determinado horario
int criaEvento(lef_t **novo, int evento, int horario, int idHeroi, int idBase, int idMissao) {
	
	if ((*novo = (lef_t*) malloc(sizeof(lef_t))) == 0)
		return 0;

	(*novo)->idMissao = idMissao;
	(*novo)->idHeroi = idHeroi;
	(*novo)->idBase = idBase;
	(*novo)->evento = evento;
	(*novo)->horario = horario;
	(*novo)->prox = NULL;

	return 1;
}

//Insere o evento criado em maneira ordenada de acordo com o horario
void insereEvento(lef_t **lista, lef_t **novo) {

	lef_t *aux;
	aux = *lista;

	//Se a lista esta vazia, insere no inicio
	if ((*lista) == NULL) {
		(*lista) = (*novo);
		return;
	}

	//Confere se o numero deve ser inserido na primeira posicao
	if (aux != NULL) {
		if ((*lista)->horario > (*novo)->horario) {
			(*novo)->prox = (*lista);
			(*lista) = (*novo);
			return;
		}
	}

	//Busca a posicao que o numero deve estar, se encontrar, insere
	while (aux->prox != NULL) {// 
		if (aux->prox->horario >= (*novo)->horario) {
			(*novo)->prox = aux->prox;
			aux->prox = (*novo);
			return;
		}
		else
			aux = aux->prox;
	}

	//Se nao encontrou a posicao, insere no final
	aux->prox = (*novo);
}

void removeEvento(lef_t **lista) {
	
	lef_t *aux;
	aux = (*lista);

	//O segundo evento vira o comeco da lista
	(*lista) = aux->prox;

	//Libera o evento que custumava ser o primeiro
	free(aux);

}

void liberaEventos(lef_t **lista) {
	
	lef_t *aux;

	while ((*lista) != NULL) {
		aux = (*lista);
		(*lista) = (*lista)->prox;
		free(aux);
	}
}

void imprimeEventos(lef_t *lista) {
	
	while (lista != NULL) {
		if (lista->evento == 1) {
			printf("------------------\n");
			printf("Horario: %d\n", lista->horario);
			printf("Evento: %d\n", lista->evento);
			printf("Heroi: %d\n", lista->idHeroi);
			printf("Base: %d\n", lista->idBase);
			printf("------------------\n");
		}
		lista = lista->prox;
	}

}

