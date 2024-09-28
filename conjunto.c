#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int num;
	struct node *prox;
}node_t;


int criaNo(node_t **novo, int num) {
	
	//Se nao conseguir alocar memoria retorna 0
	if ((*novo = (node_t*) malloc(sizeof(node_t))) == 0)
		return 0;

	(*novo)->num = num;
	(*novo)->prox = NULL;

	return 1;
}

int removeNo(node_t **lista, int num) {
	node_t *aux1, *aux2;
	aux1 = (*lista);
	
	if (aux1 != NULL && aux1->num == num) {
		(*lista) = aux1->prox;
		free(aux1);
		return 1;
	}
	
	while (aux1 != NULL && aux1->num != num) {
		aux2 = aux1;
		aux1 = aux1->prox;
	}

	if (aux1 == NULL)
		return 0;

	aux2->prox = aux1->prox;

	free(aux1);

	return 1;
}

void removeRepetidos(node_t **lista) {
	node_t *aux;
	aux = (*lista);
	while (aux->prox != NULL) {//0 8 1 3 9 9
		if (!removeNo(&aux->prox,aux->num))
			aux = aux->prox;
	}
}

//Insere um no de maneira ordenada em uma lista
void insereNo(node_t **lista, node_t **novo) {
	
	node_t *aux;
	aux = *lista;

	//Se a lista esta vazia, insere no inicio
	if ((*lista) == NULL) {
		(*lista) = (*novo);
		return;
	}

	//Confere se o numero deve ser inserido na primeira posicao
	if ((*lista)->num > (*novo)->num) {
		(*novo)->prox = (*lista);
		(*lista) = (*novo);
		return;
	}

	//Busca a posicao que o numero deve estar, se encontrar, insere
	while (aux->prox != NULL) {// 3 4 5 5
		if (aux->num > (*novo)->num) {
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

int uniaoLista(node_t *lista1, node_t *lista2, node_t **uniao) {
	
	node_t *aux1, *aux2;

	//Copia a lista1 para a uniao
	aux1 = lista1;
	while (aux1 != NULL) {
		if (!criaNo(&aux2,aux1->num))
			return 0;
		insereNo(uniao,&aux2);
		aux1 = aux1->prox;
	}

	//Copia a lista2 para a uniao
	aux1 = lista2;
	while (aux1 != NULL) {
		if(!criaNo(&aux2,aux1->num))
			return 0;
		insereNo(uniao,&aux2);
		aux1 = aux1->prox;
	}

	removeRepetidos(uniao);

	return 1;
}

int intersecLista(node_t *lista1, node_t *lista2, node_t **intersec) {
	
	node_t *aux1, *aux2, *novo;
	aux1 = lista1;

	//Compara os elementos da lista1 com a lista2
	//se encontrar algum repetido, coloca ele na intersec
	while (aux1 != NULL) {
		aux2 = lista2;
		while (aux2 != NULL) {
			if (aux2->num == aux1->num) {
				if (!criaNo(&novo,aux2->num))
					return 0;
				insereNo(intersec,&novo);
			}
			aux2 = aux2->prox;
		}
		aux1 = aux1->prox;
	}

	removeRepetidos(intersec);
	
	return 1;
}

int existe(node_t *lista1, int num) {
	
	node_t *aux;
	aux = lista1;

	while (aux != NULL) {
		if (aux->num == num)
			return 1;
		aux = aux->prox;
	}

	return 0;
}

int igual(node_t *lista1, node_t *lista2) {
	
	node_t *aux1, *aux2;
	aux1 = lista1;
	aux2 = lista2;

	//Enquanto as listas sao iguais, continua percorrendo elas ate o fim
	while ((aux1 != NULL) && (aux2 != NULL)) {
		if (aux1->num != aux2->num)
			return 0;
		aux1 = aux1->prox;
		aux2 = aux2->prox;
	}

	//Se uma chegou no fim e a outra nao eh porque os tamanhos sao diferentes
	if ((aux1 != NULL) || (aux2 != NULL))
		return 0;
	return 1;
}

int contem(node_t *lista1, node_t *lista2) {
	
	int cont1 = 0, cont2 = 0;
	node_t *aux1, *aux2;
	aux2 = lista2;

	while (aux2 != NULL) {
		aux1 = lista1;
		while (aux1 != NULL) {
			if (aux1->num == aux2->num) {
				cont1++;
				break;
			}
			aux1 = aux1->prox;
		}
		cont2++;
		aux2 = aux2->prox;
	}

	if (cont1 == cont2)
		return 1;
	return 0;

	
}

void duplicado(node_t *lista) {
	
	int num, cont;
	node_t *aux1, *aux2;
	aux1 = lista;

	while (aux1 != NULL) {  
		num = aux1->num;
		aux2 = aux1->prox;
		cont = 0;
		while (aux2 != NULL) {
			if (aux2->num == num)
				cont++;
			aux2 = aux2->prox;
		}
		if (cont > 0)
			printf("O numero %d foi repetido %d vezes\n",aux1->num, cont);
		aux1 = aux1->prox;
	}
}

void imprimeLista(node_t *lista) {
	
	node_t *aux;
	aux = lista;
	while (aux != NULL) {
		printf("%d ", aux->num);
		aux = aux->prox;
	}
	printf("\n");
}  
/*void liberaLista(node_t **lista) {

	node_t *aux1, *aux2;
	aux1 = *lista;

	while (aux1->prox != NULL) {
		aux2 = aux1;
		aux1 = aux1->prox;
		free(aux2);
	}

	free(aux1);
}*/

void liberaLista(node_t **lista) {
	
	node_t *aux;

	while ((*lista) != NULL) {
		aux = (*lista);
		(*lista) = (*lista)->prox;
		free(aux);
	}
}

