#ifndef HEAP_H
#define HEAP_H

/* ============================================================
   heap.h  –  Min-Heap para Dijkstra
   ============================================================ */

#define HEAP_MAX 1000

typedef struct {
    int    vertice;
    double dist;
} HeapNode;

typedef struct {
    int      tam;
    HeapNode h[HEAP_MAX];
} MinHeap;

void   heapInicializar(MinHeap *hp);
int    heapVazia(const MinHeap *hp);
void   heapInserir(MinHeap *hp, int vertice, double dist);
HeapNode heapExtrairMin(MinHeap *hp);
void   heapDecrecer(MinHeap *hp, int vertice, double novaDist);

#endif /* HEAP_H */
