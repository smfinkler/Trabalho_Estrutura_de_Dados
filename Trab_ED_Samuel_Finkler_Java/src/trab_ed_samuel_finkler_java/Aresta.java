
package trab_ed_samuel_finkler_java;

public class Aresta {
    int peso;
    int valor_ida;
    int valor_volta;
    int verificacao_caminho;
    Vertice vertice_saida;
    
    public Aresta(int peso, Vertice vertice_saida) {
        this.peso = peso;
        this.vertice_saida = vertice_saida;
        this.valor_ida = 0;
        this.valor_volta = Integer.MAX_VALUE;
        this.verificacao_caminho = -1;
    }

    public int getPeso() {
        return peso;
    }

    public void setPeso(int peso) {
        this.peso = peso;
    }

    public int getValor_ida() {
        return valor_ida;
    }

    public void setValor_ida(int valor_ida) {
        this.valor_ida = valor_ida;
    }

    public int getValor_volta() {
        return valor_volta;
    }

    public void setValor_volta(int valor_volta) {
        this.valor_volta = valor_volta;
    }

    public int getVerificacao_caminho() {
        return verificacao_caminho;
    }

    public void setVerificacao_caminho(int verificacao_caminho) {
        this.verificacao_caminho = verificacao_caminho;
    }

    public Vertice getVertice_saida() {
        return vertice_saida;
    }

    public void setVertice_saida(Vertice vertice_saida) {
        this.vertice_saida = vertice_saida;
    }

}



