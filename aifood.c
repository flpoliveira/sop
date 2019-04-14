<<<<<<< HEAD
=======
#include <stdio.h>
>>>>>>> 383ae79807ec39a3af960499ef10f752a52bc21a

#include"biblioteca.h"


int main (int argc, char *argv[]){

    int nthread = atoi(argv[1]);
    char *nomeArquivo = argv[2]; // Recebe o nome do arquivo por parametro


    strcpy(auxNomeArquivo, nomeArquivo);
    strcat(auxNomeArquivo, "-");
    strcat(nomeArquivo, ".txt");


    inicializar_ofertas(nomeArquivo);
    printf("--------------THREADS ATENDENTES--------------\n");
    cria_threads(nthread);
    printf("----------------------------------------------\n");
    printList(listaOfertas);
    char nomeLanche[] = "x-mondongo";
    int x  = buscaLista(listaOfertas, nomeLanche, 50);
    Lanche ddd = popLista(&listaOfertas);
    Lanche dde = popLista(&listaOfertas);
    Lanche ddi = popLista(&listaOfertas);

<<<<<<< HEAD

    printf("%s - %d - %d --> DDD || x = %d \n", ddi.nome, ddi.preco, ddi.quantidade,x);
=======
    printf("%s - %d - %d --> DDD || x = %d \n", ddi.nome, ddi.preco, ddi.quantidade, x);
>>>>>>> 383ae79807ec39a3af960499ef10f752a52bc21a
    printList(listaOfertas);

    return 0;
}
