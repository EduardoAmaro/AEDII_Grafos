#include <stdio.h>
#include <stdlib.h>

struct grafo {
    int vertices;
    int arestas;
    int **adj;
};

typedef struct grafo *Grafo;

Grafo criaGrafo(int vertices);
int **alocaMatriz(int linha, int coluna, int valor);
void insereAresta(Grafo g, int a, int b, int peso);
void imprimeGrafo(Grafo g);
int **geraMatrizVertice(Grafo g);
int calculaMenorCaminhoDijkstra(Grafo grafo, int verticeInicial, int verticeFinal);
int calculaMenorCaminhoGuloso(Grafo grafo, int verticeInicial, int verticeFinal);
int buscaProfundidadeFinal(int *buffer, int verticeInicio, int verticeFinal, Grafo grafo);

int main() {
    int menu, numVert, vertOrigem, vertDestino, peso, resultado, vertInicio, vertFim;

    printf("Numero de vertices\n");
    scanf("%d", &numVert);
    Grafo grafo = criaGrafo(numVert);

    grafo->adj[0][0] = 0;
    grafo->adj[0][1] = 2;
    grafo->adj[0][2] = 3;
    grafo->adj[0][3] = 0;
    grafo->adj[0][4] = 1;

    grafo->adj[1][0] = 0;
    grafo->adj[1][1] = 0;
    grafo->adj[1][2] = 0;
    grafo->adj[1][3] = 5;
    grafo->adj[1][4] = 0;

    grafo->adj[2][0] = 0;
    grafo->adj[2][1] = 0;
    grafo->adj[2][2] = 0;
    grafo->adj[2][3] = 6;
    grafo->adj[2][4] = 0;

    grafo->adj[3][0] = 0;
    grafo->adj[3][1] = 0;
    grafo->adj[3][2] = 0;
    grafo->adj[3][3] = 0;
    grafo->adj[3][4] = 0;

    grafo->adj[4][0] = 0;
    grafo->adj[4][1] = 10;
    grafo->adj[4][2] = 0;
    grafo->adj[4][3] = 2;
    grafo->adj[4][4] = 0;

    do {
        printf("1-Adicionar aresta\n2-Imprimir grafo\n3-Calcular menor caminho (Dijikstra)\n"
                "4-Calcular menor caminho (Algoritmo Guloso)\n0-Sair\n");
        scanf("%d", &menu);
        if (menu == 1) {

            printf("Vertice origem: ");
            scanf("%d", &vertOrigem);
            printf("Vertice destino: ");
            scanf("%d", &vertDestino);
            printf("Peso da aresta: ");
            scanf("%d", &peso);
            insereAresta(grafo, vertOrigem, vertDestino, peso);

        } else if (menu == 2) {
            imprimeGrafo(grafo);
        } else if (menu == 3) {
            printf("Vertice inicial: ");
            scanf("%d", &vertInicio);
            printf("Vertice final: ");
            scanf("%d", &vertFim);

            resultado = calculaMenorCaminhoDijkstra(grafo, vertInicio, vertFim);
            printf("\nMenor Caminho (Dijikstra): %d\n\n", resultado);
        } else if (menu == 4) {
            printf("Vertice inicial: ");
            scanf("%d", &vertInicio);
            printf("Vertice final: ");
            scanf("%d", &vertFim);

            resultado = calculaMenorCaminhoGuloso(grafo, vertInicio, vertFim);
            printf("\nMenor Caminho (Algoritmo Guloso): %d\n\n", resultado);
        }
    } while (menu != 0);

    return (0);
}

Grafo criaGrafo(int vert) {
    Grafo grafo = malloc(sizeof *grafo);
    grafo->vertices = vert;
    grafo->arestas = 0;
    grafo->adj = alocaMatriz(vert, vert, 0);
    return grafo;
}

int **alocaMatriz(int linhas, int colunas, int valor) {
    int **buffer = malloc(linhas * sizeof (int *));
    for (int i = 0; i < linhas; i++) {
        buffer[i] = malloc(colunas * sizeof (int));
    }
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            buffer[i][j] = valor;
        }
    }
    return buffer;
}

void insereAresta(Grafo g, int a, int b, int peso) {
    if (g->adj[a][b] == 0) {
        g->adj[a][b] = peso;
        g->arestas++;
    }
}

void imprimeGrafo(Grafo g) {
    for (int i = 0; i < g->vertices; i++) {
        printf("%2d:", i);
        for (int j = 0; j < g->vertices; j++) {
            if (g->adj[i][j] > 0)
                printf(" %2d -> %d", j, g->adj[i][j]);
        }
        printf("\n");
    }
}

int **geraMatrizVertice(Grafo g) {
    int **buffer = malloc(g->vertices * sizeof (int *));
    for (int i = 0; i < g->vertices; i++) {
        buffer[i] = malloc(3 * sizeof (int));
    }
    for (int i = 0; i < g->vertices; i++) {
        //coluna 0 - distancia
        buffer[i][0] = 999;
        //coluna 1 - indice pai
        buffer[i][1] = -1;
        //coluna 2 - vertice ja verificado (0 | 1)
        buffer[i][2] = 0;
    }
    return buffer;
}

int calculaMenorCaminhoDijkstra(Grafo grafo, int verticeInicial, int verticeFinal) {
    int **matriz = geraMatrizVertice(grafo);

    matriz[verticeInicial][0] = 0;

    //navega por todos os vertices
    for (int i = 0; i < grafo->vertices; i++) {

        int paiMin = -1;
        int menorValor = 9999;

        //navega por todas as estimações
        for (int j = 0; j < grafo->vertices; j++) {
            //nao visitado e menor valor
            if (!matriz[j][2] && matriz[j][0] < menorValor) {
                paiMin = j;
                menorValor = matriz[j][0];
            }
        }

        //marca como visitado
        matriz[paiMin][2] = 1;

        //verifica se chegou ao fim do caminho
        if (paiMin == verticeFinal) {
            return matriz[paiMin][0];
        }

        for (int j = 0; j < grafo->vertices; j++) {
            //vê se é filho
            if (grafo->adj[paiMin][j] > 0) {
                //estimado pai + peso aresta pai filho < estimado filho
                if (matriz[paiMin][0] + grafo->adj[paiMin][j] < matriz[j][0]) {
                    //estiamdo filho = estimado pai + aresta pai filho
                    matriz[j][0] = matriz[paiMin][0] + grafo->adj[paiMin][j];
                    matriz[j][1] = paiMin;
                }
            }
        }
    }
}

int calculaMenorCaminhoGuloso(Grafo grafo, int verticeInicial, int verticeFinal) {
    int soma = 0;
    int posVertAtual = verticeInicial;
    
    //vetor para marcar visitado na função para ver se chega ao final
    int *buffer = malloc(grafo->vertices * sizeof (int));
    int menorAdja, posMenorAdja;

    while (posVertAtual != verticeFinal) {
        //zera teste menor adjacente
        menorAdja=0;
        
        //percorre adjacentes
        for (int j = 0; j < grafo->vertices; j++) {
            
            
            //zera vetor visitado
            for (int i = 0; i < grafo->vertices; i++) {
                buffer[i] = 0;
            }
    
            //verifica se é filho
            if (grafo->adj[posVertAtual][j] != 0) {
                //verifica se chega até o vertice final
                int chegaFinal = buscaProfundidadeFinal(buffer,j,verticeFinal,grafo);
                if(chegaFinal){
                    if(grafo->adj[posVertAtual][j] < menorAdja || menorAdja==0){
                        menorAdja=grafo->adj[posVertAtual][j];
                        posMenorAdja=j;
                    } 
                }           
            }
        }
        
        soma+=menorAdja;
        posVertAtual = posMenorAdja;
        
    }
    return soma;
}

int buscaProfundidadeFinal(int *buffer, int verticeInicio, int verticeFinal, Grafo grafo) {
    buffer[verticeInicio] = 1;
    int posFinal = 0;

    for (int i = 0; i < grafo->vertices; i++) {
        //verifica se é filho
        if (grafo->adj[verticeInicio][i] != 0) {
            if (!buffer[grafo->adj[verticeInicio][i]]) {
                posFinal = i;
                buscaProfundidadeFinal(buffer, i, verticeFinal, grafo);
            }
        }
    }
    
    if (posFinal = verticeFinal) {
        return 1;
    } else {
        return 0;
    }
}