#include "ESTRUTURAS.h"
#include <pthread.h>
#define fmtfinanc "%9ld   %7u   %10u\n"
#define fmtestoque "%-15s   %7u   %8u\n"

char auxNomeArquivo[100];
pthread_barrier_t barreiraTodosProntos;
pthread_mutex_t mtxEstoque = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtxCaixa = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condCaixa;
int fimAtendentes = 0;

void * atendente(void *argp)
{
  long id = (long) argp;
  // Preparativos para ler o seu arquivo de pedido
  char nomeArqPedido[100];
  char auxConcatena[100];
  strcpy(nomeArqPedido, auxNomeArquivo);
  snprintf(auxConcatena, 10, "%ld", (id));
  strcat(nomeArqPedido, auxConcatena);
  //printf("%s\n", nomeArqPedido);
  FILE *arq;
  if((arq = fopen(nomeArqPedido, "r")) == NULL)
  {
      perror("Erro: fopen");
      exit(EXIT_FAILURE);
  }
  Lanche sanduiche;
  //printf("Atendente %ld criado.\n", (long) argp );
  pthread_barrier_wait(&barreiraTodosProntos); // Espera as demais
  while(fscanf(arq, "%s %u", sanduiche.nome, &sanduiche.quantidade) != EOF)
  {
    sanduiche.preco = 0;
    pthread_mutex_lock(&mtxEstoque);
    int aux = eh_possivel_processar_pedido(listaOfertas, sanduiche.nome, sanduiche.quantidade);
    if(aux == NOME_NAO_ENCONTRADO)
    {
      printf("O Atendente %ld nao encontrou lanche '%s' no estoque, pedido cancelado.\n", id, sanduiche.nome);
    }
    else if(aux == QUANTIDADE_INSUFICIENTE)
    {
      printf("O Atendente %ld encontrou o lanche '%s', porem a quantidade em estoque eh insuficiente, pedido cancelado.\n", id, sanduiche.nome);
    }
    else if(aux == EH_POSSIVEL)
    {
      //printf("O Atendente %ld encontrou o lanche '%s' e a quantidade em estoque eh suficiente, pedido sendo atendido.\n", id, sanduiche.nome);

        unsigned int valor = retira_lanches_estoque(listaOfertas, sanduiche.nome , sanduiche.quantidade);
        pthread_mutex_lock(&mtxCaixa);
        appendCaixa(&listaCaixa, id, valor, 1);
        pthread_cond_signal(&condCaixa);
        pthread_mutex_unlock(&mtxCaixa);

    }
    pthread_mutex_unlock(&mtxEstoque);
  }
  pthread_mutex_lock(&mtxCaixa);
  fimAtendentes--;
  pthread_cond_signal(&condCaixa);
  pthread_mutex_unlock(&mtxCaixa);
  //printf("%ld - %d \n", id, fimAtendentes);
  pthread_exit(NULL);
}
void * caixa(void *argp)
{
  //printf("Caixa criado.\n");
  pthread_barrier_wait(&barreiraTodosProntos);
  while(fimAtendentes > 0 || tamanhoListaCaixa > 0)
  {
    //printf("Caixa processa... fimAtendentes = %d\n", fimAtendentes);
    pthread_mutex_lock(&mtxCaixa);
    if(tamanhoListaCaixa == 0 && fimAtendentes > 0)
    {
      //printf("Caixa esperando Atendente, tamanhodaLista = %i, fimAtendentes = %d\n", tamanhoListaCaixa, fimAtendentes);
      pthread_cond_wait(&condCaixa, &mtxCaixa);
    }

      Pedido * aux = popLista(&listaCaixa);
      while(aux != NULL)
      {
        if(buscaListaIncrementaValor(listaPedidosProcessados, *aux) == 0)
        {
          appendCaixa(&listaPedidosProcessados, aux->idAtendente, aux->preco, 0);

        }
        aux = popLista(&listaCaixa);
      }
    pthread_mutex_unlock(&mtxCaixa);
  }
  //printf("tamanhoListaCaixa -> %d\n", tamanhoListaCaixa);
  pthread_exit(NULL);
}
void inicializa_lanches(FILE * arq_ofertas)
{
  Lanche ofertas;
  while(fscanf(arq_ofertas, "%s %u %u", ofertas.nome, &ofertas.preco, &ofertas.quantidade) != EOF)
  {
    append(&listaOfertas, ofertas);
  }
}
void cria_threads(int nthread)
{
  pthread_barrier_init(&barreiraTodosProntos,NULL,(nthread+1));
  pthread_cond_init(&condCaixa, NULL);
  int rc;
  pthread_t threads[nthread+1];
  fimAtendentes = nthread;

  rc = pthread_create(&threads[nthread], NULL, (void *) caixa, NULL);
  if(rc != 0)
    printf("Erro na criacao da Thread caixa:\nErro #%d\n", rc);
  for(long i = 0; i < nthread; i ++)
  {
    rc = pthread_create(&threads[i], NULL, (void *) atendente, (void *) (i+1));
    if(rc != 0)
      printf("Erro na criacao da Thread Atendente %ld\nErro: #%d\n", (i+1), rc);
  }

  for(int i = 0; i <= nthread; i ++)
  {
    //printf("%i\n", i);
    rc = pthread_join(threads[i], NULL);
  }
  pthread_barrier_destroy(&barreiraTodosProntos);

  //printf("Crio3u?");

  //printf("Criou?");

}
void imprime_relatorio_financeiro(struct NoCaixa * lista)
{
  printf("***** Relatorio financeiro *****\n\n");
  printf("Atendente   Pedidos   Valor (R$)\n");
  while(lista != NULL)
  {
    printf(fmtfinanc, lista->pedido.idAtendente, lista->pedido.quantidade, lista->pedido.preco);
    lista = lista->next;
  }
}
void imprime_valor_total(struct NoCaixa * lista)
{
  unsigned int i  = 0;
  while(lista != NULL)
  {
    i+= lista->pedido.preco;
    lista = lista->next;
  }
  printf("Receita total: R$ %u\n\n", i);

}

void imprime_estoque(struct No * lista)
{
  printf("***** Estoque de lanches *****\n\n");
  printf("Lanche            Inicial      Final\n");
  while(lista != NULL)
  {
    printf(fmtestoque, lista->sanduiche.nome, lista->sanduicheAntes.quantidade, lista->sanduiche.quantidade);
    lista = lista->next;
  }
}

int main(int argc, char *argv[])
{
  int nthread = atoi(argv[1]);
  char *nomeArquivo = argv[2]; // Recebe o nome do arquivo por parametro
  FILE *arq;
  strcpy(auxNomeArquivo, nomeArquivo);
  strcat(auxNomeArquivo, "-");
  if((arq = fopen(nomeArquivo, "r")) == NULL)
  {
      perror("Erro: fopen");
      exit(EXIT_FAILURE);
  }
  inicializa_lanches(arq);

//  printf("--------------------THREADS--------------------\n");
  cria_threads(nthread);
  imprime_relatorio_financeiro(listaPedidosProcessados);
  imprime_valor_total(listaPedidosProcessados);
  imprime_estoque(listaOfertas);
//  printf("-----------------------------------------------\n");
  //printf("--------------------Lista de Pedidos Que Deu Ruim--------------------\n");
  //printListCaixa(listaCaixa);
  //printf("------------------------------------------------------------------\n");
  //printf("--------------------Lista de Pedidos Processados--------------------\n");
  //printListCaixa(listaPedidosProcessados);
  //printf("------------------------------------------------------------------\n");
  //printf("-----------------Estoque Inicio-------------------------------\n");
//  printList(listaOfertas, LISTA_INICIO);
  //printf("-----------------------------------------------\n");
  //printf("-----------------Estoque Final-----------------\n");
  //printList(listaOfertas, LISTA_FIM);
  //printf("-----------------------------------------------\n");
  pthread_cond_destroy(&condCaixa);
  pthread_mutex_destroy(&mtxCaixa);
  pthread_mutex_destroy(&mtxEstoque);


}
