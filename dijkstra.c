#include <float.h>
#include <string.h>
#include <stdio.h>
#include "dijkstra.h"
#include "heap.h"
#include "haversine.h"

/* ============================================================
   dijkstra.c  –  Implementação do Algoritmo de Dijkstra
   ============================================================ */

void dijkstra(Grafo *g, int origem, double dist[], int prev[]) {
    int n = g->n;
    MinHeap hp;
    heapInicializar(&hp);

    /* Inicialização */
    for (int i = 0; i < n; i++) {
        dist[i] = DBL_MAX;
        prev[i] = -1;
    }
    dist[origem] = 0.0;
    heapInserir(&hp, origem, 0.0);

    while (!heapVazia(&hp)) {
        HeapNode cur = heapExtrairMin(&hp);
        int u = cur.vertice;

        /* Se a distância extraída for maior que a registada, ignora
           (pode acontecer com entradas duplicadas na pilha) */
        if (cur.dist > dist[u]) continue;

        /* Relaxamento das arestas */
        for (Aresta *a = g->v[u].adj; a; a = a->prox) {
            int    v    = a->destino;
            double alt  = dist[u] + a->peso;
            if (alt < dist[v]) {
                dist[v] = alt;
                prev[v] = u;
                heapDecrecer(&hp, v, alt);
            }
        }
    }
}

int reconstruirCaminho(const int prev[], int origem, int destino,
                       int caminho[], int maxLen) {
    /* Contagem reversa */
    int tmp[MAX_V], n = 0;
    int cur = destino;
    while (cur != -1 && n < MAX_V) {
        tmp[n++] = cur;
        if (cur == origem) break;
        cur = prev[cur];
    }
    if (cur != origem) return 0;   /* sem caminho */

    /* Inverte para ordem origem -> destino */
    int total = (n < maxLen) ? n : maxLen;
    for (int i = 0; i < total; i++)
        caminho[i]  = tmp[n - 1 - i];
    return total;
}

int recintoMaisProximo(Grafo *g, double latU, double lonU, double *distKm) {
    int    melhorIdx  = -1;
    double melhorDist = DBL_MAX;

    for (int i = 0; i < g->n; i++) {
        if (strcmp(g->v[i].tipo, "recinto") != 0) continue;
        double d = haversine(latU, lonU,
                             g->v[i].latitude, g->v[i].longitude);
        if (d < melhorDist) {
            melhorDist = d;
            melhorIdx  = i;
        }
    }
    if (distKm) *distKm = melhorDist;
    return melhorIdx;
}
