#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define INFINITY INT_MAX
#define NIL -1 // NIL eh usado para indicar que nao ha um vertice anterior

typedef struct graph *Digraph;
#define Graph Digraph
#define MAX_VERT 10000
#define Vertex int

struct graph {
  int Vert;
  int Arcz [MAX_VERT][MAX_VERT];
  int weightArcz [MAX_VERT][MAX_VERT];
  int numArcz;
};

typedef struct {
    int vertex;
    int weight;
} Node;

typedef struct {
    Node* nodes;
    int len;
    int size;
} Heap;

Graph initGraph(int n) {

  Graph G = (Graph)malloc(sizeof(*G));
  G->Vert = n;
  G->numArcz = 0;

  // Inicializacao de todos os arcos pra 0
  for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
          G->Arcz[i][j] = 0;
      }
  }
  return G;
}

void insertArc(Graph G, Vertex z, Vertex y, int weight) {
  // verificar se o arco jah existe
  if (G->Arcz[z][y] == 1) {
    return;
  }
  //verificar se os vertices passados sao validos  
  else if (z >= G->Vert || y >= G->Vert){
    return;
  }

  if (z >= 0 && z < G->Vert && y >= 0 && y < G->Vert){
    G->Arcz[z][y] = 1;
    G->weightArcz[z][y] = weight;
  }

  G->numArcz++;
}

void destroyGraph(Graph G) {
  if (G != NULL) {
    free(G);
  }
}

Heap* createHeap(int size) {
    Heap* h = (Heap*) malloc(sizeof(Heap));
    h->nodes = (Node*) malloc(size * sizeof(Node)); 
    h->len = 0;
    h->size = size;
    return h;
}

void push(Heap* h, int vertex, int weight) {
    Node node;
    node.vertex = vertex;
    node.weight = weight;
    h->nodes[h->len++] = node;
}

Node pop(Heap* h) {
    Node temp, node;

    node = h->nodes[0];

    temp = h->nodes[0];
    h->nodes[0] = h->nodes[h->len - 1];
    h->nodes[h->len - 1] = temp;

    h->len--;

    return node;
}


void dijkstra(int n, int vertSource, int vertTarget, Graph g) { 
    int distance[n], prev[n], visited[n];
    
    // A heap (fila de prioridade) eh usada para armazenar os vertices a serem visitados
    Heap* heap = createHeap(n);

    // Inicializa a distancia de todos os vertices pra infinito, o vertice anterior pra NIL,
    // e todos os vertices como nao visitados.
    for (int i = 0; i < n; i++) {
        distance[i] = INFINITY; 
        prev[i] = NIL;
        visited[i] = false;
    }

    distance[vertSource] = 0;
    push(heap, vertSource, 0);

    // Enquanto a heap nao estiver vazia, remove o vertice com menor distancia
    while (heap->len != 0) {
        Node node = pop(heap);
        int u = node.vertex; 

        visited[u] = true;

        for (int v = 0; v < n; v++) { 
            if (g->Arcz[u][v] != 0 && visited[v] == false) { 
                int newDist = distance[u] + g->weightArcz[u][v]; 

                // Se a nova distancia eh menor que a distancia atual, atualiza a distancia e o vertice anterior
                if (newDist < distance[v]) {
                    distance[v] = newDist;
                    prev[v] = u;
                    push(heap, v, distance[v]);
                }
            }
        }
    }

    int path[n], count = 0;
    int u = vertTarget; 

    // Recupera o caminho percorrido
    while (prev[u] != NIL) { 
        path[count++] = u; 
        u = prev[u];
    }
}

void printResults(int numVertex, int prev[numVertex][numVertex], Graph g) {    
    for (int i = 0; i < numVertex; i++) {
        printf("Node %d:\n", i);
        for (int j = 0; j < numVertex; j++) {
            if (g->weightArcz[i][j] == INFINITY) {
                printf("Distance from %d to %d = infinity\n", i, j);
            } else {
                printf("Distance from %d to %d = %d\n", i, j, g->weightArcz);
                if (j != i) {
                    printf("Path: ");
                    int path[numVertex], count = 0;
                    for (int k = j; k != NIL; k = prev[i][k]) {
                        path[count++] = k;
                    }
                    for (int k = count - 1; k > 0; k--) {
                        printf("%d -> ", path[k]);
                    }
                    printf("%d\n", path[0]);
                }
            }
        }
        printf("\n");
    }
}

int main() {
    
    int numVertex, x, y, weight;
    Graph G = NULL;

    if (scanf("%d\n", &numVertex) == 1) {
        G = initGraph(numVertex);
        while (scanf("%d %d %d\n", &x, &y, &weight) != EOF) {
            insertArc(G, x, y, weight);
        }
    }

    for (int i = 0; i < numVertex; i++) {
        for (int j = 0; j < numVertex; j++) {
            if (i != j) {
                dijkstra(numVertex, i, j, G);
            }
        }
    }

    //printResults(numVertex, prev, G);
    destroyGraph(G);
    return 0;
}