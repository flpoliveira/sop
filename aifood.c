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


int main (int argc, char *argv[]){

    int c, contaLinhas= 0;
    int nthread = atoi(argv[0]);
    char *nomeArquivo = argv[2]; // Recebe o nome do arquivo por parametro


    FILE *arq;
    if((arq = fopen(nomeArquivo, "r")) == NULL)
    {
        perror("Erro: fopen");
        exit(EXIT_FAILURE);
    }
    while((c = fgetc(arq)) != EOF)
    {
      if(c == '\n')
        contaLinhas++;
    }
    Lanche ofertas[contaLinhas];
    int i;
    rewind(arq); //Volta o ponteiro pro inicio do Arquivo
    for(i = 0; i < contaLinhas; i++)
    {
        fscanf(arq, "%s %d %d", ofertas[i].nome, &ofertas[i].preco, &ofertas[i].quantidade);
        printf("%s\n", ofertas[i].nome);
    }
    estoque = ofertas;


    for(i = 0; i < nthread; i++)
    {

    }



    return 0;
}
