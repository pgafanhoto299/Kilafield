#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "grafo.h"

/* ============================================================
   grafo.c  –  Implementação do Grafo
   ============================================================ */

void inicializarGrafo(Grafo *g) {
    g->n = 0;
    memset(g->v, 0, sizeof(g->v));
}

/* Devolve índice do vértice com id dado, ou -1 se não existe */
int  indiceVertice(const Grafo *g, int id) {
    for (int i = 0; i < g->n; i++)
        if (g->v[i].id == id) return i;
    return -1;
}

/* Adiciona vértice; devolve índice criado ou existente */
int adicionarVertice(Grafo *g, int id, const char *nome,
                     double lat, double lon, const char *tipo) {
    int idx = indiceVertice(g, id);
    if (idx >= 0) return idx;          /* já existe */
    if (g->n >= MAX_V) { fprintf(stderr, "Grafo cheio\n"); return -1; }
    idx = g->n++;
    g->v[idx].id   = id;
    g->v[idx].latitude  = lat;
    g->v[idx].longitude = lon;
    g->v[idx].adj  = NULL;
    strncpy(g->v[idx].nome, nome, MAX_NOME - 1);
    strncpy(g->v[idx].tipo, tipo, 19);
    return idx;
}

void adicionarAresta(Grafo *g, int origem, int destino, double peso) {
    int o = indiceVertice(g, origem);
    int d = indiceVertice(g, destino);
    if (o < 0 || d < 0) {
        fprintf(stderr, "adicionarAresta: vértice não encontrado (%d->%d)\n",
                origem, destino);
        return;
    }
    Aresta *a = malloc(sizeof(Aresta));
    if (!a) { perror("adicionarAresta: malloc"); exit(EXIT_FAILURE); }
    a->destino = d;
    a->peso    = peso;
    a->prox    = g->v[o].adj;
    g->v[o].adj = a;
}

void adicionarArestaBidirecional(Grafo *g, int o, int d, double peso) {
    adicionarAresta(g, o, d, peso);
    adicionarAresta(g, d, o, peso);
}

void libertarGrafo(Grafo *g) {
    for (int i = 0; i < g->n; i++) {
        Aresta *a = g->v[i].adj;
        while (a) {
            Aresta *prox = a->prox;
            free(a);
            a = prox;
        }
        g->v[i].adj = NULL;
    }
    g->n = 0;
}

void imprimirGrafo(const Grafo *g) {
    printf("Grafo com %d vértices:\n", g->n);
    for (int i = 0; i < g->n; i++) {
        printf("  [%d] %s (%.4f, %.4f) tipo=%s -> ",
               g->v[i].id, g->v[i].nome,
               g->v[i].latitude, g->v[i].longitude,
               g->v[i].tipo);
        for (Aresta *a = g->v[i].adj; a; a = a->prox)
            printf("[%d|%.2fkm] ", g->v[a->destino].id, a->peso);
        printf("\n");
    }
}
