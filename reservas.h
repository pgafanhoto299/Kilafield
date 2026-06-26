#ifndef RESERVAS_H
#define RESERVAS_H

/* ============================================================
   reservas.h  –  Gestão de reservas de recintos
   ============================================================ */

#include "modelos.h"

/* Verifica conflito horário entre duas reservas do mesmo recinto */
int conflito(const Reserva *a, const Reserva *b);

/* Cria nova reserva; devolve 1 em caso de sucesso */
int criarReserva(Reserva lista[], int *n, int maxN,
                 int idUtil, int idRecinto,
                 const char *data,
                 const char *horaInicio, const char *horaFim);

/* Lista reservas de um utilizador */
void listarReservasUtilizador(const Reserva lista[], int n,
                               int idUtilizador);

/* Lista reservas de um recinto */
void listarReservasRecinto(const Reserva lista[], int n,
                            int idRecinto);

/* Cancela reserva por id; devolve 1 se removeu */
int cancelarReserva(Reserva lista[], int *n, int idReserva);

#endif /* RESERVAS_H */
