
<h3>Controle de concorrencia</h3>
<p>
Para fazer o controle foi utilizado neste projeto, dois mutexes, para o controle de duas Regioes Critica, uma eh a lista de lanches em Estoque (listaOfertas -> a relacao de lanches no arquivo de ofertas) e outra  lista de pedidos a serem processados pelo caixa (listaCaixa -> Corresponde aos pedidos ainda nao processados).
Alem disso, foi utilizado uma barreira para a garantia de que todas as Threads, tanto a de caixa quanto os atendentes estejam prontas (barreiraTodosProntos).
Por fim, foi utilizado uma variavel condicional para que os atendentes sinalizassem o caixa toda vez que inserirem algo novo a lista de pedidos que precisam ser processados, e tambem ao final de sua execucao para a garantia de que o caixa nao esteja dormindo e um inteiro representando o numero de thread de Atendentes que faltam terminar seus trabalhos, assim o caixa sabe que deve repetir suas execucoes enquanto tiver um atendente trabalhando (fimAtendentes). </p>

<h3>Threads Atendentes</h3>

* A thread atendente eh criada na Cria_Threads().
* Ela recebe um parametro que eh o seu id e nao retorna nada
* Quando executada ela faz as seguintes acoes:
    * Criar uma string com o nome do arquivo do pedido da qual ela precisa ler (nomeArquivo-idThread)
  * Ela abre o arquivo e o atribui a um ponteiro do tipo File
  * Fica presa no espera_barreira(), ate que todas as outras threads estejam no mesmo ponto
  * Apos a barreira ser destruida, ela le linha por linha do arquivo ate o seu fim (EOF)
  * Cada linha do arquivo de pedido eh atribuido a uma variavel tipo Lanche, com a quantidade de lanches daquele pedido e o nome do lanche desejado.
  * Nesse ponto, a thread tranca o Mutex responsavel pela lista de Estoque (mtxEstoque)
    * Por que o mutex de estoque tranca aqui e por que o uso de dois mutex? Um para caixa e outro para estoque?
      * Bom, as regioes criticas sao diferentes, se eu crio dois mutexes evito o tempo ocioso na thread, ja que o caixa que quer mexer com a Lista de Pedidos a serem processados nao precisa esperar o atendente que ta verificando o estoque.
      * O uso do mutex nesse ponto, por mais que a funcao eh_possivel_processar_pedido nao altere a lista de ofertas, a funcao retorna se eh possivel ou nao executar o pedido, se dois atendentes processam o mesmo lanche ao mesmo tempo vao receber o mesmo retorno de que eh possivel e vai da ruim hahaha.
  * Ela executa a funcao eh_possivel_processar_pedido e atribui o seguinte retorno a um inteiro
    * eh_possivel_processar_pedido, percorre a lista de Ofertas e tenta encontrar um lanche em estoque com o mesmo do lanche enviado por parametro, seu retorno eh:
      * 1 caso seja possivel executar o pedido
      * 0 caso a quantidade em estoque seja insuficiente
      * -1 caso o lanche nao esteja listado na lista de ofertas
  * A thread apos receber o retorno == 1, executa a retira_lanches_estoque, atribuindo a uma variavel valor
    * Essa funcao percorre a Lista de estoque, encontra o lanche com o mesmo nome do que o lanche enviado por parametro e desconta a quantidade de lanche com a quantidade enviada por parametro, alem disso ela retorna o valor/preco daqueles lanches
      * por exemplo se eu pedir 4 big mac e cada um custa 8 reais, ao enviar para retira_lanches_estoque, ela vai me retornar 32, que eh o valor do 4 * o preco do bigmac
  * Agora a thread vai trancar o mutex da Caixa
    * como explicado ali em cima o motivo de dois mutexes
  * vai dar um push/append na lista de Pedidos que precisam ser processados pelo Caixa, o ultimo parametro da funcao append, indica que o contador tamanhodaListaCaixa deve ser incrementado, pois a listaPedidosProcessados utiliza a mesma funcao.
  * vai mandar um sinal acordando o caixa caso ele esteja preso no loop tamanhoListaCaixa == 0
  * vai destravar o mutex da Caixa
  * vai destravar o mutex do Estoque
  * #AREA DO XGH -- Serio se tirar isso o codigo da pau e eu n sei explicar por que, tem que aceitar
    * Tranca o mutex caixa, ja que a regiao critica a seguir eh referente ao caixa e aos atendentes
    * Decrementa a variavel que indica o n de threads atendentes em execucao
    * Manda um sinal para a thread caixa, caso ela esteja dormindo
    * Destrava o mutex caixa
  * #Fim DO XGH
  * Sai da thread com retorno NULL

<h3>Thread caixa</h3>
* A thread Caixa assim como a atendente eh criada na Cria_Threads().
* Ela nao recebe parametro nenhum
* Ela nao retorna nada
* Quando executada ela faz as seguintes acoes
  * Fica presa no espera_barreira(), ate que todas as outras threads estejam no mesmo ponto
  * Entra em um loop enquanto os Atendentes nao acabaram o seu trabalho, ou o tamanho da lista de pedidos a serem processados nao esteja terminada
  * Tranca o mutex caixa, para mexer na area critica
  * Se o tamanho da lista de pedidos a serem processados for igual a 0 e os atendentes ainda nao acabaram o seu trabalho, entao a thread fica presa a espera de um sinal de algum atendentes
  * A thread pega um pedido da lista de pedidos a serem processados atraves da funcao popLista(), que retorna o pedido
  * se esse pedido nao for nulo, ela entra em um loop
    * se buscaListaIncrementaValor(listaPedidosProcessados, * aux) == 0
      * Essa funcao percorre a listaPedidosProcessados, que usa as mesma estrutura de pedido nao processada.
        * Essa funcao busca nessa lista o id do atendente do pedido que ela enviou como parametro, se esse atendente ja esta listado, ela incrementa o preco do pedido enviado por parametor ao preco do pedido ja armazenado, alem disso ela incrementa o contador de pedidos atendidos por aquele idAtendente
          * Caso a funcao nao encontre nenhum atendente com o id do pedido enviado por parametro, ela retorna 0
          * Se ela retorna 0 entao eu dou push/append na listaPedidosProcessados com aquele pedido, e com o ultimo parametro um inteiro 0 para que o contador tamanhoListaCaixa nao seja incrementado
      * Por fim, o ponteiro aux recebe um novo pop, e o loop se repete
  * Apos o fim do loop, o mutex do caixa eh destravado e o loop inicial se repete
  * Entao, a thread retorna NULL caso todos os atendentes tenham terminado seu trabalho e a ListaDeCaixa esteja vazia.


<h3>Compilando o codigo</h3>

gcc -pthread aifood.c -o aifood

<h3>Executando</h3>

./aifood 2 a
