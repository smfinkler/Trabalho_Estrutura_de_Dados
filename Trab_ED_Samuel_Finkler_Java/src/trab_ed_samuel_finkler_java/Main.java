
package trab_ed_samuel_finkler_java;

import java.util.Scanner;

/*
 * @author Samuel Matias
 */
public class Main {

    public static void main(String[] args) {
        Grafo grafo = new Grafo();
        grafo.ler_arquivo();
        Vertice vertice_fonte = grafo.grafo_busca_vertice_fonte();
        Vertice vertice_sumidouro = grafo.grafo_busca_vertice_sumidouro();
        System.out.println("Vértice fonte:"+vertice_fonte.getNome());
        System.out.println("Vértice sumidouro:"+vertice_sumidouro.getNome());
        int maiorCaminho = grafo.grafo_maior_caminho_ida(vertice_fonte, vertice_sumidouro, 0);
        grafo.grafo_maior_caminho_volta(vertice_fonte, vertice_sumidouro, maiorCaminho);
        System.out.println("Maior caminho inicial: "+maiorCaminho);
        grafo.grafo_imprime_maior_caminho(vertice_fonte);
        grafo.grafo_resetar_arestas();
        grafo.grafo_teste_insercao_custo0(vertice_fonte, vertice_sumidouro);  
    }

}
