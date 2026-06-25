#ifndef TRIE_H
#define TRIE_H

/* ============================================================
   trie.h  –  Árvore Trie para pesquisa de nomes de recintos
   ============================================================ */

#define ALPHA 26

typedef struct TrieNode {
    struct TrieNode *filhos[ALPHA];
    int fim;          /* 1 se o nó termina uma palavra */
    int idRecinto;    /* id do recinto (válido quando fim == 1) */
} TrieNode;

TrieNode *criarTrie(void);
void      inserirTrie(TrieNode *raiz, const char *s, int idRecinto);
int       pesquisarTrie(TrieNode *raiz, const char *s, int *idOut);
void      prefixosTrie(TrieNode *raiz, const char *prefixo,
                       int ids[], int *n, int maxN);
void      libertarTrie(TrieNode *raiz);

#endif /* TRIE_H */
