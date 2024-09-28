#include "filaEncadeada.h"

int criaNoFila(nodeFila_t **novo, int num) {
	
	if ((*novo = (nodeFila_t*) malloc(sizeof(nodeFila_t))) == 0)
		return 0;

	(*novo)->num = num;
	(*novo)->next = NULL;

	return 1;

}

void insereFinal(nodeFila_t **fila, nodeFila_t **novo) {
	
	nodeFila_t *aux;
	aux = (*fila);

	//Caso a fila esteja vazia, insere na primeira posicao
	if (aux == NULL) {
		(*fila) = (*novo);
		return;
	}

	//Caso o contrario, percorre ate o fim da fila e insere
	while (aux->next != NULL)
		aux = aux->next;
	aux->next = (*novo);

}

void removeInicio(nodeFila_t **fila, int *num) {
	
	nodeFila_t *aux;
	aux = (*fila);

	//Guarda o numero que estava na primeira posicao
	(*num) = aux->num;

	//O segundo evento vira o comeco da lista
	(*fila) = aux->next;

	//Libera o evento que custumava ser o primeiro
	free(aux);


}

void liberaFila(nodeFila_t **fila) {

	nodeFila_t *aux;

	while ((*fila) != NULL) {
		aux = (*fila);
		(*fila) = (*fila)->next;
		free(aux);
	}
}

void imprimeFila(nodeFila_t *fila) {

	while (fila != NULL) {
		printf("%d ", fila->num);
		fila = fila->next;	
	}

	printf("\n");

}
