#ifndef HAVERSINE_H
#define HAVERSINE_H

/* ============================================================
   haversine.h  –  Cálculo de distância geodésica
   ============================================================
   Parâmetros: lat1, lon1, lat2, lon2 (graus decimais)
   Retorno   : distância em quilómetros
   ============================================================ */

double haversine(double lat1, double lon1, double lat2, double lon2);

#endif /* HAVERSINE_H */
