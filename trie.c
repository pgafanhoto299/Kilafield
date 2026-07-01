#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"

/* ============================================================
   trie.c  –  Implementação da Trie
   ============================================================ */

/* Normaliza carácter para índice 0-25 (ignora não-alfabéticos) */
int indice(char c) {
   c = (char)tolower((unsigned char)c);
    if (c >= 'a' && c <= 'z') return c - 'a';
    return -1;
}

TrieNode *criarTrie(void) {
    TrieNode *no = calloc(1, sizeof(TrieNode));
    if (!no) { perror("criarTrie: calloc"); exit(EXIT_FAILURE); }
    no->idRecinto = -1;
    return no;
}

void inserirTrie(TrieNode *raiz, const char *s, int idRecinto) {
    TrieNode *cur = raiz;
    for (; *s; s++) {
        int i = indice(*s);
        if (i < 0) continue;                    /* ignora espaços, etc. */
        if (!cur->filhos[i]) cur->filhos[i] = criarTrie();
        cur = cur->filhos[i];
    }
    cur->fim = 1;
    cur->idRecinto = idRecinto;
}

int pesquisarTrie(TrieNode *raiz, const char *s, int *idOut) {
    TrieNode *cur = raiz;
    for (; *s; s++) {
        int i = indice(*s);
        if (i < 0) continue;
        if (!cur->filhos[i]) return 0;
        cur = cur->filhos[i];
    }
    if (cur->fim) {
        if (idOut) *idOut = cur->idRecinto;
        return 1;
    }
    return 0;
}

/* Recolha recursiva de todos os ids a partir de 'cur' */
void recolherIds(TrieNode *cur, int ids[], int *n, int maxN) {
    if (!cur || *n >= maxN) return;
    if (cur->fim) ids[(*n)++] = cur->idRecinto;
    for (int i = 0; i < ALPHA; i++)
        recolherIds(cur->filhos[i], ids, n, maxN);
}

void prefixosTrie(TrieNode *raiz, const char *prefixo,
                  int ids[], int *n, int maxN) {
    *n = 0;
    TrieNode *cur = raiz;
    for (const char *p = prefixo; *p; p++) {
        int i = indice(*p);
        if (i < 0) continue;
        if (!cur->filhos[i]) return;   /* prefixo inexistente */
        cur = cur->filhos[i];
    }
    recolherIds(cur, ids, n, maxN);
}

void libertarTrie(TrieNode *raiz) {
    if (!raiz) return;
    for (int i = 0; i < ALPHA; i++) libertarTrie(raiz->filhos[i]);
    free(raiz);
}
