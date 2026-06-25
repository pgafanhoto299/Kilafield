#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

/* ============================================================
   persistencia.h  –  Leitura e escrita de ficheiros TXT
   ============================================================ */

#include "modelos.h"
#include "grafo.h"

/* Caminhos padrão dos ficheiros de dados */
#define FICH_UTILIZADORES "utilizadores.txt"
#define FICH_RECINTOS     "recintos.txt"
#define FICH_RESERVAS     "reservas.txt"
#define FICH_VERTICES     "vertices.txt"
#define FICH_ARESTAS      "arestas.txt"

/* ---- Utilizadores ---- */
int carregarUtilizadores(const char *ficheiro,
                         Utilizador lista[], int max);
int guardarUtilizadores(const char *ficheiro,
                        const Utilizador lista[], int n);

/* ---- Recintos (lista ligada) ---- */
NoRecinto *carregarRecintos(const char *ficheiro);
int        guardarRecintos(const char *ficheiro, NoRecinto *lista);

/* ---- Reservas ---- */
int carregarReservas(const char *ficheiro,
                     Reserva lista[], int max);
int guardarReservas(const char *ficheiro,
                    const Reserva lista[], int n);

/* ---- Grafo (vértices + arestas) ---- */
int carregarGrafo(const char *fichVert, const char *fichArestas,
                  Grafo *g);
int guardarGrafo(const char *fichVert, const char *fichArestas,
                 const Grafo *g);

#endif /* PERSISTENCIA_H */
