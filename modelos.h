#ifndef MODELOS_H
#define MODELOS_H

/* ============================================================
   modelos.h  –  Estruturas de dados do sistema
   ============================================================ */

#define MAX_NOME      100
#define MAX_MODAL      50
#define MAX_DATA       20
#define MAX_HORA       10
#define MAX_RECINTOS  200
#define MAX_VERTICES  200
#define MAX_RESERVAS  500
#define MAX_UTILIZADORES 200

/* ---------- Utilizador ---------- */
typedef struct {
    int    id;
    char   nome[MAX_NOME];
    double latitude;
    double longitude;
} Utilizador;

/* ---------- Recinto desportivo ---------- */
typedef struct {
    int    id;
    char   nome[MAX_NOME];
    char   modalidade[MAX_MODAL];
    double latitude;
    double longitude;
    int    capacidade;
    int    disponivel;   /* 1 = disponível, 0 = encerrado */
} Recinto;

/* ---------- Lista ligada de recintos ---------- */
typedef struct NoRecinto {
    Recinto          recinto;
    struct NoRecinto *prox;
} NoRecinto;

/* ---------- Reserva ---------- */
typedef struct {
    int  idReserva;
    int  idUtilizador;
    int  idRecinto;
    char data[MAX_DATA];
    char horaInicio[MAX_HORA];
    char horaFim[MAX_HORA];
} Reserva;

#endif /* MODELOS_H */
