
package trab_ed_samuel_finkler_java;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Grafo {
    ArrayList<Vertice> lista_vertices;
    int cont_arestas;

    public Grafo() {
        this.lista_vertices = new ArrayList<Vertice>();
    }

    public ArrayList<Vertice> getLista_vertice() {
        return lista_vertices;
    }

    public void setLista_vertice(ArrayList<Vertice> lista_vertice) {
        this.lista_vertices = lista_vertice;
    }

    public int getCont_arestas() {
        return cont_arestas;
    }

    public void setCont_arestas(int cont_arestas) {
        this.cont_arestas = cont_arestas;
    }
    
    public int grafo_qtd_vertices(){
       return this.lista_vertices.size();
    }
    
    public ArrayList<Vertice> grafo_busca_vertices_saida(Vertice vertice){
        int num_arestas = 0, i=0, j=0;
        ArrayList<Vertice> lista_vertices_saida = new ArrayList<Vertice>();
        for(i=0; i<lista_vertices.size(); i++){
            if(lista_vertices.get(i)==vertice){
                ArrayList<Aresta> lista_arestas = this.lista_vertices.get(i).getLista_arestas();
                for(j=0; j<lista_arestas.size(); j++){
                    lista_vertices_saida.add(lista_arestas.get(j).getVertice_saida());
                    num_arestas++;
                }
            }
        }
        setCont_arestas(num_arestas);
        return lista_vertices_saida;
    }
    
    public ArrayList<Vertice> grafo_busca_vertices_entrada(Vertice vertice){
        int num_arestas = 0, i=0, j=0;
        ArrayList<Vertice> lista_vertices_entrada = new ArrayList<Vertice>();
        for(i=0; i<lista_vertices.size(); i++){
            ArrayList<Aresta> lista_arestas = this.lista_vertices.get(i).getLista_arestas();
            for(j=0; j<lista_arestas.size(); j++){
                if(lista_arestas.get(j).vertice_saida==vertice){
                    System.out.println("entrei no IF");
                    lista_vertices_entrada.add(lista_vertices.get(i));
                    num_arestas++;
                }
            }
        }
        setCont_arestas(num_arestas);
        return lista_vertices_entrada;
    }
    
    public Vertice grafo_busca_vertice_fonte(){
        int entradas=0;
        for(int i=0; i<lista_vertices.size(); i++){
            grafo_busca_vertices_entrada(lista_vertices.get(i));
            entradas = getCont_arestas();
            if(entradas==0){
                return lista_vertices.get(i);
            }
        }
        return null;
    }
    
    public Vertice grafo_busca_vertice_sumidouro(){
        int saidas=0;
        for(int i=0; i<lista_vertices.size(); i++){
            grafo_busca_vertices_saida(lista_vertices.get(i));
            saidas = getCont_arestas();
            if(saidas==0){
                return lista_vertices.get(i);
            }
        }
        return null;
    }
    
    public void grafo_resetar_arestas(){
        int i=0, j=0;
        for(i=0; i<lista_vertices.size()-1; i++){ 
            ArrayList<Aresta> lista_arestas = this.lista_vertices.get(i).getLista_arestas();
            for(j=0; j<lista_arestas.size(); j++){
                lista_arestas.get(j).setValor_ida(0);
                lista_arestas.get(j).setValor_volta(Integer.MAX_VALUE);
                lista_arestas.get(j).setVerificacao_caminho(-1);
            }
        }  
    }
    
    public Aresta grafo_busca_aresta(Vertice origem, Vertice destino){
        return origem.busca_aresta(destino);
    }
    
    public void grafo_remove_aresta(Vertice origem, Vertice destino){
        origem.remove_aresta(destino);
    }
    
    public Vertice grafo_cria_vertice(String nome){
        Vertice vertice = new Vertice(nome);
        this.lista_vertices.add(vertice);
        return vertice;
    }
    
    public Vertice grafo_retorna_vertice_por_nome(String nome){
        int i=0;
        for(i=0; i<lista_vertices.size(); i++){
            if(lista_vertices.get(i).nome.equals(nome)){
                return lista_vertices.get(i);
            }
        }
        return null;
    }
    
    public void grafo_imprime(Vertice verticeFonte){
        int i=0;
        ArrayList<Aresta> listaArestas = verticeFonte.getLista_arestas();
        for(i=0; i<listaArestas.size(); i++){
            System.out.println(""+verticeFonte.nome+" -> "+listaArestas.get(i).getVertice_saida().getNome()+" ("+listaArestas.get(i).getPeso()+")");
            grafo_imprime(listaArestas.get(i).getVertice_saida());
        }
    }
    
    public void grafo_imprime_maior_caminho(Vertice verticeFonte){
        int i=0;
        ArrayList<Aresta> listaArestas = verticeFonte.getLista_arestas();
        for(i=0; i<listaArestas.size(); i++){
            if(listaArestas.get(i).verificacao_caminho==0){
                System.out.println(""+verticeFonte.nome+" -> "+listaArestas.get(i).getVertice_saida().getNome()+" ("+listaArestas.get(i).getPeso()+")");
                grafo_imprime_maior_caminho(listaArestas.get(i).getVertice_saida());
            }
        }
    }
    
    public int grafo_maior_caminho_volta(Vertice verticeFonte, Vertice verticeSumidouro, int idaCaminho){
        
        int voltaCaminho=0, maiorCaminho=0, cont=0, i=0;
        ArrayList<Vertice> v_dest = grafo_busca_vertices_saida(verticeFonte);
        cont = cont_arestas;
        Aresta aresta = null;
        
        if(verticeFonte==verticeSumidouro){
            return idaCaminho;
        }
        
        for(i=0; i<cont; i++){
            voltaCaminho = grafo_maior_caminho_volta(v_dest.get(i), verticeSumidouro, idaCaminho);
            aresta = grafo_busca_aresta(verticeFonte, v_dest.get(i));
            if(voltaCaminho < aresta.getValor_volta()){
                aresta.setValor_volta(voltaCaminho);
                aresta.setVerificacao_caminho(aresta.getValor_ida()-aresta.getValor_volta());
            }
            if(aresta.getVerificacao_caminho()==0){
                maiorCaminho=voltaCaminho-aresta.getPeso();
            }
        }
        return maiorCaminho;
    }
    
    public int grafo_maior_caminho_ida(Vertice verticeFonte, Vertice verticeSumidouro, int idaCaminho){
        int voltaCaminho=0, maiorCaminho=0, cont=0, i=0;
        ArrayList<Vertice> v_dest = grafo_busca_vertices_saida(verticeFonte);
        cont = cont_arestas;
        Aresta aresta = null;
        if(verticeFonte==verticeSumidouro){
            return idaCaminho;
        }
        for(i=0; i<cont; i++){
            aresta = grafo_busca_aresta(verticeFonte, v_dest.get(i));
            if(aresta.getValor_ida() < idaCaminho+aresta.getPeso()){
                aresta.setValor_ida(idaCaminho+aresta.getPeso());
            }
            voltaCaminho = grafo_maior_caminho_ida(v_dest.get(i), verticeSumidouro, aresta.getValor_ida());
            if(voltaCaminho>maiorCaminho){
                maiorCaminho=voltaCaminho;
            }
        }
        return maiorCaminho;
    }
    
    public void imprimir_itens_alcance(Vertice vertice){
        int i=0;
        ArrayList<Item> itens_alcance = vertice.getItens_saida();
        for(i=0; i<itens_alcance.size(); i++){
            System.out.println(""+itens_alcance.get(i).getVertice().getNome());
        }
    }
    
    public void dfs(Vertice vertice, Vertice destino){
        int cont=0, i=0;
        ArrayList<Vertice> v_dest = grafo_busca_vertices_saida(destino);
        cont = cont_arestas;
        
        for(i=0; i<cont; i++){
            dfs(vertice, v_dest.get(i));
            if(vertice.busca_item(v_dest.get(i))==null){
                vertice.adiciona_item(v_dest.get(i));
            }       
        }
    }
    
    private int grafo_verifica_alcance(Vertice v1, Vertice v2){
        int i=0;
        if(v1==v2){
            return 1;
        }
        ArrayList<Item> lista_itens = v1.getItens_saida();
        for(i=0; i<lista_itens.size(); i++){
            if(lista_itens.get(i).vertice == v2  || lista_itens.get(i).vertice == v1){
                return 1;
            }  
        }
        lista_itens = v2.getItens_saida();
        for(i=0; i<lista_itens.size(); i++){
            if(lista_itens.get(i).vertice == v1  || lista_itens.get(i).vertice == v2){
                return 1;
            }
        }
        return 0;
    }
    
    public void grafo_teste_insercao_custo0(Vertice verticeFonte, Vertice verticeSumidouro){
        int i=0, j=0;
        int caminhoCritico = grafo_maior_caminho_ida(verticeFonte, verticeSumidouro, 0);
        grafo_maior_caminho_volta(verticeFonte, verticeSumidouro, caminhoCritico);
        grafo_resetar_arestas();
        int testeCaminhoCritico=0, testealcance=0;
        Item item = null;
        
        for(i=0; i<this.lista_vertices.size(); i++){
            dfs(this.lista_vertices.get(i), this.lista_vertices.get(i));
        }
        
        //Linhas comentadas a seguir são para imprimir a lista de alcance de cada vértice
        /*System.out.println("\nLista de alcançe:");
        for(i=0; i<this.lista_vertices.size(); i++){
            System.out.println("Vertice "+lista_vertices.get(i).getNome());
            this.imprimir_itens_alcance(lista_vertices.get(i));
        }*/
        
        for(i=0; i<this.lista_vertices.size();i++){
            for(j=0; j<this.lista_vertices.size(); j++){
                testealcance = grafo_verifica_alcance(this.lista_vertices.get(i), this.lista_vertices.get(j));
                if(testealcance==0){
                    this.lista_vertices.get(i).insere_aresta(this.lista_vertices.get(j), 0);
                    testeCaminhoCritico = grafo_maior_caminho_ida(verticeFonte, verticeSumidouro, 0);
                    grafo_maior_caminho_volta(verticeFonte, verticeSumidouro, testeCaminhoCritico);
                    if(testeCaminhoCritico>caminhoCritico){
                        System.out.println("\nNovo caminho crítico com custo 0 inserido entre "+this.lista_vertices.get(i).getNome()+" e "+this.lista_vertices.get(j).getNome());
                        System.out.println("Novo caminho crítico:"+testeCaminhoCritico);
                        grafo_imprime_maior_caminho(verticeFonte);
                    }
                    grafo_resetar_arestas();
                    grafo_remove_aresta(this.lista_vertices.get(i), this.lista_vertices.get(j));
                }
            }
        }
    }
    
    void ler_arquivo(){
        Vertice verticeOrigem;
        Vertice verticeDestino;
        String origem, destino;
        int peso, cont=0;
        try {
            Scanner scanner = new Scanner(new FileReader("Rede.txt")).useDelimiter("\\,|\\n");
            while (scanner.hasNext()) {
                cont++;
                origem = scanner.next();
                destino = scanner.next();
                peso = scanner.nextInt();
                
                verticeOrigem = grafo_retorna_vertice_por_nome(origem);
                if(verticeOrigem==null){
                    verticeOrigem = grafo_cria_vertice(origem);
                }
                verticeDestino = grafo_retorna_vertice_por_nome(destino);
                if(verticeDestino==null){
                    verticeDestino = grafo_cria_vertice(destino);
                }
                verticeOrigem.insere_aresta(verticeDestino, peso);
            }
        } catch (FileNotFoundException ex) {
            Logger.getLogger(Grafo.class.getName()).log(Level.SEVERE, null, ex);
        }   
    }
    
}