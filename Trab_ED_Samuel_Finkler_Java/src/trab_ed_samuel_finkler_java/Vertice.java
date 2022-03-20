
package trab_ed_samuel_finkler_java;

import java.util.ArrayList;

public class Vertice {
    String nome;
    ArrayList<Aresta> lista_arestas = new ArrayList<>();
    ArrayList<Item> itens_saida = new ArrayList<>();

    public Vertice() {
    }
    
    public Vertice(String nome) {
        this.nome = nome;
    }

    public String getNome() {
        return nome;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }

    public ArrayList<Aresta> getLista_arestas() {
        return lista_arestas;
    }

    public void setLista_arestas(ArrayList<Aresta> lista_arestas) {
        this.lista_arestas = lista_arestas;
    }

    public ArrayList<Item> getItens_saida() {
        return itens_saida;
    }

    public void setItens_saida(ArrayList<Item> itens_saida) {
        this.itens_saida = itens_saida;
    }  
    
    public void insere_aresta(Vertice destino, int peso){
        Aresta aresta = new Aresta(peso, destino);
        this.lista_arestas.add(aresta);
    }
    
    public void remove_aresta(Vertice destino){
        int i=0;
        for(i=0; i<lista_arestas.size(); i++){
            if(lista_arestas.get(i).vertice_saida==destino){
                Aresta aresta = lista_arestas.get(i);
                this.lista_arestas.remove(aresta);
            }
        }
    }
    
    public Aresta busca_aresta(Vertice destino){
        int i=0;
        for(i=0; i<lista_arestas.size(); i++){
            if(lista_arestas.get(i).vertice_saida==destino){
                return lista_arestas.get(i);
            }
        }
        return null;
    }
    
    public void adiciona_item(Vertice vertice){
        Item item = new Item(vertice);
        this.itens_saida.add(item);
    }
    
    public Item busca_item(Vertice vertice){
        int i=0;
        for(i=0; i<itens_saida.size(); i++){
            if(itens_saida.get(i).vertice==vertice){
                return itens_saida.get(i);
            }
        }
        return null;
    }
    
}
