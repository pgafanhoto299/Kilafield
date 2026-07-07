/*Bibliotecas externas*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
/*Bibliotecas do kila field*/
#include "modelos.h"
#include "lista_recintos.h"
#include "trie.h"
#include "grafo.h"
#include "heap.h"
#include "haversine.h"
#include "dijkstra.h"
#include "persistencia.h"
#include "reservas.h"

//Estado global das aplicacao
Utilizador    utilizadores[MAX_UTILIZADORES];
int           nUtilizadores = 0;
NoRecinto    *listaRecintos = NULL;
TrieNode     *trie          = NULL;
Grafo         grafo;
Reserva       reservas[MAX_RESERVAS];
int           nReservas = 0;

//Funcoes auxiliares de inputs
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
    } while (op != 0);
}

/* ---------- 2. Gestao de utilizadores ---------- */
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
    } while (op != 0);
}

//Persistência
void menuPersistencia() {
    int op;
    do {
        printf(" ________________________________________");
        printf("\n|_____________Persistencia______________|\n");
        printf("|\t  1. Carregar todos os dados\t|\n");
        printf("|\t  2. Guardar todos os dados\t|\n");
        printf("|\t  0. Voltar\t\t\t|\n");
        printf("|_______________________________________|\n");
        op = lerInt("Opcao: ");

    } while (op != 0);
}

// DADOS DE EXEMPLO  (popula a aplicações sem ficheiros externos)
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


    printf("*****************************************\n");
    printf("#");
    printf("       KilaField  version 1.0");
    printf("          #\n");
    printf("*****************************************\n");
    //carregarDadosExemplo();

    int op;
    do {
        printf("=========================================\n");
    printf("‖");
    printf("       MENU PRINCIPAL");
    printf("                  ‖\n");
    printf("=========================================\n");

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

    return 0;
}
