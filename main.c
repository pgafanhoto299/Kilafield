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
    while (scanf("%d", &v) != 1) { limparBuffer(); printf("  Valor invalido. %s", msg); }
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

//Main
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
