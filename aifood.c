//#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include"biblioteca.h"

typedef struct {
  char nome[100];
  int preco;
  int quantidade;
} Lanche;

typedef struct{
    Lanche *lanches;
} Pedido;


Lanche *estoque;


void *caixa()
{

}

void *atendente()
{

}


int main (int argc, char *argv[]){

    int nthread = atoi(argv[1]);
    char *nomeArquivo = argv[2]; // Recebe o nome do arquivo por parametro

    char auxNomeArquivo[100];
    strcpy(auxNomeArquivo, nomeArquivo);
    strcat(nomeArquivo, ".txt");
    //printf("%s\n", nomeArquivo);


    FILE *arq;
    if((arq = fopen(nomeArquivo, "r")) == NULL)
    {
        perror("Erro: fopen");
        exit(EXIT_FAILURE);
    }
    Lanche *ofertas;
    ofertas = malloc(sizeof(Lanche));

    int i = 1;

    while(fscanf(arq, "%s %d %d", ofertas[i-1].nome, &ofertas[i-1].preco, &ofertas[i-1].quantidade) != EOF)
    {
      i++;
      ofertas = realloc(ofertas, sizeof(Lanche) * i);
    }

    estoque = ofertas;
    //printf("%s\n", estoque[2].nome);

    Pedido pedidos[nthread];



    for(int j = 0; j < nthread; j++)
    {
        char nomeArqPedido[100];
        char auxConcatena[100];
        strcpy(nomeArqPedido, auxNomeArquivo);
        strcat(nomeArqPedido, "-");
        snprintf(auxConcatena, 10, "%d", (j+1));
        strcat(nomeArqPedido, auxConcatena);
        strcat(nomeArqPedido, ".txt");
        //printf("%s\n", nomeArqPedido);


        Lanche *lanchesDoPedido = malloc(sizeof(Lanche));
        if((arq = fopen(nomeArqPedido, "r")) == NULL)
        {
            perror("Erro: fopen");
            exit(EXIT_FAILURE);
        }

        i = 1;
        while(fscanf(arq, "%s %d %d", lanchesDoPedido[i-1].nome, &lanchesDoPedido[i-1].preco, &lanchesDoPedido[i-1].quantidade) != EOF)
        {
          i++;
          lanchesDoPedido = realloc(lanchesDoPedido, sizeof(Lanche) * i);
        }

      pedidos[j].lanches = lanchesDoPedido;

    }
    printf("%s\n", pedidos[2].lanches[0].nome);






    return 0;
}
