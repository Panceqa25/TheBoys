#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "conjunto.h"
#include "lef.h"
#include "filaEncadeada.h"

#define CHEGA 0
#define ESPERA 1
#define DESISTE 2
#define AVISA 3
#define ENTRA 4
#define SAI 5
#define VIAJA 6
#define MISSAO 7
#define FIM 8

typedef struct heroi {
	node_t *habilidades;
	int id;
	int paciencia;
	int velocidade;
	int experiencia;
	int idBase;
}heroi_t;

typedef struct base {
	int id;
	int lotacao;
	node_t *presentes;
	nodeFila_t *espera;
	int qntdPresente;
	int qntdEspera;
	int x;
	int y;
}base_t;

typedef struct missao {
	int id;
	int x;
	int y;
	node_t *habilidades;
}missao_t;

typedef struct mundo {
	int numHerois;
	heroi_t *herois;
	int numBases;
	base_t *bases;
	int numMissoes;
	missao_t *missoes;
	int numHabilidades;
	int missoesCumpridas;
	int tamanhoMundo;
	int relogio;
	int tempoFinal;
}mundo_t;

//Inicializa o estado inicial do mundo
int inicializaMundo(mundo_t *mundo) {

	mundo->relogio = 0;
	mundo->tempoFinal = 525600;
	mundo->tamanhoMundo = 20000;
	mundo->numHabilidades = 10;
	mundo->missoesCumpridas = 0;
	mundo->numHerois = mundo->numHabilidades*5;
	mundo->numBases = mundo->numHerois/6;
	mundo->numMissoes = mundo->tempoFinal/100;

	//Aloca memoria para os vetores presentes na struct mundo_t
	if ((mundo->herois = (heroi_t*) malloc(mundo->numHerois*sizeof(heroi_t))) == NULL)
		return 0;
	if ((mundo->bases = (base_t*) malloc(mundo->numBases*sizeof(base_t))) == NULL)
		return 0;
	if ((mundo->missoes = (missao_t*) malloc(mundo->numMissoes*sizeof(missao_t))) == NULL)
		return 0;

	return 1;
}


//Inicializa o estado inicial de um heroi
int inicializaHeroi(heroi_t *heroi, int id) {

	int qntdHabilidades = (rand() % 3) + 1; //Num aleatorio [1..3]
	node_t *novo;
	heroi->id = id;
	heroi->experiencia = 0;
	heroi->paciencia = (rand() % 101);
	heroi->velocidade = (rand() % 4951) + 50;
	heroi->habilidades = NULL;

	//Insere qntdHabilidades aleatorias no conjunto heroi->habilidades
	for (int i = 0; i < qntdHabilidades; i++) {
		if (!criaNo(&novo,rand() % 10))
			return 0;
		insereNo(&heroi->habilidades,&novo);
	}

	removeRepetidos(&heroi->habilidades);

	return 1;
}

//Inicializa o estado inicial de uma base
void inicializaBase(base_t *base, int id) {

	base->id = id;
	base->x = (rand() % 20000);
	base->y = (rand() % 20000);
	base->lotacao = (rand() % 8) + 3; //Num aleatorio [3..10]
	base->qntdPresente = 0;
	base->qntdEspera = 0;
	base->presentes = NULL;
	base->espera = NULL;	
	
}

//Inicializa o estado inicial de uma missao
int inicializaMissao(missao_t *missao, int id) {

	int qntdHabilidades = (rand() % 5) + 6; //Num aleatorio [6..10]
	node_t *novo = NULL;
	missao->id = id;
	missao->x = (rand() % 20000);
	missao->y = (rand() % 20000);
	missao->habilidades = NULL;

	//Insere qntdHabilidades aleatorias no conjunto missao->habilidades
	for (int i = 0; i < qntdHabilidades; i++) {
		if (!criaNo(&novo,rand() % 10))
			return 0;
		insereNo(&missao->habilidades,&novo);
	}

	//Remove quaisquer habilidades repetidas
	removeRepetidos(&missao->habilidades);

	return 1;
}

int eventoChega(heroi_t *heroi, base_t base, lef_t **lef, int tempo) {

	int espera = 0;
	lef_t *novo;

	//Heroi passa a indicar que esta nessa base
	heroi->idBase = base.id;

	//Se a base nao esta lotada e a fila de espera esta vazia,
	//ou o heroi tem paciencia o suficiente, espera vale 1
	if ((base.qntdPresente < base.lotacao) && (base.qntdEspera == 0))
		espera = 1;
	else if (heroi->paciencia > 10*base.qntdEspera)
		espera = 1;

	//Se espera vale 1, cria o evento ESPERA, se nao, cria o evento DESISTE
	if (espera) {
		if (!criaEvento(&novo,ESPERA,tempo,heroi->id,base.id,-1))
			return 0;
		insereEvento(lef,&novo);
		printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n", tempo, heroi->id, base.id, base.qntdPresente, base.lotacao);
	}
	else {
		if (!criaEvento(&novo,DESISTE,tempo,heroi->id,base.id,-1))
			return 0;
		insereEvento(lef,&novo);
		printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n", tempo, heroi->id, base.id, base.qntdPresente, base.lotacao);
	}

	return 1;
}

int eventoEspera(heroi_t *heroi, base_t *base, lef_t **lef, int tempo) {
	nodeFila_t *novoFila;
	lef_t *novoLef;

	//Insere heroi no fim da fila de espera da base
	if (!criaNoFila(&novoFila,heroi->id))
		return 0;
	insereFinal(&base->espera,&novoFila);
	printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, heroi->id, base->id, base->qntdEspera);
	
	base->qntdEspera++; //Incrementa o tamanho da fila de espera

	//Cria evento AVISA para o tempo atual
	if(!criaEvento(&novoLef,AVISA,tempo,heroi->id,base->id,-1))
		return 0;
	insereEvento(lef,&novoLef);

	return 1;
}

int eventoDesiste(heroi_t *heroi, lef_t **lef, int tempo) {
	lef_t *novoLef;
	int novaBase = (rand() % 8); //Num aleatorio [0..7]

	printf("%6d: DESIST HEROI %2d BASE %d\n", tempo, heroi->id, heroi->idBase);

	//Cria o evento VIAJA 
	if (!criaEvento(&novoLef,VIAJA,tempo,heroi->id,novaBase,-1))
		return 0;
	insereEvento(lef,&novoLef);

	return 1;
}

int eventoAvisa(base_t *base, lef_t **lef, int tempo) {
	int idHeroi;
	node_t *novo;
	lef_t *novoLef;

	printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA: ", tempo, base->id, base->qntdPresente, base->lotacao);
	imprimeFila(base->espera);

	//Enquanto a base nao estiver cheia e a lista de espera nao estiver vazia,
	//remove um heroi da fila de espera, e coloca ele na base
	while ((base->qntdPresente < base->lotacao) && (base->qntdEspera != 0)) {

		//Remove heroi no comeco da lista
		removeInicio(&base->espera,&idHeroi);
		base->qntdEspera--; //Decrementa o tamanho da fila de espera

		//Insere heroi na base
		if (!criaNo(&novo,idHeroi))
			return 0;
		insereNo(&base->presentes,&novo);
		base->qntdPresente++; //Incrementa a quantidade de herois presentes na base

		printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", tempo, base->id, idHeroi);

		//Cria evento ENTRA
		if(!criaEvento(&novoLef,ENTRA,tempo,idHeroi,base->id,-1))
			return 0;
		insereEvento(lef,&novoLef);
	}

	return 1;
}

int eventoEntra(heroi_t heroi, base_t base, lef_t **lef, int tempo) {
	int TPB;
	lef_t *novoLef;

	//Calcula o tempo de permanencia do heroi na base
	TPB = 15 + heroi.paciencia*((rand() % 20) + 1);
	printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", tempo, heroi.id, base.id, base.qntdPresente, base.lotacao, tempo+TPB);

	//Cria o evento SAI
	if(!criaEvento(&novoLef,SAI,tempo+TPB,heroi.id,base.id,-1))
		return 0;
	insereEvento(lef,&novoLef);

	return 1;
}

int eventoSai(heroi_t heroi, base_t *base, lef_t **lef, int tempo) {
	lef_t *novoLef;
	int novaBase = (rand() % 8); //Num aleatorio [0..7]
	
	removeNo(&base->presentes,heroi.id);
	base->qntdPresente--; //Decrementa a quantidade de herois presentes na base

	printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", tempo, heroi.id, base->id, base->qntdPresente, base->lotacao);

	//Cria evento VIAJA
	if(!criaEvento(&novoLef,VIAJA,tempo,heroi.id,novaBase,-1))
		return 0;
	insereEvento(lef,&novoLef);

	//Cria evento AVISA
	if (!criaEvento(&novoLef,AVISA,tempo,-1,base->id,-1))
		return 0;
	insereEvento(lef,&novoLef);

	return 1;
}

int eventoViaja(mundo_t mundo, heroi_t heroi, base_t destino, lef_t **lef, int tempo) {
	int x,y,distancia,duracao;
	lef_t *novoLef;

	//Teorema de pitagoras para calcular a distancia cartesiana entre as bases
	x = mundo.bases[heroi.idBase].x - destino.x;
	y = mundo.bases[heroi.idBase].y - destino.y;
	distancia = (int) sqrt(x*x + y*y);
	//Formula da velocidade media para calcular o tempo de viagem
	duracao = distancia/heroi.velocidade;


	printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", 
			tempo, heroi.id, heroi.idBase, destino.id, distancia, heroi.velocidade, tempo+duracao);

	//Cria evento CHEGA
	if (!criaEvento(&novoLef,CHEGA,tempo+duracao,heroi.id,destino.id,-1))
		return 0;
	insereEvento(lef,&novoLef);

	return 1;
}



int eventoMissao(mundo_t *mundo, missao_t missao, lef_t **lef, int tempo) {
	int menor = 30000, x, y, BMP = -1;
	int *v;
	node_t *aux1, *aux2, *aux3;
	node_t *uniao = NULL;
	lef_t *novoLef;
	
	printf("%6d: MISSAO %d HAB REQ: ", tempo, missao.id);
	imprimeLista(missao.habilidades);

	//Aloca um vetor temporario para armazenar a distancia da missao a todas as bases
	if ((v = (int*) malloc(mundo->numBases*sizeof(int))) == NULL)
		return 0;

	//Calcula a distancia da missao a todas as bases e insere os resultados em um vetor
	for (int i = 0; i < mundo->numBases; i++) {
		if (mundo->bases[i].presentes != NULL) {
			x = missao.x - mundo->bases[i].x;
			y = missao.y - mundo->bases[i].y;
			v[i] = (int) sqrt(x*x + y*y);
		}
		//Se a base nao tem nenhum heroi presente, sua distancia nao eh calculada
		//e, ao inves da distancia, -1 eh inserido no vetor
		else
			v[i] = -1;
	}
	//Faz a uniao do conjunto de habilidades de todos os herois presentes em cada base
	for (int i = 0; i < mundo->numBases; i++) {// a b c d
		uniao = NULL;
		aux1 = mundo->bases[i].presentes;
		if (aux1 != NULL) {
			aux2 = mundo->herois[aux1->num].habilidades;
			aux1 = aux1->prox;
			while (aux1 != NULL) {
				aux3 = mundo->herois[aux1->num].habilidades;
				uniaoLista(aux2,aux3,&uniao);
				aux1 = aux1->prox;
			}
			//Caso a base nao tenha as habilidades necessarias, sua posicao no vetor vale -1
			if (!contem(uniao,missao.habilidades))
				v[i] = -1;
		}
		if (uniao != NULL)
			liberaLista(&uniao);
	}


	//Busca o menor valor diferente de -1 no vetor, pois essa sera a base mais proxima cujo
	//os herois possuem as habilidades necessarias
	for (int i = 0; i < mundo->numBases; i++)
		if ((v[i] != -1) && (v[i] < menor)) {
			menor = v[i];
			BMP = i;
		}
	
	//mundo->tentativasMissoes++;

	//Caso haja uma BMP, incrementa a xp dos herois
	if ((BMP != -1) && (menor != 30000)) {
		printf("%6d: MISSAO %d CUMPRIDA BASE %d HEROIS: ", tempo, missao.id, mundo->bases[BMP].id);
		aux1 = mundo->bases[BMP].presentes;
		imprimeLista(aux1);
		mundo->missoesCumpridas++; //Incrementa a quantidade de missoes cumpridas
		while (aux1 != NULL) {
			mundo->herois[aux1->num].experiencia++;
			aux1 = aux1->prox;
		}
	}
	//Caso contrario, agenda essa missao para o dia seguinte
	else {
		printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, missao.id);
		criaEvento(&novoLef,MISSAO,tempo+(24*60),-1,-1,missao.id);
		insereEvento(lef,&novoLef);
	}

	free(v);
	if (uniao != NULL)
		liberaLista(&uniao);
	return 1;
}

void eventoFim(mundo_t mundo, int tentativas, int tempo) {
	float media = 0, porcentagem = 0;

	printf("%6d: FIM\n", tempo);
	for (int i = 0; i < mundo.numHerois; i++) {
		printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS: ", i, mundo.herois[i].paciencia, mundo.herois[i].velocidade, mundo.herois[i].experiencia);
		imprimeLista(mundo.herois[i].habilidades);
	}

	//Caso algum dos divisores seja 0, nao realiza as contas
	if (mundo.missoesCumpridas != 0)
		media = (float) tentativas/mundo.missoesCumpridas;
	if (mundo.numMissoes != 0)
		porcentagem = (float) mundo.missoesCumpridas*100/mundo.numMissoes;
	
	printf("%d/%d MISSOES CUMPRIDAS (%.2f%%), MEDIA: %.2f TENTATIVAS/MISSAO\n", mundo.missoesCumpridas, mundo.numMissoes,
			porcentagem, media);



}

int main() {
	srand(0);
	mundo_t mundo;
	int base = 0, tempo = 0, evento = 0, tentativas = 0;
	lef_t *lef, *novoEvento, *aux;

	//Inicializa o estado inicial do mundo
	if (!inicializaMundo(&mundo)) {
		printf("Falha. Nao foi possivel inicializar o mundo!\n");
		return 1;
	}
	lef = NULL;

	//Inicializa todos os herois e cria eventos para eles chegarem em bases aleatorias em tempos aleatorios
	for (int i = 0; i < mundo.numHerois; i++) {
		if (!inicializaHeroi(&mundo.herois[i],i)) {
			printf("Falha. Nao foi possivel inicializar o heroi %d!\n", i);
			return 1;
		}
		base = (rand() % mundo.numBases);
		tempo = (rand() % 4321);
		if (!criaEvento(&novoEvento, CHEGA, tempo, i, base, -1)) {
			printf("Falha. Nao foi possivel criar o evento CHEGA!\n");
			return 1;
		}
		insereEvento(&lef,&novoEvento);
	}
	
	//Inicializa todas as bases
	for (int i = 0; i < mundo.numBases; i++) {
		inicializaBase(&mundo.bases[i],i);

	}

	//Inicializa todas as missoes e gera um tempo aleatorio para que elas acontecam
	for (int i = 0; i < mundo.numMissoes; i++) {
		if (!inicializaMissao(&mundo.missoes[i],i)) {
			printf("Falha. Nao foi possivel inicializar a missao %d!\n", i);
			return 1;
		}
		tempo = (rand() % (mundo.tempoFinal+1));
		if (!criaEvento(&novoEvento, MISSAO, tempo, -1, -1, i)) {
			printf("Falha. Nao foi possivel criar o evento MISSAO!\n");
			return 1;
		}
		insereEvento(&lef,&novoEvento);
	}

	//Cria o evento final
	if (!criaEvento(&novoEvento, FIM, mundo.tempoFinal, -1, -1, -1)) {
		printf("Falha. Nao foi possivel criar o evento FIM!\n");
		return 1;
	}
	insereEvento(&lef,&novoEvento);

	aux = lef;
	//Loop que sera executado ate chegar no FIM
	while (evento != FIM) {
		evento = aux->evento;
		tempo = aux->horario;
		switch (evento) {
			//Evento Chega
			case CHEGA:
				if (!eventoChega(&mundo.herois[aux->idHeroi],mundo.bases[aux->idBase],&lef,tempo)) {
					printf("Falha. Nao foi possivel completar o evento CHEGA\n");
					return 1;
				}
				aux = aux->prox;
				removeEvento(&lef);
				break;
			//Evento Espera
			case ESPERA:
				if (!eventoEspera(&mundo.herois[aux->idHeroi],&mundo.bases[aux->idBase],&lef,tempo)) {
					printf("Falha. Nao foi possivel completar o evento ESPERA!\n");
					return 1;
				}
				aux = aux->prox;
				removeEvento(&lef);
				break;
			//Evento Desiste
			case DESISTE:
				if (!eventoDesiste(&mundo.herois[aux->idHeroi],&lef,tempo)) {
					printf("Falha. Nao foi possivel completar o evento DESISTE!\n");
					return 1;
				}
				aux = aux->prox;
				removeEvento(&lef);
				break;
			//Evento Avisa
			case AVISA:
				if (!eventoAvisa(&mundo.bases[aux->idBase],&lef,tempo)) {
					printf("Falha. Nao foi possivel completar o evento AVISA!\n");
					return 1;
				}
				aux = aux->prox;
				removeEvento(&lef);
				break;
			//Evento Entra
			case ENTRA:
				if (!eventoEntra(mundo.herois[aux->idHeroi],mundo.bases[aux->idBase],&lef,tempo)) {
					printf("Falha. Nao foi possivel completar o evento ENTRA!\n");
					return 1;
				}
				aux = aux->prox;
				removeEvento(&lef);
				break;
			//Evento Sai
			case SAI:
				if (!eventoSai(mundo.herois[aux->idHeroi],&mundo.bases[aux->idBase],&lef,tempo)) {
					printf("Falha. Nao foi possivel completar o evento SAI!\n");
					return 1;
				}
				aux = aux->prox;
				removeEvento(&lef);
				break;
			//Evento Viaja
			case VIAJA:
				if (!eventoViaja(mundo, mundo.herois[aux->idHeroi], mundo.bases[aux->idBase], &lef, tempo)) {
					printf("Falha. Nao foi possivel completar o evento VIAJA!\n");
					return 1;
				}
				aux = aux->prox;
				removeEvento(&lef);
				break;
			//Evento Missao
			case MISSAO:
				if (!eventoMissao(&mundo, mundo.missoes[aux->idMissao], &lef, tempo)) {
					printf("Falha. Nao foi possivel completar o evento MISSAO!\n");
					return 1;
				}
				tentativas++; //Incrementa a quantidade total de tentativas realizadas para completar missoes
				aux = aux->prox;
				removeEvento(&lef);
				break;
			//Evento Fim
			case FIM:
				eventoFim(mundo,tentativas,tempo);
				break;
		}
	}
	
	//Free no resto da LEF
	liberaEventos(&lef);

	//Free nas habilidades dos Herois
	for (int i = 0; i < mundo.numHerois; i++)
		if (mundo.herois[i].habilidades != NULL)
			liberaLista(&mundo.herois[i].habilidades);

	//Free no conjunto de herois presentes na base e na fila de espera
	for (int i = 0; i < mundo.numBases; i++) {
		if (mundo.bases[i].presentes != NULL)
			liberaLista(&mundo.bases[i].presentes);
		if (mundo.bases[i].espera != NULL)
			liberaFila(&mundo.bases[i].espera);
	}

	//Free nas habilidades das missoes
	for (int i = 0; i < mundo.numMissoes; i++)
		if (mundo.missoes[i].habilidades != NULL)
			liberaLista(&mundo.missoes[i].habilidades);



	//Free nos vetores que armazenam os Herois, Bases e Missoes
	free(mundo.herois);
	free(mundo.bases);
	free(mundo.missoes);



	return 0;
}
