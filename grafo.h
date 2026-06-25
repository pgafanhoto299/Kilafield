#ifndef GRAFO_H
#define GRAFO_H

/* ============================================================
   grafo.h  –  Grafo dirigido por lista de adjacência
   ============================================================ */

#include "modelos.h"

#define MAX_V MAX_VERTICES

typedef struct Aresta {
    int           destino;
    double        peso;       /* distância em km (Haversine) */
    struct Aresta *prox;
} Aresta;

typedef struct {
    int    id;
    char   nome[MAX_NOME];
    double latitude;
    double longitude;
    char   tipo[20];          /* "recinto" | "intersecao" | "ponto" */
    Aresta *adj;
} Vertice;

typedef struct {
    int     n;                /* número de vértices */
    Vertice v[MAX_V];
} Grafo;

void   inicializarGrafo(Grafo *g);
int    adicionarVertice(Grafo *g, int id, const char *nome,
                        double lat, double lon, const char *tipo);
void   adicionarAresta(Grafo *g, int origem, int destino, double peso);
void   adicionarArestaBidirecional(Grafo *g, int o, int d, double peso);
int    indiceVertice(const Grafo *g, int id);
void   libertarGrafo(Grafo *g);
void   imprimirGrafo(const Grafo *g);

#endif /* GRAFO_H */
