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

int main() {
    int menu,numVert,vertOrigem,vertDestino,peso;

    printf("Numero de vertices\n");
    scanf("%d", &numVert);

    Grafo grafo = criaGrafo(numVert);
    do{
        printf("1-Adicionar aresta\n2-Imprimir grafo\n0-Sair\n");
        scanf("%d", &menu);
        if(menu==1){
            
            printf("Vertice origem: ");
            scanf("%d", &vertOrigem);
            printf("Vertice destino: ");
            scanf("%d", &vertDestino);
            printf("Peso da aresta: ");
            scanf("%d", &peso);
            insereAresta(grafo,vertOrigem,vertDestino,peso);
            
        }else if(menu==2){
            imprimeGrafo(grafo);
        }
    }while(menu!=0);



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
                printf(" %2d -> %d", j,g->adj[i][j]);
        }
        printf("\n");
    }
}