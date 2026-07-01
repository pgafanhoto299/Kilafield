#include <stdio.h>
#include <float.h>
#include "heap.h"

/* ============================================================
   heap.c  –  Implementação da Min-Heap (por distância)
   ============================================================ */

void trocar(HeapNode *a, HeapNode *b) {
    HeapNode tmp = *a; *a = *b; *b = tmp;
}

void subir(MinHeap *hp, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (hp->h[pai].dist > hp->h[i].dist) {
            trocar(&hp->h[pai], &hp->h[i]);
            i = pai;
        } else break;
    }
}

void descer(MinHeap *hp, int i) {
    int n = hp->tam;
    while (1) {
        int menor = i;
        int esq   = 2 * i + 1;
        int dir   = 2 * i + 2;
        if (esq < n && hp->h[esq].dist < hp->h[menor].dist) menor = esq;
        if (dir < n && hp->h[dir].dist < hp->h[menor].dist) menor = dir;
        if (menor == i) break;
        trocar(&hp->h[i], &hp->h[menor]);
        i = menor;
    }
}

void heapInicializar(MinHeap *hp) {
    hp->tam = 0;
}

int heapVazia(const MinHeap *hp) {
    return hp->tam == 0;
}

void heapInserir(MinHeap *hp, int vertice, double dist) {
    if (hp->tam >= HEAP_MAX) {
        fprintf(stderr, "Heap cheia!\n");
        return;
    }
    hp->h[hp->tam].vertice = vertice;
    hp->h[hp->tam].dist    = dist;
    subir(hp, hp->tam);
    hp->tam++;
}

HeapNode heapExtrairMin(MinHeap *hp) {
    HeapNode minimo = hp->h[0];
    hp->h[0] = hp->h[--hp->tam];
    descer(hp, 0);
    return minimo;
}

void heapDecrecer(MinHeap *hp, int vertice, double novaDist) {
    for (int  i = 0; i < hp->tam; i++) {
        if (hp->h[i].vertice == vertice) {
            hp->h[i].dist = novaDist;
            subir(hp, i);
            return;
        }
    }
    /* Vértice não está na heap: insere */
    heapInserir(hp, vertice, novaDist);
}
