#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "persistencia.h"
#include "haversine.h"

/* ============================================================
   persistencia.c  –  Persistência em ficheiros TXT
   ============================================================
   Formato dos ficheiros: separador ';', primeira linha = cabeçalho
   ============================================================ */

/* ---------- auxiliar: lê linha e descarta cabeçalho ---------- */
int lerCabecalho(FILE *f) {
    char buf[512];
    return fgets(buf, sizeof(buf), f) != NULL;
}

/* ==================== UTILIZADORES ========================== */

int carregarUtilizadores(const char *ficheiro,
                          Utilizador lista[], int max) {
    FILE *f = fopen(ficheiro, "r");
    if (!f) { perror(ficheiro); return 0; }
    lerCabecalho(f);

    int n = 0;
    char buf[512];
    while (n < max && fgets(buf, sizeof(buf), f)) {
        Utilizador *u = &lista[n];
        if (sscanf(buf, "%d;%99[^;];%lf;%lf",
                   &u->id, u->nome, &u->latitude, &u->longitude) == 4)
            n++;
    }
    fclose(f);
    return n;
}

int guardarUtilizadores(const char *ficheiro,
                         const Utilizador lista[], int n) {
    FILE *f = fopen(ficheiro, "w");
    if (!f) { perror(ficheiro); return 0; }
    fprintf(f, "id;nome;latitude;longitude\n");
    for (int i = 0; i < n; i++)
        fprintf(f, "%d;%s;%.6f;%.6f\n",
                lista[i].id, lista[i].nome,
                lista[i].latitude, lista[i].longitude);
    fclose(f);
    return 1;
}

/* ==================== RECINTOS (lista ligada) =============== */

NoRecinto *carregarRecintos(const char *ficheiro) {
    FILE *f = fopen(ficheiro, "r");
    if (!f) { perror(ficheiro); return NULL; }
    lerCabecalho(f);

    NoRecinto *lista = NULL;
    char buf[512];
    while (fgets(buf, sizeof(buf), f)) {
        Recinto tmp;
        if (sscanf(buf, "%d;%99[^;];%49[^;];%lf;%lf;%d;%d",
                   &tmp.id, tmp.nome, tmp.modalidade,
                   &tmp.latitude, &tmp.longitude,
                   &tmp.capacidade, &tmp.disponivel) == 7) {
            NoRecinto *no = malloc(sizeof(NoRecinto));
            if (!no) { perror("malloc"); break; }
            no->recinto = tmp;
            no->prox    = lista;
            lista       = no;
        }
    }
    fclose(f);
    return lista;
}

int guardarRecintos(const char *ficheiro, NoRecinto *lista) {
    FILE *f = fopen(ficheiro, "w");
    if (!f) { perror(ficheiro); return 0; }
    fprintf(f, "id;nome;modalidade;latitude;longitude;capacidade;disponivel\n");
    for (NoRecinto *p = lista; p; p = p->prox)
        fprintf(f, "%d;%s;%s;%.6f;%.6f;%d;%d\n",
                p->recinto.id, p->recinto.nome, p->recinto.modalidade,
                p->recinto.latitude, p->recinto.longitude,
                p->recinto.capacidade, p->recinto.disponivel);
    fclose(f);
    return 1;
}

/* ==================== RESERVAS ============================== */

int carregarReservas(const char *ficheiro,
                      Reserva lista[], int max) {
    FILE *f = fopen(ficheiro, "r");
    if (!f) { perror(ficheiro); return 0; }
    lerCabecalho(f);

    int n = 0;
    char buf[512];
    while (n < max && fgets(buf, sizeof(buf), f)) {
        Reserva *r = &lista[n];
        if (sscanf(buf, "%d;%d;%d;%19[^;];%9[^;];%9[^\n]",
                   &r->idReserva, &r->idUtilizador, &r->idRecinto,
                   r->data, r->horaInicio, r->horaFim) == 6)
            n++;
    }
    fclose(f);
    return n;
}

int guardarReservas(const char *ficheiro,
                     const Reserva lista[], int n) {
    FILE *f = fopen(ficheiro, "w");
    if (!f) { perror(ficheiro); return 0; }
    fprintf(f, "idReserva;idUtilizador;idRecinto;data;horaInicio;horaFim\n");
    for (int i = 0; i < n; i++)
        fprintf(f, "%d;%d;%d;%s;%s;%s\n",
                lista[i].idReserva, lista[i].idUtilizador,
                lista[i].idRecinto, lista[i].data,
                lista[i].horaInicio, lista[i].horaFim);
    fclose(f);
    return 1;
}

/* ==================== GRAFO ================================= */

int carregarGrafo(const char *fichVert, const char *fichArestas,
                   Grafo *g) {
    inicializarGrafo(g);

    /* --- Vértices --- */
    FILE *fv = fopen(fichVert, "r");
    if (!fv) { perror(fichVert); return 0; }
    lerCabecalho(fv);

    char buf[512];
    while (fgets(buf, sizeof(buf), fv)) {
        int    id;
        char   nome[MAX_NOME], tipo[20];
        double lat, lon;
        if (sscanf(buf, "%d;%99[^;];%lf;%lf;%19[^\n]",
                   &id, nome, &lat, &lon, tipo) == 5)
            adicionarVertice(g, id, nome, lat, lon, tipo);
    }
    fclose(fv);

    /* --- Arestas --- */
    FILE *fa = fopen(fichArestas, "r");
    if (!fa) { perror(fichArestas); return 0; }
    lerCabecalho(fa);

    while (fgets(buf, sizeof(buf), fa)) {
        int    o, d;
        double peso;
        if (sscanf(buf, "%d;%d;%lf", &o, &d, &peso) == 3)
            adicionarAresta(g, o, d, peso);
    }
    fclose(fa);
    return 1;
}

int guardarGrafo(const char *fichVert, const char *fichArestas,
                  const Grafo *g) {
    /* Vértices */
    FILE *fv = fopen(fichVert, "w");
    if (!fv) { perror(fichVert); return 0; }
    fprintf(fv, "id;nome;latitude;longitude;tipo\n");
    for (int i = 0; i < g->n; i++)
        fprintf(fv, "%d;%s;%.6f;%.6f;%s\n",
                g->v[i].id, g->v[i].nome,
                g->v[i].latitude, g->v[i].longitude,
                g->v[i].tipo);
    fclose(fv);

    /* Arestas */
    FILE *fa = fopen(fichArestas, "w");
    if (!fa) { perror(fichArestas); return 0; }
    fprintf(fa, "origem;destino;peso\n");
    for (int i = 0; i < g->n; i++)
        for (Aresta *a = g->v[i].adj; a; a = a->prox)
            fprintf(fa, "%d;%d;%.6f\n",
                    g->v[i].id, g->v[a->destino].id, a->peso);
    fclose(fa);
    return 1;
}
