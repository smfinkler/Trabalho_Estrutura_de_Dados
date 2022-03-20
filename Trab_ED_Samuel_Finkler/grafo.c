#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "grafo.h"

struct aresta{
    int peso;
    int valor_ida;
    int valor_volta;
    int verificacao_caminho;  //se for igual a zero depois do teste faz parte do caminho crítico
    Vertice * vertice_saida;
    struct aresta * prox_aresta;
};

struct vertice{
    char nome[50];
    Item * primeiro_item;  //Vértices alcançaveis pelo vértice atual
    Vertice * prox_vertice;
    struct aresta * primeira_aresta;
    struct aresta * ultima_aresta;
};

struct grafo{
    Vertice * primeiro_vertice;
    Vertice * ultimo_vertice;
};

//Estrutura para guardar todos os vértices que podem ser alcançados por um dado vértice
struct item{
    Vertice* vertice;
    struct item  * prox_item;
};

//funço para criação de um grafo
Grafo * grafo_cria(){
    Grafo * grafo = (Grafo *) malloc(sizeof(Grafo));
    grafo->primeiro_vertice = NULL;
    grafo->ultimo_vertice = NULL;
    return grafo;
}

//Função para liberar a memória ocupada pelo grafo
void grafo_libera (Grafo * grafo){
  int qtVertices= grafo_qtd_vertices(grafo);
  Vertice* vertice_atual;
  Vertice* vertice_aux;
  Aresta* aresta_atual;
  Aresta* aresta_aux;
  vertice_atual = grafo->primeiro_vertice;
  for(int i=0; i<qtVertices; i++){
    aresta_atual = vertice_atual->primeira_aresta;
    while(aresta_atual!=NULL){
      aresta_aux = aresta_atual->prox_aresta;
      free(aresta_atual);
      aresta_atual=aresta_aux;
    }
    vertice_aux = vertice_atual->prox_vertice;
    free(vertice_atual);
    vertice_atual=vertice_aux;
  }
  free(grafo);
}

//Função para criar e retornar ítem
Item * adiciona_item(Item* item, Vertice * vertice){
    Item * novo_item = (Item *)malloc(sizeof(Item));
    novo_item->vertice = vertice;
    novo_item->prox_item = item;
    return novo_item;
}

//Função para buscar um ítem da lista de alcance
Item * busca_item(Item * item, Vertice * vertice){
    while (item!=NULL){
        if (item->vertice == vertice)
            return item;
        item = item->prox_item;
    }
    return NULL;
}

//Função de busca em profundidade alterada para mapear vértices alcançaveis por um dado vértice
static Item* dfs(Grafo * grafo, Vertice * vertice, Item* item){
  int cont = 0;
  Vertice ** v_dest = grafo_busca_vertices_saida(grafo, vertice, &cont);

  for(int i=0;i<cont;i++){
    item = dfs(grafo, v_dest[i], item);
    if(busca_item(item, v_dest[i])==NULL){
      item = adiciona_item(item, v_dest[i]);
    }
  }
  return item;
}

//Função para fins de teste, para verificar se o mapeamento de alcançe ficou correto
/*static void imprimir_itens_alcance(Vertice* vertice){
  Item* item = vertice->primeiro_item;
  while(item!=NULL){
    printf("\n%s", item->vertice->nome);
    item= item->prox_item;
  }
}*/

//retorna 1 se um vértice possui alcance até o outro
static int grafo_verifica_alcance(Grafo* grafo, Vertice* v1, Vertice* v2){
  if(v1==v2){
    return 1;
  }

  Item* item = v1->primeiro_item;
  while(item!=NULL){
    if(item->vertice==v2 || item->vertice==v1){
      return 1;
    }
    item = item->prox_item;
  }
  item = v2->primeiro_item;
  while(item!=NULL){
    if(item->vertice==v1 || item->vertice==v2){
      return 1;
    }
    item = item->prox_item;
  }
  return 0;
}

//Função para retornar a quantidade de vértices do grafo
int grafo_qtd_vertices (Grafo * grafo){
    Vertice * vertice = grafo->primeiro_vertice;
    int qtd = 0;
    while (vertice!=NULL){
        qtd++;
        vertice = vertice->prox_vertice;
    }
    return qtd;
}

//Função para retornar vértices de saída de um dado vértice
Vertice ** grafo_busca_vertices_saida(Grafo * grafo, Vertice * vertice, int * cont){
    int num_arestas =0;
    Aresta * aresta = vertice->primeira_aresta;
    while (aresta!=NULL){
        num_arestas++;
        aresta = aresta->prox_aresta;
    }
    Vertice ** vertices = (Vertice **)malloc(sizeof(Vertice * ) * num_arestas);
    int num_aresta = 0;
    aresta = vertice->primeira_aresta;
    while (aresta!=NULL && num_aresta<num_arestas){
        vertices[num_aresta] = aresta->vertice_saida;
        aresta = aresta->prox_aresta;
        num_aresta++;
    }
    *cont = num_arestas;
    return vertices;
}

//Função para retornar vértices de entrada de um dado vértice
Vertice ** grafo_busca_vertices_entrada(Grafo * grafo, Vertice * vertice, int * cont){
    Vertice* vertice_atual = grafo->primeiro_vertice;
    Aresta* aresta_atual = NULL;
    int contador_arestas_saida=0;

    while(vertice_atual!=NULL){
      aresta_atual=vertice_atual->primeira_aresta;
      while(aresta_atual!=NULL){
        if(aresta_atual->vertice_saida == vertice){
          contador_arestas_saida++;
          break;
        }
        aresta_atual=aresta_atual->prox_aresta;
      }
      vertice_atual=vertice_atual->prox_vertice;
    }

    Vertice ** vertices = (Vertice **)malloc(sizeof(Vertice*) * contador_arestas_saida);

    vertice_atual = grafo->primeiro_vertice;
    int aux_cont=0;
    while(vertice_atual!=NULL && aux_cont<contador_arestas_saida){
      aresta_atual=vertice_atual->primeira_aresta;
      while(aresta_atual!=NULL){
        if(aresta_atual->vertice_saida == vertice){
          vertices[aux_cont] = vertice_atual;
          aux_cont++;
        }
        aresta_atual = aresta_atual->prox_aresta;
      }
      vertice_atual=vertice_atual->prox_vertice;
    }
    *cont = contador_arestas_saida;
    return vertices;
}

//Função para encontrar e retornar o vértice fonte do grafo
Vertice * grafo_busca_vertice_fonte(Grafo * grafo){
  int cont=0;
  Vertice * vertice = grafo->primeiro_vertice;
  while(vertice!=NULL){
    grafo_busca_vertices_entrada(grafo, vertice, &cont);
    if(cont==0){
      return vertice;
    }
    vertice = vertice->prox_vertice;
  }
  return NULL;
}

//Função para encontrar e retornar o vértice sumidouro do grafo
Vertice * grafo_busca_vertice_sumidouro(Grafo * grafo){
  int cont=0;
  Vertice * vertice = grafo->primeiro_vertice;
  while(vertice!=NULL){
    grafo_busca_vertices_saida(grafo, vertice, &cont);
    if(cont==0){
      return vertice;
    }
    vertice = vertice->prox_vertice;
  }
  return NULL;
}


//Função para resetar os valores padrão dos atributos das arestas usados para achar o maior caminho
void grafo_resetar_arestas(Grafo* grafo){
  Vertice* vertice = grafo->primeiro_vertice;
  Aresta* aresta = NULL;
  while(vertice!=NULL){
    aresta = vertice->primeira_aresta;
    while(aresta!=NULL){
      aresta->valor_ida=0;
      aresta->valor_volta=INT_MAX;
      aresta->verificacao_caminho=-1;

      aresta = aresta->prox_aresta;
    }
    vertice=vertice->prox_vertice;
  }
}

//Função para inserir uma aresta entre dois vértices
void grafo_insere_aresta (Grafo * grafo, Vertice * vertice1, Vertice * vertice2, int peso){
    Aresta * aresta = (Aresta*) malloc(sizeof(Aresta));
    aresta->peso = peso;
    aresta->valor_ida= 0;
    aresta->valor_volta = INT_MAX;
    aresta->verificacao_caminho = -1;
    aresta->vertice_saida = vertice2;
    aresta->prox_aresta = NULL;

    if (vertice1->ultima_aresta!=NULL){
        vertice1->ultima_aresta->prox_aresta = aresta;
        vertice1->ultima_aresta = aresta;
    }else{
        vertice1->primeira_aresta = aresta;
        vertice1->ultima_aresta = aresta;
    }
}

//Função para buscar uma aresta entre dois vértices
Aresta * grafo_busca_aresta(Grafo * grafo, Vertice * vertice1, Vertice * vertice2){
    Aresta* aresta_atual = vertice1->primeira_aresta;
    while(aresta_atual!=NULL){
      if(aresta_atual->vertice_saida == vertice2){
        return aresta_atual;
      }
      aresta_atual=aresta_atual->prox_aresta;
    }
    return NULL;
}

//Função para remover uma aresta entre dois vértices
void grafo_remove_aresta (Grafo* grafo, Vertice * vertice1, Vertice * vertice2){
  Aresta* ant = NULL;
  Aresta* aresta = vertice1->primeira_aresta;
  while(aresta!=vertice1->ultima_aresta){
    ant = aresta;
    aresta = aresta->prox_aresta;
  }
  if(ant==NULL){
    vertice1->primeira_aresta = NULL;
    vertice1->ultima_aresta = NULL;
  }else{
    ant->prox_aresta=NULL;
    vertice1->ultima_aresta = ant;
  }
  free(aresta);
}

//Funcão de criação de vértice
Vertice * grafo_cria_vertice(Grafo * grafo, const char nome[]){
    Vertice * v = (Vertice *)malloc(sizeof(Vertice));
    strcpy(v->nome, nome);
    v->primeiro_item = NULL;
    v->prox_vertice = NULL;
    v->primeira_aresta = NULL;
    v->ultima_aresta = NULL;

    if (grafo->ultimo_vertice!=NULL){
        grafo->ultimo_vertice->prox_vertice = v;
        grafo->ultimo_vertice = v;
    }else{
        grafo->primeiro_vertice = v;
        grafo->ultimo_vertice = v;
    }
    return v;
}

Vertice * grafo_retorna_vertice(Grafo * grafo, int indice){
    Vertice * vertice = grafo->primeiro_vertice;
    int qtd = 0;
    while (vertice!=NULL){
        if (qtd == indice)
            return vertice;
        qtd++;
        vertice = vertice->prox_vertice;
    }
    return NULL;
}

//Função para buscar e retornar um vértice de acordo com seu nome
Vertice * grafo_retorna_vertice_por_nome(Grafo * grafo, char* nome){
    Vertice * vertice = grafo->primeiro_vertice;
    while (vertice!=NULL){
        if (!strcmp(nome, vertice->nome))
            return vertice;
        vertice = vertice->prox_vertice;
    }
    return NULL;
}

//Função para retornar nome de um vértice
char * grafo_retorna_nome(Vertice * vertice){
    return vertice->nome;
}

//Função de teste usada para imprimir todos os caminhos do grafo
void grafo_imprime(Grafo * grafo, Vertice * verticeFonte){
  Aresta* aresta = verticeFonte->primeira_aresta;
  while(aresta != NULL){
      printf("\n%s -> %s (%d)", verticeFonte->nome, aresta->vertice_saida->nome, aresta->peso);
      grafo_imprime(grafo, aresta->vertice_saida);
    aresta = aresta->prox_aresta;
  }
}

//Função para imprimir o maior caminho
void grafo_imprime_maior_caminho(Grafo * grafo, Vertice * verticeFonte){
  Aresta* aresta = verticeFonte->primeira_aresta;
  while(aresta != NULL){
    if(aresta->verificacao_caminho==0){
      printf("%s -> %s (%d)  |  ", verticeFonte->nome, aresta->vertice_saida->nome, aresta->peso);
      grafo_imprime_maior_caminho(grafo, aresta->vertice_saida);
    }
    aresta = aresta->prox_aresta;
  }
}

int grafo_maior_caminho_volta(Grafo * grafo, Vertice * verticeFonte, Vertice * verticeSumidouro, int idaCaminho){
  int voltaCaminho=0, maiorCaminho=0;
  int cont = 0;
  Vertice ** v_dest = grafo_busca_vertices_saida(grafo, verticeFonte, &cont);
  Aresta * aresta = NULL;

  if(verticeFonte==verticeSumidouro){
    return idaCaminho;
  }

  for (int i=0;i<cont;i++){
    voltaCaminho = grafo_maior_caminho_volta(grafo, v_dest[i], verticeSumidouro, idaCaminho);
    aresta = grafo_busca_aresta(grafo, verticeFonte, v_dest[i]);

    if(voltaCaminho < aresta->valor_volta){
      aresta->valor_volta = voltaCaminho;
      aresta->verificacao_caminho = aresta->valor_ida-aresta->valor_volta;
    }
    if(aresta->verificacao_caminho == 0){
      maiorCaminho=voltaCaminho-aresta->peso;
    }
  }
  return maiorCaminho;
}

int grafo_maior_caminho_ida(Grafo * grafo, Vertice * verticeFonte, Vertice * verticeSumidouro, int idaCaminho){
  int voltaCaminho=0, maiorCaminho=0;
  int cont = 0;
  Vertice ** v_dest = grafo_busca_vertices_saida(grafo, verticeFonte, &cont);
  Aresta * aresta = NULL;

  if(verticeFonte==verticeSumidouro){
    return idaCaminho;
  }
  for (int i=0;i<cont;i++){
    aresta = grafo_busca_aresta(grafo, verticeFonte, v_dest[i]);
    if(aresta->valor_ida < idaCaminho+aresta->peso){
      aresta->valor_ida = idaCaminho+aresta->peso;
    }
    voltaCaminho = grafo_maior_caminho_ida(grafo, v_dest[i], verticeSumidouro, aresta->valor_ida);
    if(voltaCaminho > maiorCaminho){
      maiorCaminho = voltaCaminho;
    }
  }
  return maiorCaminho;
}

void grafo_teste_insercao_custo0(Grafo* grafo, Vertice* verticeFonte, Vertice* verticeSumidouro){
  int caminhoCritico = grafo_maior_caminho_ida(grafo, verticeFonte, verticeSumidouro, 0);
  grafo_maior_caminho_volta(grafo, verticeFonte, verticeSumidouro, caminhoCritico);
  grafo_resetar_arestas(grafo);
  int testeCaminhoCritico = 0, testeAlcance=0;
  Vertice* v1=verticeFonte;
  Vertice* v2=NULL;
  Item* item = NULL;

  while(v1!=NULL){
    item = v1->primeiro_item;
    v1->primeiro_item = dfs(grafo, v1, item);
    v1 = v1->prox_vertice;
  }
  v1=verticeFonte;

  /*printf("\n\nLISTA DE ALCANÇE");
  while(v1!=NULL){
    printf("\nAo alcance de %s", v1->nome);
    imprimir_itens_alcance(v1);
    v1 = v1->prox_vertice;
  }
  v1=verticeFonte;*/

  printf("\n\n");
  while(v1!=NULL){
    v2 = verticeFonte;
    while(v2!=NULL){
      testeAlcance = grafo_verifica_alcance(grafo, v1, v2);
      if(testeAlcance==0){
        grafo_insere_aresta(grafo, v1, v2, 0);
        testeCaminhoCritico = grafo_maior_caminho_ida(grafo, verticeFonte, verticeSumidouro, 0);
        grafo_maior_caminho_volta(grafo, verticeFonte, verticeSumidouro, testeCaminhoCritico);
        if(testeCaminhoCritico>caminhoCritico){
          printf("\nNovo caminho cŕitico com custo 0 inserido entre %s e %s", grafo_retorna_nome(v1), grafo_retorna_nome(v2));
          printf("\nNovo caminho crítico: %d", testeCaminhoCritico);
          printf("\n");
          grafo_imprime_maior_caminho(grafo, verticeFonte);
          printf("\n");
        }
        grafo_resetar_arestas(grafo);
        grafo_remove_aresta(grafo, v1, v2);
      }
      v2 = v2->prox_vertice;
    }
    v1 = v1->prox_vertice;
  }
}

//Função para ler arquivo, inserir arestas e vértices no grafo
void lerArquivo(Grafo* grafo){
  Vertice* verticeOrigem = NULL;
  Vertice* verticeDestino = NULL;
  char tarefaOrigem[50], tarefaDestino[50];
  int custo;

  FILE *file;
  file = fopen("Rede.txt", "r");

  if (file == NULL){
    printf("\nERRO! O arquivo não foi aberto!\n");
    exit(1);
  }else{
     printf("\nO arquivo foi aberto com sucesso!");
  }

  while(fscanf(file,"\n%50[^,] ,%50[^,] ,%d", tarefaOrigem, tarefaDestino, &custo)!=EOF){
      verticeOrigem = grafo_retorna_vertice_por_nome(grafo, tarefaOrigem);
      if(verticeOrigem==NULL){
        verticeOrigem = grafo_cria_vertice(grafo, tarefaOrigem);
      }
      verticeDestino = grafo_retorna_vertice_por_nome(grafo, tarefaDestino);
      if(verticeDestino==NULL){
        verticeDestino = grafo_cria_vertice(grafo, tarefaDestino);
      }
      grafo_insere_aresta(grafo, verticeOrigem, verticeDestino, custo);
      printf("\n%s,%s,%d", tarefaOrigem, tarefaDestino, custo);
  }

  fclose(file);
}
