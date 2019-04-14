#include <stdio.h>
#include"biblioteca.h"

int main (int argc, char *argv[]){

    int nthread = atoi(argv[1]);
    char *nomeArquivo = argv[2]; // Recebe o nome do arquivo por parametro

    char auxNomeArquivo[100];
    strcpy(auxNomeArquivo, nomeArquivo);
    strcat(nomeArquivo, ".txt");
    //printf("%s\n", nomeArquivo);
    inicializar_ofertas(nomeArquivo);
    int i = 0;

    FILE *arq;

  //  estoque = ofertas;
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
    //printf("%s\n", pedidos[2].lanches[0].nome);
    cria_threads(nthread);
    printList(listaOfertas);
    char nomeLanche[] = "x-mondongo";
    int x  = buscaLista(listaOfertas, nomeLanche);
    Lanche ddd = popLista(&listaOfertas);
    Lanche dde = popLista(&listaOfertas);
    Lanche ddi = popLista(&listaOfertas);

    printf("%s - %d - %d --> DDD || x = %d \n", ddi.nome, ddi.preco, ddi.quantidade, x);
    printList(listaOfertas);
    return 0;
}
