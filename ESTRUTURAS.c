#include "ESTRUTURAS.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void append(struct No ** head_ref, Lanche novoLanche)
{
    struct No* new_node = ( struct No*)malloc(sizeof(struct No));

    struct No* last = *head_ref; /* used in step 5*/

    /* 2. put in the data  */
    new_node->sanduiche = novoLanche;

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
void printList(struct No * node)
{

    struct No * last;
    while (node != NULL) {
        printf(" Lista : %s ", node->sanduiche.nome);
        printf(" - preco: %u - quantidade %u", node->sanduiche.preco, node->sanduiche.quantidade);
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

int retira_lanches_estoque(struct No * node, char nomeLanche[] , unsigned int quantidade)
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
