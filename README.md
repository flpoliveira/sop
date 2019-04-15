-> O que o projeto deve ter
* Multiplas threads atendentes (void * atendente())
  * Cada Atendente tem seu conjunto de pedidos (um arquivo)
  * Cada pedido tem o nome do lanche e a quantidade desejada
  * O Atendente verifica se a quantidade desejada esta disponivel.
    * Se a quantidade desejada nao for a que tem no estoque, o pedido eh rejeitado

* Uma thread Caixa, que registra os valores dos pedidos processado pelos atendentes (void * caixa())
* O programa recebe dois parametros "aifood nthr nomearq"
  * nthr -> numero de threado representando o atendentes (n>=1)
  * nomearq -> nome do arquivo contendo os lanches ofertados
    * nomearq-idThread -> nome de cada arquivo de pedido, seguido do id do atendentes
* O lanche possui
  * string nome
  * unsigned int preco
  * unsigned int quantidade
* O arquivo de Ofertas/estoque eh sempre no formato %s %d %d (nome, preco, quantidade)
* Os arquivos de pedidos eh sempre no formato %s %d (nome, quantidade)
* A main deve fazer o seguinte
  * inicializa_lanches(arq_ofertas);
  * cria_threads(); -> Como cada atendente pode pegar o nome do arquivo? Variavel global?
  * espera_fim_threads();-> Como fazer isso?
  * imprime_valor_total();
  * imprime_estoque();

* A thread ATENDENTE deve esperar todas estarem prontas (barreira)
  * A thread deve processar seus pedidos, um por vez (Ler linha do arquivo)
    * Caso um pedido possa ser atendido, o numero de lanches deve ser retirado do estoque
      * O idAtendente e o valor do pedido deve ser informados a thread Caixa
        * Comunicacao com a thread caixa via signal

  sinaliza_thread_pronta_e_espera_demais();
  enquanto(houver pedidos a processar){
    pedido = le_prox_linha_arq_pedidos();
    se (pedido puder ser atendido)
    {
      retira_lanches_estoque();
      enfileira_caixa(id, valor);
    }
  }
  * A thread de caixa processa os pedidos bem sucedidos dos atendentes
    * Depois que todas as threads atendentes encerrarem, imprime um relatorio financeiro, no formato desejado.
  * Imprimir estoque antes e depois.


Compilando o codigo

gcc -c aifood.c -o aifood.o
gcc -c ESTRUTURAS.c -o ESTRUTURAS.o
gcc aifood.o ESTRUTURAS.o -o aifood
