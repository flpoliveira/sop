#include <stdio.h>

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
  //int idPedido;
  struct NoCaixa * next;
  struct NoCaixa * prev;

};

typedef struct{
  long idAtendente;
  int totalPedidos;
  int valorTotal;
} ProcessamentoCaixa;

char auxNomeArquivo[100];
ProcessamentoCaixa * relatorioFinanceiro;
struct Node* listaOfertas = NULL;
struct NoCaixa* listaCaixa = NULL;
pthread_mutex_t mtxEstoque = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtxCaixa = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barreiraTodosProntos;
pthread_cond_t condCaixa;
int tamanhoListaCaixa = 0;


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
struct NoCaixa  * popListaCaixa(struct NoCaixa** head_ref);
