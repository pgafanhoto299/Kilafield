#include <stdio.h>
#include <string.h>
#include "reservas.h"

/* ============================================================
   reservas.c  –  Implementação da gestão de reservas
   ============================================================ */

/* Converte "HH:MM" em minutos desde meia-noite */
int horaParaMin(const char *h) {
    int hh = 0, mm = 0;
    sscanf(h, "%d:%d", &hh, &mm);
     return hh * 60 + mm;
}

int conflito(const Reserva *a, const Reserva *b) {
    if (a->idRecinto != b->idRecinto) return 0;
    if (strcmp(a->data, b->data)     != 0) return 0;
    int ini_a = horaParaMin(a->horaInicio), fim_a = horaParaMin(a->horaFim);
    int ini_b = horaParaMin(b->horaInicio), fim_b = horaParaMin(b->horaFim);
    return !(fim_a <= ini_b || fim_b <= ini_a);
}

int criarReserva(Reserva lista[], int *n, int maxN,
                 int idUtil, int idRecinto,
                 const char *data,
                 const char *horaInicio, const char *horaFim) {
    if (*n >= maxN) { printf("  Limite de reservas atingido.\n"); return 0; }

    /* Verifica conflito com reservas existentes */
    Reserva nova;
    nova.idUtilizador = idUtil;
    nova.idRecinto    = idRecinto;
    strncpy(nova.data,       data,       MAX_DATA - 1);
    strncpy(nova.horaInicio, horaInicio, MAX_HORA - 1);
    strncpy(nova.horaFim,    horaFim,    MAX_HORA - 1);

    for (int i = 0; i < *n; i++) {
        if (conflito(&lista[i], &nova)) {
            printf("  ERRO: conflito horario com reserva #%d.\n",
                   lista[i].idReserva);
            return 0;
        }
    }

    /* Determina próximo id */
    int maxId = 0;
    for (int i = 0; i < *n; i++)
        if (lista[i].idReserva > maxId) maxId = lista[i].idReserva;
    nova.idReserva = maxId + 1;

    lista[(*n)++] = nova;
    printf("*  Reserva #%d criada com sucesso.\t*\n", nova.idReserva);
    return 1;
}

void listarReservasUtilizador(const Reserva lista[], int n,
                               int idUtilizador) {
    int achou = 0;
    for (int i = 0; i < n; i++) {
        if (lista[i].idUtilizador == idUtilizador) {
            if (!achou) printf("  Reservas do utilizador %d:\n", idUtilizador);
            achou = 1;
            printf("    #%-4d Recinto:%-4d  %s  %s-%s\n",
                   lista[i].idReserva, lista[i].idRecinto,
                   lista[i].data, lista[i].horaInicio, lista[i].horaFim);
        }
    }
    if (!achou) printf("  Sem reservas para o utilizador %d.\n", idUtilizador);
}

void listarReservasRecinto(const Reserva lista[], int n,
                            int idRecinto) {
    int achou = 0;
    for (int i = 0; i < n; i++) {
        if (lista[i].idRecinto == idRecinto) {
            if (!achou) printf("  Reservas do recinto %d:\n", idRecinto);
            achou = 1;
            printf("    #%-4d Utilizador:%-4d  %s  %s-%s\n",
                   lista[i].idReserva, lista[i].idUtilizador,
                   lista[i].data, lista[i].horaInicio, lista[i].horaFim);
        }
    }
    if (!achou) printf("  Sem reservas para o recinto %d.\n", idRecinto);
}

int cancelarReserva(Reserva lista[], int *n, int idReserva) {
    for (int i = 0; i < *n; i++) {
        if (lista[i].idReserva == idReserva) {
            lista[i] = lista[--(*n)];
            printf("  Reserva #%d cancelada.\n", idReserva);
            return 1;
        }
    }
    printf("  Reserva #%d nao encontrada.\n", idReserva);
    return 0;
}
