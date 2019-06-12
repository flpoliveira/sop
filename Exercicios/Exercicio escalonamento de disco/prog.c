#include<stdio.h>
#include<stdlib.h>
#define MAXLEN 30

struct No{
	int blocoInicial, nBlocos;
	char operacao;
	struct No * prox;
	struct No * antes;
};

void append(struct No ** cabeca, int blocoInicial, int nBlocos, char operacao)
{
	struct No* new_node = ( struct No*)malloc(sizeof(struct No));

	new_node->blocoInicial = blocoInicial;
	new_node->nBlocos = nBlocos;
	new_node->operacao = operacao;
	new_node->prox = NULL;

	struct No * current;

	if(*cabeca == NULL)
	{
		new_node->antes = NULL;
		*cabeca = new_node;
		return;
	}
	else if((*cabeca)->blocoInicial >= new_node->blocoInicial)
	{
		// if((*cabeca)->operacao == new_node->operacao && new_node->operacao == 'r')
		// {
		// 	if((new_node->blocoInicial + new_node->nBlocos) >= (*cabeca)->blocoInicial && (*cabeca)->nBlocos + ((*cabeca)->blocoInicial - new_node->blocoInicial) < 64)
		// 	{
		// 		(*cabeca)->nBlocos = (*cabeca)->nBlocos + ((*cabeca)->blocoInicial - new_node->blocoInicial) ;
		// 		(*cabeca)->blocoInicial = new_node->blocoInicial;
		// 	}
		//
		// }
		// else
		// {
			new_node->prox = *cabeca;
			new_node->prox->antes = new_node;
			*cabeca = new_node;
		//}


	}
	else
	{
			current = *cabeca;
			while(current->prox != NULL && current->prox->blocoInicial < new_node->blocoInicial)
			current = current->prox;

			new_node->prox = current->prox;
			if(current->prox != NULL)
				new_node->prox->antes = new_node;

			current->prox = new_node;
			new_node->antes = current;
	}


	return;

}

void blockUnionR(struct No ** head)
{
	int k;
	struct No * current = *head;
	while(current->antes != NULL)
		current = current->antes;
	while(current->prox != NULL)
	{
		k = current->prox->blocoInicial + current->prox->nBlocos - current->blocoInicial;

		if(current->blocoInicial + current->nBlocos >= current->prox->blocoInicial && k < 64)
		{
			// printf("%d\n", k);
			// 	printf("<<%d %d %c\n", current->blocoInicial, current->nBlocos, current->operacao);
			// 	printf("--%d %d %c\n", current->prox->blocoInicial, current->prox->nBlocos, current->prox->operacao);
				current->nBlocos = k;

				//struct No * aux = current->prox;
				current->prox = current->prox->prox;
				//free(aux);
				//printf(">>%d %d %c\n", current->blocoInicial, current->nBlocos, current->operacao);
		}
		else
			current = current->prox;
	}
}

void blockUnionW(struct No ** head)
{
	int k;
	struct No * current = *head;
	while(current->antes != NULL)
		current = current->antes;
	while(current->prox != NULL)
	{
		k = current->prox->blocoInicial + current->prox->nBlocos - current->blocoInicial;
		if(current->blocoInicial + current->nBlocos == current->prox->blocoInicial && k < 64)
		{
				//printf("<<%d %d %c\n", current->blocoInicial, current->nBlocos, current->operacao);
				//printf("--%d %d %c\n", current->prox->blocoInicial, current->prox->nBlocos, current->prox->operacao);
				current->nBlocos = k;

				//struct No * aux = current->prox;
				current->prox = current->prox->prox;
				//free(aux);
				//printf(">>%d %d %c\n", current->blocoInicial, current->nBlocos, current->operacao);
		}
		else
			current = current->prox;
	}

}

void printLista(struct No * lista)
{
	while(lista->antes != NULL)
		lista = lista->antes;
	while(lista != NULL)
	{
		printf("%d %d %c\n", lista->blocoInicial, lista->nBlocos, lista->operacao);
		lista = lista->prox;
	}

}


int main(){

	int blocoInicial, nBlocos;
	char operacao, str[MAXLEN+1];
	struct No * listaR = NULL;
	struct No * listaW = NULL;
	//FILE *arq = fopen("entrada.txt", "r");
	while(fgets(str, MAXLEN, stdin) != NULL)
	{
		sscanf(str, "%d %d %c\n", &blocoInicial, &nBlocos, &operacao);
		if(operacao == 'r')
			append(&listaR, blocoInicial, nBlocos, operacao);
		else
			append(&listaW, blocoInicial, nBlocos, operacao);
	}


	// while(scanf("%d %d %c", &blocoInicial, &nBlocos, &operacao) != EOF)
	// {
	// 	if(operacao == 'r')
	// 		append(&listaR, blocoInicial, nBlocos, operacao);
	// 	else
	// 		append(&listaW, blocoInicial, nBlocos, operacao);
	//
	// }
	struct No * heuristica = NULL;
	if(listaW != NULL)
	{
		blockUnionW(&listaW);
		while(listaW->antes != NULL)
			listaW = listaW->antes;
	}
	if(listaR != NULL)
	{
		blockUnionR(&listaR);
		while(listaR->antes != NULL)
			listaR = listaR->antes;
	}



	while(listaW != NULL)
	{
		append(&heuristica, listaW->blocoInicial, listaW->nBlocos, listaW->operacao);
		listaW = listaW->prox;
	}
	while(listaR != NULL)
	{
		append(&heuristica, listaR->blocoInicial, listaR->nBlocos, listaR->operacao);
		listaR = listaR->prox;
	}

	// printLista(listaW);
	// printLista(listaR);
	printf("Fila:\n");
	if(heuristica != NULL)
		printLista(heuristica);

	//printf("%d %d %c\n", lista->blocoInicial, lista->nBlocos, lista->operacao);
	//ordernarLista(lista);


	return 0;
}
