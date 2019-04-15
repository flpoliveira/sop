#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NOME_NAO_ENCONTRADO -1
#define QUANTIDADE_INSUFICIENTE 0
#define EH_POSSIVEL 1
#define LISTA_INICIO 1
#define LISTA_FIM 2
#define LISTACAIXA 1

typedef struct {
  char nome[100];
  unsigned int preco;
  unsigned int quantidade;
} Lanche;

typedef struct{
  long idAtendente;
  unsigned int preco;
  unsigned int quantidade;

} Pedido;

/*struct No{
    long idAtendente;
    unsigned int quantidadeAtendido;
    unsigned int valorTotal;
} NoCaixa;*/

struct No{
  Lanche sanduicheAntes;
  Lanche sanduiche;
  struct No * next;
  struct No * prev;
};

struct NoCaixa{
    Pedido pedido;
    struct NoCaixa * next;
};

struct No * listaOfertas = NULL;
struct NoCaixa * listaCaixa = NULL;
struct NoCaixa * listaPedidosProcessados = NULL;
int tamanhoListaCaixa = 0;
void append(struct No ** head_ref, Lanche novoLanche);
void printList(struct No * node, int x);
void deleteNode(struct No** head_ref, struct No * del);
int eh_possivel_processar_pedido(struct No* node, char nomeLanche[], unsigned int quantidade);
unsigned int retira_lanches_estoque(struct No * node, char nomeLanche[],unsigned int quantidade);
void appendCaixa(struct NoCaixa** head_ref, long idAtendente, unsigned int valor, int x);
void printListCaixa(struct NoCaixa *node);
Pedido * popLista(struct NoCaixa ** head_ref);
int buscaListaIncrementaValor(struct NoCaixa * node, Pedido x);
void append(struct No ** head_ref, Lanche novoLanche)
{
    struct No* new_node = ( struct No*)malloc(sizeof(struct No));

    struct No* last = *head_ref; /* used in step 5*/

    /* 2. put in the data  */
    new_node->sanduiche = novoLanche;
    new_node->sanduicheAntes = novoLanche;

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
void printList(struct No * node, int x)
{
  if(x == LISTA_FIM)
  {
    //struct No * last;
    while (node != NULL) {
        printf(" Lista : %s ", node->sanduiche.nome);
        printf(" - preco: %u - quantidade %u", node->sanduiche.preco, node->sanduiche.quantidade);
      //  last = node;
        node = node->next;
        printf("\n");
    }
  }
  else
  {
    while (node != NULL) {
        printf(" Lista : %s ", node->sanduicheAntes.nome);
        printf(" - preco: %u - quantidade %u", node->sanduicheAntes.preco, node->sanduicheAntes.quantidade);
      //  last = node;
        node = node->next;
        printf("\n");
    }
  }


    /*printf("\nTraversal in reverse direction \n");
    while (last != NULL) {
        printf(" %s ", node->sanduiche.nome);
        last = last->prev;
    }*/
}

void deleteNode(struct No** head_ref, struct No * del)
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

int eh_possivel_processar_pedido(struct No* node, char nomeLanche[], unsigned int quantidade)
{

  while (node != NULL)
  {
    if(strcmp(node->sanduiche.nome, nomeLanche) == 0)
    {
      if(node->sanduiche.quantidade >= quantidade)
        return 1;
      else
        return 0;
    }

    //printf("%s -> ", node->sanduiche.nome);
    node = node->next;
  }
  return -1;
}

unsigned int retira_lanches_estoque(struct No * node, char nomeLanche[] , unsigned int quantidade)
{
  while (node != NULL)
  {
    if(strcmp(node->sanduiche.nome, nomeLanche) == 0)
    {
        node->sanduiche.quantidade = node->sanduiche.quantidade - quantidade;
        return quantidade * node->sanduiche.preco;
    }

    //printf("%s -> ", node->sanduiche.nome);
    node = node->next;
  }
  return -1;
}

void appendCaixa(struct NoCaixa** head_ref, long idAtendente, unsigned int valor, int x)
{

    Pedido aux;
    if(x == LISTACAIXA)
      tamanhoListaCaixa++;
    aux.idAtendente = idAtendente;
    aux.preco = valor;
    aux.quantidade = 1;
    /* 1. allocate node */
    struct NoCaixa* new_node = (struct NoCaixa*) malloc(sizeof(struct NoCaixa));

    struct NoCaixa *last = *head_ref;  /* used in step 5*/

    /* 2. put in the data  */
    new_node->pedido  = aux;

    /* 3. This new node is going to be the last node, so make next of
          it as NULL*/
    new_node->next = NULL;

    /* 4. If the Linked List is empty, then make the new node as head */
    if (*head_ref == NULL)
    {
       *head_ref = new_node;
       return;
    }

    /* 5. Else traverse till the last node */
    while (last->next != NULL)
        last = last->next;

    /* 6. Change the next of last node */
    last->next = new_node;

    return;
}

// This function prints contents of linked list starting from head
void printListCaixa(struct NoCaixa *node)
{
  while (node != NULL)
  {
     printf("ListaCaixa: Atendente : %ld -  valorTotal : %u - quantidade : %u \n", node->pedido.idAtendente, node->pedido.preco, node->pedido.quantidade);
     node = node->next;
  }
}

Pedido * popLista(struct NoCaixa ** head_ref)
{

  Pedido * aux = NULL;
  if(*head_ref != NULL)
  {
    struct NoCaixa* current = *head_ref;

    aux = malloc(sizeof(Pedido));
    *aux = current->pedido;

    *head_ref = current->next;   // Change head
    tamanhoListaCaixa--;
    free(current);
  }

  return aux;
}

int buscaListaIncrementaValor(struct NoCaixa * node, Pedido x)
{
  while(node != NULL)
  {
    if(node->pedido.idAtendente == x.idAtendente)
    {
      node->pedido.quantidade++;
      node->pedido.preco += x.preco;
      return 1;
    }
    node = node->next;
  }
  return 0;
}
