#include"biblioteca.h"


int main (int argc, char *argv[]){

    int nthread = atoi(argv[1]);
    char *nomeArquivo = argv[2]; // Recebe o nome do arquivo por parametro


    strcpy(auxNomeArquivo, nomeArquivo);
    strcat(auxNomeArquivo, "-");
    strcat(nomeArquivo, ".txt");


    inicializar_ofertas(nomeArquivo);
    printf("----------------Estoque Inicio----------------\n");
    printList(listaOfertas);
    printf("----------------------------------------------\n");
    printf("--------------THREADS ATENDENTES--------------\n");
    cria_threads(nthread);
    printf("----------------------------------------------\n");

    char nomeLanche[] = "x-mondongo";
    /*Lanche *ddd = popLista(&listaOfertas);
    Lanche *dde = popLista(&listaOfertas);
    Lanche *ddi = popLista(&listaOfertas);
    Lanche *fodeu = popLista(&listaOfertas);
    printf("%s - %d - %d --> DDD || x = %d \n", ddi->nome, ddi->preco, ddi->quantidade,x->sanduiche.preco);
*/
   printListaCaixa(listaCaixa);
   printf("---------------Estoque Sobrando---------------\n");
   printList(listaOfertas);
   printf("----------------------------------------------\n");

    return 0;
}
