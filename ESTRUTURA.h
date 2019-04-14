#ifndef ESTRUTURA_H
#define ESTRUTURA_H


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

#endif
