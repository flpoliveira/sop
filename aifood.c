#include <pthread.h>
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
Lanche *pedidosBemSucedidos;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barreiraTodosProntos;




void *atendente(void *argp)
{
  long id = (long) argp;
  printf("hue\n");
  pthread_barrier_wait(&barreiraTodosProntos);
    printf("Pinto %ld\n", id);
}

void cria_threads(int nthread)
{
  int rc;
  pthread_t threads[nthread];
  for(long i = 0; i < nthread; i ++)
  {
    rc = pthread_create(&threads[i], NULL, (void *) atendente, (void *) i);
  }
  printf("AEAEA");
  for(int i = 0; i < nthread; i ++)
  {
    rc = pthread_join(threads[i], NULL);
  }
  pthread_barrier_destroy(&barreiraTodosProntos);
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
    cria_threads(nthread);





    return 0;
}
