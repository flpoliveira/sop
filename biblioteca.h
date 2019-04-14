#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>



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

struct NoCaixa {
  int valor;
  long idAtendente;
  int idPedido;
  struct NoCaixa * next;
  struct NoCaixa * prev;

};
char auxNomeArquivo[100];

struct Node* listaOfertas = NULL;
struct NoCaixa* listaCaixa = NULL;
pthread_mutex_t mtxEstoque = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtxCaixa = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barreiraTodosProntos;

void *atendente(void *argp);
void cria_threads(int nthread);
void inicializar_ofertas(char nomeArquivo[]);
void append(struct Node** head_ref, Lanche new_data);
void printList(struct Node* node);
void deleteNode(struct Node** head_ref, struct Node* del);
Lanche * popLista(struct Node** head_ref);
void appendNoCaixa(struct NoCaixa** head_ref, long idAtendente, int valor);
struct Node * buscaLista(struct Node* node, char nomeLanche[], int quantidade);
int retira_lanches_estoque(struct Node* node, int quantidade);


void *atendente(void *argp)
{
  long id = (long) argp;
  char nomeArqPedido[100];
  char auxConcatena[100];
  strcpy(nomeArqPedido, auxNomeArquivo);
  snprintf(auxConcatena, 10, "%ld", (id));
  strcat(nomeArqPedido, auxConcatena);
  strcat(nomeArqPedido, ".txt");

  struct Node* listaPedido = NULL;
  FILE *arq;
  if((arq = fopen(nomeArqPedido, "r")) == NULL)
  {
      perror("Erro: fopen");
      exit(EXIT_FAILURE);
  }
  Lanche pedido;

  while(fscanf(arq, "%s %d", pedido.nome, &pedido.quantidade) != EOF)
  {
    pedido.preco = 0;
    append(&listaPedido, pedido);
  }
  pthread_barrier_wait(&barreiraTodosProntos);
  while(1)
  {
    Lanche * aux = popLista(&listaPedido);
    if(aux == NULL)
      break;

    printf("thread %ld acessando buscaLista\n", id);
    struct Node * x  = buscaLista(listaOfertas, aux->nome, aux->quantidade);
    if(x != NULL)
    {
      pthread_mutex_lock(&mtxEstoque);
        int valor  = retira_lanches_estoque(x, aux->quantidade);
      pthread_mutex_unlock(&mtxEstoque);
      pthread_mutex_lock(&mtxCaixa);
        printf("valor %d do pedido pela Thread %ld\n", valor, id);
        appendNoCaixa(&listaCaixa, id, valor);
      pthread_mutex_unlock(&mtxCaixa);
    }


  }



  printList(listaPedido);

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

void inicializar_ofertas(char nomeArquivo[])
{
  FILE *arq;
  if((arq = fopen(nomeArquivo, "r")) == NULL)
  {
      perror("Erro: fopen");
      exit(EXIT_FAILURE);
  }
  Lanche ofertas;

  while(fscanf(arq, "%s %d %d", ofertas.nome, &ofertas.preco, &ofertas.quantidade) != EOF)
  {
    append(&listaOfertas, ofertas);
  }
}

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

void appendNoCaixa(struct NoCaixa** head_ref, long idAtendente, int valor)
{
    //printf("%s - append\n", new_data.nome);
    /* 1. allocate node */
    struct NoCaixa* new_node = (struct NoCaixa*)malloc(sizeof(struct NoCaixa));

    struct NoCaixa* last = *head_ref; /* used in step 5*/

    /* 2. put in the data  */
    new_node->idAtendente = idAtendente;
    new_node->valor = valor;

    /* 3. This new node is going to be the last node, so
          make next of it as NULL*/
    new_node->next = NULL;

    /* 4. If the Linked List is empty, then make the new
          node as head */
    if (*head_ref == NULL) {
        new_node->prev = NULL;
        new_node->idPedido = 1;
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
    new_node->idPedido = new_node->prev->idPedido + 1;

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
void printListaCaixa(struct NoCaixa * node)
{
  struct NoCaixa* last;
    while (node != NULL) {
      printf(" Lista Caixa: %ld ", node->idAtendente);
      printf(" - idPedido: %d - valor: %d", node->idPedido, node->valor);
      last = node;
      node = node->next;
      printf("\n");
    }
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

Lanche  * popLista(struct Node** head_ref)
{
  Lanche * aux = NULL;

  if(*head_ref != NULL)
  {

    struct Node* current = *head_ref;


    while(current->prev != NULL)
    {
      current = current->prev;
    }

    aux = malloc(sizeof(Lanche));
    *aux = current->sanduiche;

    deleteNode(head_ref, current);
  }

  return aux;

}

struct Node * buscaLista(struct Node* node, char nomeLanche[], int quantidade)
{
  printf("%s, %d -- Buscando nome\n", nomeLanche, quantidade);

  while (node != NULL)
  {
    if(strcmp(node->sanduiche.nome, nomeLanche) == 0)
    {
      if(node->sanduiche.quantidade >= quantidade)
        return node;

    }

    //printf("%s -> ", node->sanduiche.nome);
    node = node->next;
  }
  return NULL;
}
int retira_lanches_estoque(struct Node* node, int quantidade)
{
  node->sanduiche.quantidade = node->sanduiche.quantidade - quantidade;
  return quantidade * node->sanduiche.preco;
}
