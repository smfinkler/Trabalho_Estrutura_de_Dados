#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

int main()
{
  int maiorCaminho=0;
  Grafo * grafo = grafo_cria();
  lerArquivo(grafo);
  Vertice* verticeFonte = grafo_busca_vertice_fonte(grafo);
  Vertice* verticeSumidouro =  grafo_busca_vertice_sumidouro(grafo);
  maiorCaminho = grafo_maior_caminho_ida(grafo, verticeFonte, verticeSumidouro, 0);
  grafo_maior_caminho_volta(grafo, verticeFonte, verticeSumidouro, maiorCaminho);
  printf("\n");
  grafo_imprime_maior_caminho(grafo, verticeFonte);
  grafo_resetar_arestas(grafo);
  printf("\nMaior caminho inicial: %d", maiorCaminho);
  grafo_teste_insercao_custo0(grafo, verticeFonte, verticeSumidouro);
  return 0;
}
