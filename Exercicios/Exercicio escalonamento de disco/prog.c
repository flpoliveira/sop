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
		if((*cabeca)->operacao == new_node->operacao && new_node->operacao == 'r')
		{
			if((new_node->blocoInicial + new_node->nBlocos) >= (*cabeca)->blocoInicial && (*cabeca)->nBlocos + new_node->nBlocos < 64)
			{
				(*cabeca)->blocoInicial = new_node->blocoInicial;
				(*cabeca)->nBlocos = (*cabeca)->nBlocos + new_node->nBlocos;
			}

		}
		else if((*cabeca)->operacao == new_node->operacao && new_node->operacao == 'w')
		{
			if(new_node->blocoInicial + new_node->nBlocos > (*cabeca)->blocoInicial && (*cabeca)->nBlocos + new_node->nBlocos < 64)
			{
				(*cabeca)->blocoInicial = new_node->blocoInicial;
				(*cabeca)->nBlocos = (*cabeca)->nBlocos + new_node->nBlocos;
			}
		}
		else
		{
			new_node->prox = *cabeca;
			new_node->prox->antes = new_node;
			*cabeca = new_node;
		}


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
	struct No * lista = NULL;
	//FILE *arq = fopen("entrada.txt", "r");
	/*while(fgets(str, MAXLEN, stdin) != NULL)
	{
		sscanf(str, "%d %d %c\n", &blocoInicial, &nBlocos, &operacao);
		append(&lista, blocoInicial, nBlocos, operacao);
	}
	*/

	while(scanf("%d %d %c", &blocoInicial, &nBlocos, &operacao) != EOF)
	{
		append(&lista, blocoInicial, nBlocos, operacao);
	}
	//printf("%d %d %c\n", lista->blocoInicial, lista->nBlocos, lista->operacao);
	//ordernarLista(lista);
	printLista(lista);


	return 0;
}
