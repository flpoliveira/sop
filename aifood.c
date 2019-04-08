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

    int c, contaLinhas= 0;
    int nthread = atoi(argv[1]);
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

    }
    estoque = ofertas;

   // Pedido pedidos[nthread];
    


    for(i = 0; i < nthread; i++)
    {

        char nomeArqPedido[] = {'a', 'b', 'c', '-'};
        itoa((i+1), nomeArqPedido+4, 10);
        //strcat(nomeArqPedido, ".txt");
        printf("%s\n", nomeArqPedido);
        
        if((arq = fopen(nomeArqPedido, "r")) == NULL)
        {
            perror("Erro: fopen");
            exit(EXIT_FAILURE);
        }
        contaLinhas = 0;
        while((c = fgetc(arq)) != EOF)
        {
            if(c == '\n')
            {
                contaLinhas++;
                printf("Linha - %d\n", contaLinhas);
            }
            
        }
        Lanche lanchesDoPedido[contaLinhas];
        rewind(arq);
        for(int j = 0; j < contaLinhas; j++)
        {
            fscanf(arq, "%s %d", lanchesDoPedido[j].nome, &lanchesDoPedido[j].quantidade);
            lanchesDoPedido[j].preco = 0;
            printf("%s\n", lanchesDoPedido[j].nome);
        }
  //      pedidos[i].lanches = lanchesDoPedido;

    }




    return 0;
}
