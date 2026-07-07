/*  Bibliotecas Externas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
/* Bibliotecas do Kila field */
#include "modelos.h"
#include "lista_recintos.h"
#include "trie.h"
#include "grafo.h"
#include "heap.h"
#include "haversine.h"
#include "dijkstra.h"
#include "persistencia.h"
#include "reservas.h"

/* Estado global da aplicação */
Utilizador    utilizadores[MAX_UTILIZADORES];
int           nUtilizadores = 0;
NoRecinto    *listaRecintos = NULL;
TrieNode     *trie          = NULL;
Grafo         grafo;
Reserva       reservas[MAX_RESERVAS];
int           nReservas = 0;

/* ---------- Funcoes auxiliares de input ---------- */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int lerInt(char *msg) {
    int v;
    printf("%s", msg);
    while (scanf("%d", &v) != 1) {
        limparBuffer(); printf("  Valor invalido. %s", msg);
    }
    limparBuffer();
    return v;
}

void lerString(char *msg, char *buf, int max) {
    printf("%s", msg);
    fgets(buf, max, stdin);
    buf[strcspn(buf, "\n")] = '\0';
}

/* ---------- reconstrução da Trie a partir da lista ---------- */
void reconstruirTrie() {
    if (trie) libertarTrie(trie);
    trie = criarTrie();
    for (NoRecinto *p = listaRecintos; p; p = p->prox)
        inserirTrie(trie, p->recinto.nome, p->recinto.id);
}


//Gestão de recintos
void menuRecintos() {
    int op;
    do {
        printf("_________________________________________");
printf("\n|__________Gestao de Recintos___________|\n");
printf("|\t  1. Listar recintos\t\t|\n");
printf("|\t  2. Adicionar recinto\t\t|\n");
printf("|\t  3. Remover recinto\t\t|\n");
printf("|\t  4. Pesquisar por nome\t\t|\n");
printf("|\t     (Trie)\t\t\t|\n");
printf("|\t  5. Filtrar modalidade\t\t|\n");
printf("|\t  6. Alterar disponibilidade \t|\n");
printf("|\t  0. Voltar\t\t\t|\n");
printf("|_______________________________________|\n");

        op = lerInt("Opcao: ");

        if (op == 1) {
            listarRecintos(listaRecintos);

        } else if (op == 2) {
            Recinto r;
            r.id = lerInt("  ID: ");
            lerString("  Nome: ", r.nome, MAX_NOME);
            lerString("  Modalidade: ", r.modalidade, MAX_MODAL);
            r.latitude   = 0; r.longitude = 0;
            printf("  Latitude : "); scanf("%lf", &r.latitude);  limparBuffer();
            printf("  Longitude: "); scanf("%lf", &r.longitude); limparBuffer();
            r.capacidade = lerInt("  Capacidade: ");
            r.disponivel = lerInt("  Disponivel (1=Sim, 0=Nao): ");
            listaRecintos = inserirRecinto(listaRecintos, r);
            reconstruirTrie();
            printf("  Recinto adicionado.\n");

        } else if (op == 3) {
            int id = lerInt("  ID do recinto a remover: ");
            listaRecintos = removerRecinto(listaRecintos, id);
            reconstruirTrie();

        } else if (op == 4) {
            char nome[MAX_NOME];
            lerString("  Nome a pesquisar: ", nome, MAX_NOME);
            int id = -1;
            if (pesquisarTrie(trie, nome, &id)) {
                Recinto *r = encontrarRecinto(listaRecintos, id);
                if (r) printf("  Encontrado: [%d] %s (%s) Cap:%d\n",
                              r->id, r->nome, r->modalidade, r->capacidade);
            } else {
                /* Sugestões por prefixo */
                int ids[20]; int n = 0;
                prefixosTrie(trie, nome, ids, &n, 20);
                if (n > 0) {
                    printf("  Nao encontrado; sugestoes com prefixo '%s':\n", nome);
                    for (int i = 0; i < n; i++) {
                        Recinto *r = encontrarRecinto(listaRecintos, ids[i]);
                        if (r) printf("    [%d] %s\n", r->id, r->nome);
                    }
                } else printf("  Nenhum recinto encontrado.\n");
            }

        } else if (op == 5) {
            char modal[MAX_MODAL];
            lerString("  Modalidade: ", modal, MAX_MODAL);
            listarRecintosPorModalidade(listaRecintos, modal);

        } else if (op == 6) {
            int id  = lerInt("  ID do recinto: ");
            int val = lerInt("  Disponivel (1=Sim, 0=Nao): ");
            Recinto *r = encontrarRecinto(listaRecintos, id);
            if (r) { r->disponivel = val; printf("  Atualizado.\n"); }
            else    printf("  Recinto nao encontrado.\n");
        }
    } while (op != 0);
}

//Gestão de utilizadores
void menuUtilizadores() {
    int op;
    do {
         printf(" ________________________________________");
printf("\n|________Gestao de Utilizadores_________|\n");
printf("|\t  1. Listar utilizadores\t|\n");
printf("|\t  2. Adicionar utilizador\t|\n");
printf("|\t  3. Remover utilizador\t\t|\n");
printf("|\t  0. Voltar\t\t\t|\n");
printf("|_______________________________________|\n");
        op = lerInt("Opcao: ");

        if (op == 1) {
            if (nUtilizadores == 0) { printf("  (nenhum utilizador)\n"); continue; }
            printf("  %-4s %-30s %-10s %-10s\n", "ID","Nome","Lat","Lon");
            for (int i = 0; i < nUtilizadores; i++)
                printf("  %-4d %-30s %-10.4f %-10.4f\n",
                       utilizadores[i].id, utilizadores[i].nome,
                       utilizadores[i].latitude, utilizadores[i].longitude);

        } else if (op == 2) {
            if (nUtilizadores >= MAX_UTILIZADORES) { printf("  Limite atingido.\n"); continue; }
            Utilizador *u = &utilizadores[nUtilizadores];
            u->id = lerInt("  ID: ");
            lerString("  Nome: ", u->nome, MAX_NOME);
            printf("  Latitude : "); scanf("%lf", &u->latitude);  limparBuffer();
            printf("  Longitude: "); scanf("%lf", &u->longitude); limparBuffer();
            nUtilizadores++;
            printf("  Utilizador adicionado.\n");

        } else if (op == 3) {
            int id = lerInt("  ID a remover: ");
            int pos = -1;
            for (int i = 0; i < nUtilizadores; i++)
                if (utilizadores[i].id == id) { pos = i; break; }
            if (pos >= 0) {
                utilizadores[pos] = utilizadores[--nUtilizadores];
                printf("  Removido.\n");
            } else printf("  Nao encontrado.\n");
        }
    } while (op != 0);
}

/* ---------- 3. Reservas ---------- */
void menuReservas() {
    int op;
    do {
         printf(" ________________________________________");
printf("\n|_______________Reservas_______________|\n");
printf("|\t  1. Criar reserva\t\t|\n");
printf("|\t  2. Cancelar reserva\t\t|\n");
printf("|\t  3. Reservas por\t\t|\n");
printf("|\t     utilizador\t\t\t|\n");
printf("|\t  4. Reservas por recinto\t|\n");
printf("|\t  0. Voltar\t\t\t|\n");
printf("|_______________________________________|\n");
       op = lerInt("Opcao: ");

        if (op == 1) {
            int   idU = lerInt("  ID utilizador: ");
            int   idR = lerInt("  ID recinto: ");
            char  data[MAX_DATA], ini[MAX_HORA], fim[MAX_HORA];
            lerString("  Data (AAAA-MM-DD): ", data, MAX_DATA);
            lerString("  Hora inicio (HH:MM): ", ini, MAX_HORA);
            lerString("  Hora fim    (HH:MM): ", fim, MAX_HORA);
            criarReserva(reservas, &nReservas, MAX_RESERVAS,
                         idU, idR, data, ini, fim);

        } else if (op == 2) {
            int id = lerInt("  ID da reserva: ");
            cancelarReserva(reservas, &nReservas, id);

        } else if (op == 3) {
            int id = lerInt("  ID utilizador: ");
            listarReservasUtilizador(reservas, nReservas, id);

        } else if (op == 4) {
            int id = lerInt("  ID recinto: ");
            listarReservasRecinto(reservas, nReservas, id);
        }
    } while (op != 0);
}

/* ---------- 4. Grafo e rotas ---------- */
void menuGrafo() {
    int op;
    do {
        printf(" ________________________________________");
printf("\n|____________Grafo e Rotas______________|\n");
printf("|\t  1. Mostrar grafo\t\t|\n");
printf("|\t  2. Adicionar vertice\t\t|\n");
printf("|\t  3. Adicionar aresta\t\t|\n");
printf("|\t  4. Caminho mais curto\t\t|\n");
printf("|\t     (Dijkstra)\t\t\t|\n");
printf("|\t  5. Recinto mais proximo\t|\n");
printf("|\t     de coordenada\t\t|\n");
printf("|\t  6. Distancia (Haversine)\t|\n");
printf("|\t  0. Voltar\t\t\t|\n");
printf("|_______________________________________|\n");
        op = lerInt("Opcao: ");

        if (op == 1) {
            imprimirGrafo(&grafo);

        } else if (op == 2) {
            int id = lerInt("  ID do vertice: ");
            char nome[MAX_NOME], tipo[20];
            lerString("  Nome: ", nome, MAX_NOME);
            lerString("  Tipo (recinto/intersecao/ponto): ", tipo, 20);
            double lat = 0, lon = 0;
            printf("  Latitude : "); scanf("%lf", &lat); limparBuffer();
            printf("  Longitude: "); scanf("%lf", &lon);  limparBuffer();
            adicionarVertice(&grafo, id, nome, lat, lon, tipo);
            printf("  Vertice adicionado.\n");

        } else if (op == 3) {
            int o = lerInt("  Origem (id): ");
            int d = lerInt("  Destino (id): ");
            int bidi = lerInt("  Bidirecional (1=Sim, 0=Nao): ");
            double peso;
            printf("  Peso (km): "); scanf("%lf", &peso); limparBuffer();
            if (bidi) adicionarArestaBidirecional(&grafo, o, d, peso);
            else       adicionarAresta(&grafo, o, d, peso);
            printf("  Aresta(s) adicionada(s).\n");

        } else if (op == 4) {
            if (grafo.n < 2) { printf("  Grafo insuficiente.\n"); continue; }
            int ori = lerInt("  ID vertice origem: ");
            int dst = lerInt("  ID vertice destino: ");
            int oi  = indiceVertice(&grafo, ori);
            int di  = indiceVertice(&grafo, dst);
            if (oi < 0 || di < 0) { printf("  Vertice(s) nao encontrado(s).\n"); continue; }

            double dist[MAX_V];
            int    prev[MAX_V];
            dijkstra(&grafo, oi, dist, prev);

            if (dist[di] == DBL_MAX) {
                printf("  Sem caminho de %d para %d.\n", ori, dst);
            } else {
                int cam[MAX_V]; int nc;
                nc = reconstruirCaminho(prev, oi, di, cam, MAX_V);
                printf("  Distancia minima: %.3f km\n", dist[di]);
                printf("  Caminho: ");
                for (int i = 0; i < nc; i++)
                    printf("%s%d(%s)", i ? " -> " : "",
                           grafo.v[cam[i]].id, grafo.v[cam[i]].nome);
                printf("\n");
            }

        } else if (op == 5) {
            double lat, lon;
            printf("  Latitude : "); scanf("%lf", &lat); limparBuffer();
            printf("  Longitude: "); scanf("%lf", &lon); limparBuffer();
            double d;
            int idx = recintoMaisProximo(&grafo, lat, lon, &d);
            if (idx < 0) printf("  Nenhum vertice do tipo 'recinto' no grafo.\n");
            else printf("  Recinto mais proximo: [%d] %s  (%.3f km)\n",
                        grafo.v[idx].id, grafo.v[idx].nome, d);

        } else if (op == 6) {
            double la1, lo1, la2, lo2;
            printf("  Ponto A - Lat: "); scanf("%lf",&la1); limparBuffer();
            printf("  Ponto A - Lon: "); scanf("%lf",&lo1); limparBuffer();
            printf("  Ponto B - Lat: "); scanf("%lf",&la2); limparBuffer();
            printf("  Ponto B - Lon: "); scanf("%lf",&lo2); limparBuffer();
            printf("  Distancia Haversine: %.4f km\n",
                   haversine(la1, lo1, la2, lo2));
        }
    } while (op != 0);
}

/* ---------- 5. Persistência ---------- */
void menuPersistencia() {
    int op;
    do {printf(" ________________________________________");
        printf("\n|_____________Persistencia______________|\n");
        printf("|\t  1. Carregar todos os dados\t|\n");
        printf("|\t  2. Guardar todos os dados\t|\n");
        printf("|\t  0. Voltar\t\t\t|\n");
        printf("|_______________________________________|\n");

        op = lerInt("Opcao: ");

        if (op == 1) {
            nUtilizadores = carregarUtilizadores(FICH_UTILIZADORES,
                                                  utilizadores, MAX_UTILIZADORES);
            libertarListaRecintos(listaRecintos);
            listaRecintos = carregarRecintos(FICH_RECINTOS);
            reconstruirTrie();
            nReservas = carregarReservas(FICH_RESERVAS, reservas, MAX_RESERVAS);
            libertarGrafo(&grafo);
            carregarGrafo(FICH_VERTICES, FICH_ARESTAS, &grafo);
            printf("  Dados carregados: %d utilizadores, %d recintos, "
                   "%d reservas, %d vertices.\n",
                   nUtilizadores, contarRecintos(listaRecintos),
                   nReservas, grafo.n);

        } else if (op == 2) {
            guardarUtilizadores(FICH_UTILIZADORES, utilizadores, nUtilizadores);
            guardarRecintos(FICH_RECINTOS, listaRecintos);
            guardarReservas(FICH_RESERVAS, reservas, nReservas);
            guardarGrafo(FICH_VERTICES, FICH_ARESTAS, &grafo);
            printf("  Todos os dados guardados.\n");
        }
    } while (op != 0);
}

/* ============================================================
   DADOS DE EXEMPLO  (popula a aplicação sem ficheiros externos)
   ============================================================ */
void carregarDadosExemplo() {
    /* Utilizadores */
    Utilizador us[] = {
        {1, "Gustavo Guenge",    -8.8147, 13.2302},
        {2, "Orlando Cumandala",  -8.8200, 13.2400},
        {3, "Verilson Caninole", -8.8100, 13.2250},
        {4, "Sívia Antônio",  -8.8300, 13.2500},
        {5, "Ernesto Bartolomeu", -8.8050, 13.2150},
    };
    nUtilizadores = 5;
    for (int i = 0; i < nUtilizadores; i++) utilizadores[i] = us[i];

    /* Recintos */
    Recinto rs[] = {
        {1, "Estadio da Cidadela",  "Futebol",     -8.8100, 13.2200, 50000, 1},
        {2, "Pavilhao Kilamba",     "Basquetebol", -8.8200, 13.2350, 5000,  1},
        {3, "Arena Premier Bet Benfica", "Futebol",     -8.8050, 13.2150, 1000,  1},
        {4, "Live Act Patriota",  "Tenis",       -8.8300, 13.2450, 800,   1},
        {5, "Pavilhão Polidesportivo do ISPTEC",     "Futebol",   -8.8250, 13.2400, 300,   1},
        {6, "Quintalão do Petro C1",   "Futebol",   -8.8150, 13.2280, 3000,  0},
    };
    for (int i = 0; i < 6; i++)
        listaRecintos = inserirRecinto(listaRecintos, rs[i]);
    reconstruirTrie();

    /* Grafo: vértices */
    inicializarGrafo(&grafo);
    adicionarVertice(&grafo, 1, "Estadio da Cidadela",  -8.8100, 13.2200, "recinto");
    adicionarVertice(&grafo, 2, "Pavilhao Kilamba",     -8.8200, 13.2350, "recinto");
    adicionarVertice(&grafo, 3, "Arena Premier Bet Benfica", -8.8050, 13.2150, "recinto");
    adicionarVertice(&grafo, 4, "Live Act Patriota",  -8.8300, 13.2450, "recinto");
    adicionarVertice(&grafo, 5, "Rotunda do camama",  -8.8180, 13.2300, "intersecao");
    adicionarVertice(&grafo, 6, "Intersecao Talatona",          -8.8250, 13.2380, "intersecao");

    /* Arestas bidirecionais com peso Haversine */
    double d;
    d = haversine(-8.8100,13.2200,-8.8180,13.2300); adicionarArestaBidirecional(&grafo,1,5,d);
    d = haversine(-8.8180,13.2300,-8.8200,13.2350); adicionarArestaBidirecional(&grafo,5,2,d);
    d = haversine(-8.8050,13.2150,-8.8180,13.2300); adicionarArestaBidirecional(&grafo,3,5,d);
    d = haversine(-8.8180,13.2300,-8.8250,13.2380); adicionarArestaBidirecional(&grafo,5,6,d);
    d = haversine(-8.8250,13.2380,-8.8300,13.2450); adicionarArestaBidirecional(&grafo,6,4,d);

    /* Reservas */
    criarReserva(reservas,&nReservas,MAX_RESERVAS, 1,1,"2026-07-10","09:00","11:00");
    criarReserva(reservas,&nReservas,MAX_RESERVAS, 2,2,"2026-07-10","10:00","12:00");
    criarReserva(reservas,&nReservas,MAX_RESERVAS, 3,3,"2026-07-11","08:00","09:30");
    criarReserva(reservas,&nReservas,MAX_RESERVAS, 4,4,"2026-07-12","14:00","16:00");
    criarReserva(reservas,&nReservas,MAX_RESERVAS, 5,1,"2026-07-10","14:00","16:00");

    printf("  Dados de exemplo carregados.\n");
}

/* ============================================================
   MAIN
   ============================================================ */
int main() {
    inicializarGrafo(&grafo);
    trie = criarTrie();

    printf("*****************************************\n");
    printf("#");
    printf("       KilaField  version 1.0");
    printf("          #\n");
    printf("*****************************************\n");
    carregarDadosExemplo();

    int op;
    do {
        printf("‖\t  1. Gerir Recintos             ‖\n");
        printf("‖\t  2. Gerir Utilizadores         ‖\n");
        printf("‖\t  3. Gerir Reservas\t\t‖\n");
        printf("‖\t  4. Grafo e Rotas\t\t‖\n");
        printf("‖\t  5. Persistencia (ficheiros)\t‖\n");
        printf("‖\t  0. Sair\t\t\t‖\n");
    printf("=========================================\n");
        op = lerInt("Opcao: ");

        switch (op) {
            case 1: menuRecintos();      break;
            case 2: menuUtilizadores();  break;
            case 3: menuReservas();      break;
            case 4: menuGrafo();         break;
            case 5: menuPersistencia();  break;
            case 0: printf("A sair...\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);

    /* Libertação de memória */
    libertarListaRecintos(listaRecintos);
    libertarTrie(trie);
    libertarGrafo(&grafo);
    return 0;
}
