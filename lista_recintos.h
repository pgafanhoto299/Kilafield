#ifndef LISTA_RECINTOS_H
#define LISTA_RECINTOS_H

/* ============================================================
   lista_recintos.h  –  Operações sobre a lista ligada de recintos
   ============================================================ */

#include "modelos.h"

NoRecinto *inserirRecinto(NoRecinto *lista, Recinto r);
NoRecinto *removerRecinto(NoRecinto *lista, int id);
Recinto   *encontrarRecinto(NoRecinto *lista, int id);
int        contarRecintos(const NoRecinto *lista);
void       listarRecintos(const NoRecinto *lista);
void       listarRecintosPorModalidade(const NoRecinto *lista,
                                       const char *modalidade);
void       libertarListaRecintos(NoRecinto *lista);

#endif /* LISTA_RECINTOS_H */
