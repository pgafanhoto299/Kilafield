#define _USE_MATH_DEFINES
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "haversine.h"

/* ============================================================
   haversine.c  –  Fórmula de Haversine
   ============================================================ */

double haversine(double lat1, double lon1, double lat2, double lon2) {
    const double R   = 6371.0;          /* raio médio da Terra em km */
    const double rad = M_PI / 180.0;

    double dLat = (lat2 - lat1) * rad;
    double dLon = (lon2 - lon1) * rad;
    lat1 *= rad;
    lat2 *= rad;

    double a= sin(dLat / 2) * sin(dLat / 2)
             + cos(lat1) * cos(lat2) * sin(dLon / 2) * sin(dLon / 2);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    return R * c;
}
