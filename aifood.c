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

struct Node {
    Lanche sanduiche;
    struct Node* next;
    struct Node* prev;
};
typedef struct{
    Lanche *lanches;
} Pedido;
void append(struct Node** head_ref, Lanche new_data)
{
    //printf("%s - append\n", new_data.nome);
    /* 1. allocate node */
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));

    struct Node* last = *head_ref; /* used in step 5*/

    /* 2. put in the data  */
    new_node->sanduiche = new_data;

    /* 3. This new node is going to be the last node, so
          make next of it as NULL*/
    new_node->next = NULL;

    /* 4. If the Linked List is empty, then make the new
          node as head */
    if (*head_ref == NULL) {
        new_node->prev = NULL;
        *head_ref = new_node;
        return;
    }

    /* 5. Else traverse till the last node */
    while (last->next != NULL)
        last = last->next;

    /* 6. Change the next of last node */
    last->next = new_node;

    /* 7. Make last node as previous of new node */
    new_node->prev = last;

    return;
}

// This function prints contents of linked list starting from the given node
void printList(struct Node* node)
{

    struct Node* last;
    while (node != NULL) {
        printf(" Lista : %s ", node->sanduiche.nome);
        printf(" - preco: %d - quantidade %d", node->sanduiche.preco, node->sanduiche.quantidade);
        last = node;
        node = node->next;
        printf("\n");
    }

    /*printf("\nTraversal in reverse direction \n");
    while (last != NULL) {
        printf(" %s ", node->sanduiche.nome);
        last = last->prev;
    }*/
}

void deleteNode(struct Node** head_ref, struct Node* del)
{
    /* base case */
    if (*head_ref == NULL || del == NULL)
        return;

    /* If node to be deleted is head node */
    if (*head_ref == del)
        *head_ref = del->next;

    /* Change next only if node to be deleted
       is NOT the last node */
    if (del->next != NULL)
        del->next->prev = del->prev;

    /* Change prev only if node to be deleted
       is NOT the first node */
    if (del->prev != NULL)
        del->prev->next = del->next;

    /* Finally, free the memory occupied by del*/
    free(del);
}

Lanche popLista(struct Node** head_ref)
{
  struct Node* current = *head_ref;
  Lanche aux;
  while(current->prev != NULL)
  {
    current = current->prev;
  }
  aux = current->sanduiche;
  deleteNode(head_ref, current);

  return aux;
}


Lanche *estoque;
Lanche *pedidosBemSucedidos;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barreiraTodosProntos;




void *atendente(void *argp)
{
  long id = (long) argp;
  pthread_barrier_wait(&barreiraTodosProntos);
  pthread_mutex_lock(&mtx);
  printf("Pinto %ld\n", id);
  pthread_mutex_unlock(&mtx);


}

void cria_threads(int nthread)
{
  pthread_barrier_init(&barreiraTodosProntos,NULL,nthread);
  int rc;
  pthread_t threads[nthread];
  for(long i = 0; i < nthread; i ++)
  {
    rc = pthread_create(&threads[i], NULL, (void *) atendente, (void *) (i+1));
  }
  for(int i = 0; i < nthread; i ++)
  {
    //printf("%i\n", i);
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
    struct Node* head = NULL;
    int i = 1;
    while(fscanf(arq, "%s %d %d", ofertas[i-1].nome, &ofertas[i-1].preco, &ofertas[i-1].quantidade) != EOF)
    {

      //printf("%s\n", ofertas[i-1].nome);
      append(&head, ofertas[i-1]);
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
    //printf("%s\n", pedidos[2].lanches[0].nome);
    cria_threads(nthread);
    printList(head);
    Lanche ddd = popLista(&head);
    Lanche dde = popLista(&head);
    Lanche ddi = popLista(&head);
    printf("%s - %d - %d --> DDD \n", ddi.nome, ddi.preco, ddi.quantidade);
    printList(head);





    return 0;
}
