#ifndef DIJKSTRA_H
#define DIJKSTRA_H

/* ============================================================
   dijkstra.h  –  Caminho mais curto (Dijkstra com Min-Heap)
   ============================================================ */

#include "grafo.h"
#include "modelos.h"

#define SEM_CAMINHO -1

/* Calcula distâncias mínimas a partir de 'origem' (índice no grafo).
   dist[i]  = distância mínima até vértice i (DBL_MAX se inacessível)
   prev[i]  = índice do vértice anterior no caminho (-1 se raiz/inacessível) */
void dijkstra(Grafo *g, int origem, double dist[], int prev[]);

/* Reconstrói o caminho de 'origem' a 'destino' em caminho[].
   Devolve o número de vértices no caminho (0 se inexistente). */
int reconstruirCaminho(const int prev[], int origem, int destino,
                       int caminho[], int maxLen);

/* Encontra o recinto mais próximo do utilizador.
   Devolve o índice do vértice no grafo, ou -1. */
int recintoMaisProximo(Grafo *g, double latU, double lonU,
                       double *distKm);

#endif /* DIJKSTRA_H */
