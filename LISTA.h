#ifndef LISTA_H
#define LISTA_H
#include "ESTRUTURA.h"

void push(struct Node** head_ref, int new_data) ;
void insertAfter(struct Node* prev_node, int new_data);
void append(struct Node** head_ref, int new_data);
void printList(struct Node* node);



#endif
