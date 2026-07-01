#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_recintos.h"

/* ============================================================
   lista_recintos.c  –  Lista ligada de recintos desportivos
   ============================================================ */

NoRecinto *inserirRecinto(NoRecinto *lista, Recinto r) {
    NoRecinto *no = malloc(sizeof(NoRecinto));
    if (!no) { perror("inserirRecinto"); return lista; }
    no->recinto = r;
    no->prox    = lista;
    return no;
}

NoRecinto *removerRecinto(NoRecinto *lista, int id) {
    NoRecinto *prev = NULL, *cur = lista;
    while (cur) {
        if (cur->recinto.id == id) {
            if (prev) prev->prox = cur->prox;
            else       lista     = cur->prox;
            free(cur);
            return lista;
        }
        prev = cur;
        cur  = cur->prox;
    }
    printf("Recinto com id %d nao encontrado.\n", id);
    return lista;
}

Recinto *encontrarRecinto(NoRecinto *lista, int id) {
    for (NoRecinto *p = lista; p; p = p->prox)
        if (p->recinto.id == id) return &p->recinto;
    return NULL;
}

int contarRecintos(const NoRecinto *lista) {
    int n = 0;
    for (const NoRecinto *p = lista; p; p = p->prox) n++;
    return n;
}

void listarRecintos(const NoRecinto *lista) {
    if (!lista) { printf("  (nenhum recinto registado)\n"); return; }
    printf("  %-4s %-35s %-20s %-8s %-8s %-10s %s\n",
           "ID\t", "Nome\t\t", "Modalidade\t", "Latitudet\t", "Longitude\t", "Capacidade\t", "Disponibilidade");
    printf("  %s\n", "----------------------------------------------------------------------------------------------------------------------------------------------");
    for (const NoRecinto *p = lista; p; p = p->prox) {
        const Recinto *r = &p->recinto;
        printf("  %-4d\t %-35s\t\t %-20s\t %-8.4f\t %-8.4f\t %-10d\t %s\n",
               r->id, r->nome, r->modalidade,
               r->latitude, r->longitude,
               r->capacidade,
               r->disponivel ? "Sim" : "Nao");
    }
    printf("  %s\n", "----------------------------------------------------------------------------------------------------------------------------------------------");
}

void listarRecintosPorModalidade(const NoRecinto *lista,
                                   const char *modalidade) {
    int achou = 0;
    for (const NoRecinto *p = lista; p; p = p->prox) {
        if (strcmp(p->recinto.modalidade, modalidade) == 0) {
            if (!achou) {
                printf("  Recintos de '%s':\n", modalidade);
                achou = 1;
            }
            const Recinto *r = &p->recinto;
            printf("    [%d] %s  Cap:%d  %s\n",
                   r->id, r->nome, r->capacidade,
                   r->disponivel ? "(disponivel)" : "(encerrado)");
        }
    }
    if (!achou)  printf("  Nenhum recinto da modalidade '%s'.\n", modalidade);
}

void libertarListaRecintos(NoRecinto *lista) {
    while (lista) {
        NoRecinto *prox = lista->prox;
        free(lista);
        lista = prox;
    }
}
