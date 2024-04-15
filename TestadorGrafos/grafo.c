#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep(x * 1000)
#define system("cls") system("clear")
#endif

typedef struct No
{
    int vertice;
    struct No *prox;
} No;

typedef struct Head
{
    int tamanho;
    No *inicio;
} Head;

typedef struct Grafo
{
    int numVertices;
    Head *vetor;
} Grafo;

typedef struct Fila
{
    No *inicio;
    No *fim;
} Fila;

Fila *criaFila()
{
    Fila *f = (Fila *)malloc(sizeof(Fila));
    f->inicio = NULL;
    f->fim = NULL;
    return f;
}

void insereFila(Fila *f, int vertice)
{
    No *novo = (No *)malloc(sizeof(No));
    novo->vertice = vertice;
    novo->prox = NULL;

    if (f->inicio == NULL)
    {
        f->inicio = novo;
        f->fim = novo;
    }
    else
    {
        f->fim->prox = novo;
        f->fim = novo;
    }
}

int removeFila(Fila *f)
{
    No *aux = f->inicio;
    int vertice = aux->vertice;
    f->inicio = f->inicio->prox;
    free(aux);
    return vertice;
}

int filaVazia(Fila *f)
{
    return f->inicio == NULL;
}

Grafo *criaGrafo()
{
    Grafo *g = (Grafo *)malloc(sizeof(Grafo));
    g->numVertices = 0;
    g->vetor = NULL;
    return g;
}

void criaVertice(Grafo *g)
{
    g->vetor = (Head *)realloc(g->vetor, (g->numVertices + 1) * sizeof(Head));
    g->vetor[g->numVertices].tamanho = 0;
    g->vetor[g->numVertices].inicio = NULL;
    g->numVertices++;
}

void criaAresta(Grafo *g, int origem, int destino)
{
    No *novo = malloc(sizeof(No));
    novo->vertice = destino;
    novo->prox = NULL;

    if (g->vetor[origem].inicio == NULL)
    {
        g->vetor[origem].inicio = novo;
    }
    else
    {
        No *aux = g->vetor[origem].inicio;
        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}

void criaArestaSemDirecao(Grafo *g, int origem, int destino)
{
    criaAresta(g, origem, destino);
    criaAresta(g, destino, origem);
}

Grafo *bfs(Grafo *g)
{
    Grafo *g2 = criaGrafo();
    Fila *f = criaFila();
    int *visitado = (int *)calloc(g->numVertices, sizeof(int));

    for (int i = 0; i < g->numVertices; i++)
    {
        criaVertice(g2);
    }

    for (int i = 0; i < g->numVertices; i++)
    {
        if (!visitado[i])
        {
            insereFila(f, i);
            visitado[i] = 1;

            while (!filaVazia(f))
            {
                int vertice = removeFila(f);
                No *aux = g->vetor[vertice].inicio;
                while (aux != NULL)
                {
                    if (!visitado[aux->vertice])
                    {
                        insereFila(f, aux->vertice);
                        visitado[aux->vertice] = 1;
                        criaAresta(g2, vertice, aux->vertice);
                    }
                    aux = aux->prox;
                }
            }
        }
    }

    free(visitado);
    return g2;
}

Grafo *bfs_SemDirecao(Grafo *g)
{
    Grafo *g2 = criaGrafo();
    Fila *f = criaFila();
    int *visitado = (int *)calloc(g->numVertices, sizeof(int));

    for (int i = 0; i < g->numVertices; i++)
    {
        criaVertice(g2);
    }

    for (int i = 0; i < g->numVertices; i++)
    {
        if (!visitado[i])
        {
            insereFila(f, i);
            visitado[i] = 1;

            while (!filaVazia(f))
            {
                int vertice = removeFila(f);
                No *aux = g->vetor[vertice].inicio;
                while (aux != NULL)
                {
                    if (!visitado[aux->vertice])
                    {
                        insereFila(f, aux->vertice);
                        visitado[aux->vertice] = 1;
                        criaArestaSemDirecao(g2, vertice, aux->vertice);
                    }
                    aux = aux->prox;
                }
            }
        }
    }

    free(visitado);
    return g2;
}

int bfs_bipartido(Grafo *g)
{
    int *cor = (int *)calloc(g->numVertices, sizeof(int));
    Fila *f = criaFila();
    int *visitado = (int *)calloc(g->numVertices, sizeof(int));
    int bipartido = 1;

    for (int i = 0; i < g->numVertices; i++)
    {
        if (!visitado[i])
        {
            insereFila(f, i);
            visitado[i] = 1;
            cor[i] = 1;

            while (!filaVazia(f))
            {
                int vertice = removeFila(f);
                No *aux = g->vetor[vertice].inicio;
                while (aux != NULL)
                {
                    if (!visitado[aux->vertice])
                    {
                        insereFila(f, aux->vertice);
                        visitado[aux->vertice] = 1;
                        cor[aux->vertice] = !cor[vertice];
                    }
                    else if (cor[aux->vertice] == cor[vertice])
                    {
                        bipartido = 0;
                    }
                    aux = aux->prox;
                }
            }
        }
    }

    free(cor);
    free(visitado);

    return bipartido;
}

void dfs_ciclo_maior(Grafo *g, int v, bool visitado[], int caminho[], int *indiceCaminho, int tamanhoCaminho, int *TamanhoMaiorCiclo, int *maiorCiclo)
{
    visitado[v] = true;
    caminho[(*indiceCaminho)++] = v;

    No *aux = g->vetor[v].inicio;
    while (aux != NULL)
    {
        int adj = aux->vertice;
        if (!visitado[adj])
        {
            dfs_ciclo_maior(g, adj, visitado, caminho, indiceCaminho, tamanhoCaminho, TamanhoMaiorCiclo, maiorCiclo);
        }
        else
        {
            for (int i = 0; i < *indiceCaminho - 1; i++)
            {
                if (caminho[i] == adj)
                {
                    int tamanhoCiclo = *indiceCaminho - i;
                    if (tamanhoCiclo > *TamanhoMaiorCiclo)
                    {
                        *TamanhoMaiorCiclo = tamanhoCiclo;
                        for (int j = 0; j < tamanhoCiclo; j++)
                        {
                            maiorCiclo[j] = caminho[i + j];
                        }
                    }
                }
            }
        }
        aux = aux->prox;
    }
    (*indiceCaminho)--;
    visitado[v] = false;
}

//  https://leetcode.com/problems/longest-cycle-in-a-graph/description/
int leetcode_2360_longest_cycle_in_a_graph(Grafo *g)
{
    int *maiorciclo = (int *)malloc(g->numVertices * sizeof(int));
    bool *visitado = (bool *)calloc(g->numVertices, sizeof(int));
    int *caminho = (int *)malloc(g->numVertices * sizeof(int));
    int TamanhoMaiorCiclo = 0;
    int indiceCaminho = 0;
    for (int i = 0; i < g->numVertices; i++)
    {
        if (!visitado[i])
        {
            dfs_ciclo_maior(g, i, visitado, caminho, &indiceCaminho, 0, &TamanhoMaiorCiclo, maiorciclo);
        }
    }
    free(visitado);
    free(caminho);
    if (TamanhoMaiorCiclo == 0)
    {
        return -1;
    }
    return TamanhoMaiorCiclo;
}

//  https://leetcode.com/problems/is-graph-bipartite/description
bool leetcode_785_isBipartite(Grafo *g)
{
    if (bfs_bipartido(g))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void printaGrafo(Grafo *g)
{
    printf("\n");
    if (g->numVertices == 0)
    {
        printf("Grafo vazio\n");
        return;
    }
    for (int i = 0; i < g->numVertices; i++)
    {
        No *aux = g->vetor[i].inicio;
        printf("%d -->", i);
        while (aux != NULL)
        {
            printf(" %d ->", aux->vertice);
            aux = aux->prox;
        }
        printf(" //");
        printf("\n");
    }
}

void printWelcome()
{
    printf("----------------------------------------");
    Sleep(250);
    printf("-----------------------------------------\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\tBem vindo ao programa de grafos\t\t\t\t|\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("----------------------------------------");
    Sleep(250);
    printf("-----------------------------------------\n");
    Sleep(1000);
}

void printGoodBye()
{
    printf("----------------------------------------");
    Sleep(250);
    printf("-----------------------------------------\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t   Obrigado por utilizar o programa de grafos\t\t\t|\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("----------------------------------------");
    Sleep(250);
    printf("-----------------------------------------\n");
    Sleep(1000);
}

void printMenu()
{
    printf("----------------------------------------");
    Sleep(250);
    printf("-----------------------------------------\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    Sleep(250);
    printf("|\t\t\t\tMenu principal\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t(1) Mostrar grafo\t\t\t\t\t\t|\n");
    wprintf(L"|\t\t(2) Adicionar vértice\t\t\t\t\t\t|\n");
    printf("|\t\t(3) Adicionar aresta bidirecional\t\t\t\t|\n");
    printf("|\t\t(4) Adicionar aresta direcionada\t\t\t\t|\n");
    printf("|\t\t(5) BFS simples\t\t\t\t\t\t\t|\n");
    wprintf(L"|\t\t(6) Verificar se o grafo é bipartido\t\t\t\t|\n");
    wprintf(L"|\t\t(7) Verificar o maior ciclo do grafo\t\t\t\t|\n");
    printf("|\t\t(0) Sair\t\t\t\t\t\t\t|\n");
    Sleep(250);
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("----------------------");
    Sleep(150);
    printf("----------------------");
    Sleep(250);
    printf("-------------------------------------\n");
}

void printMenuRapido()
{
    printf("----------------------------------------");
    printf("-----------------------------------------\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\tMenu principal\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t(1) Mostrar grafo\t\t\t\t\t\t|\n");
    wprintf(L"|\t\t(2) Adicionar vértice\t\t\t\t\t\t|\n");
    printf("|\t\t(3) Adicionar aresta bidirecional\t\t\t\t|\n");
    printf("|\t\t(4) Adicionar aresta direcionada\t\t\t\t|\n");
    printf("|\t\t(5) BFS simples\t\t\t\t\t\t\t|\n");
    wprintf(L"|\t\t(6) Verificar se o grafo é bipartido\t\t\t\t|\n");
    wprintf(L"|\t\t(7) Verificar o maior ciclo do grafo\t\t\t\t|\n");
    printf("|\t\t(0) Sair\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t|\n");
    printf("----------------------");
    printf("----------------------");
    printf("-------------------------------------\n");
}

int main()
{
    setlocale(LC_ALL, "Portuguese");
    Grafo *g = criaGrafo();
    printWelcome();
    printMenu();
    int rodando = 1;
    while (rodando == 1)
    {
        int option;
        system("cls");
        printMenuRapido();
        scanf("%d", &option);
        int origem;
        int destino;

        switch (option)
        {
        case 1:
            printaGrafo(g);
            Sleep(5000);
            break;

        case 2:
            printf("Vertice %d criado\n", g->numVertices);
            criaVertice(g);
            Sleep(750);
            break;

        case 3:
            printf("Digite a aresta de origem: \n");
            scanf("%d", &origem);
            printf("Digite a aresta de chegada: \n");
            scanf("%d", &destino);
            criaArestaSemDirecao(g, origem, destino);
            printf("Aresta %d ---- %d criada", origem, destino);
            Sleep(1000);
            break;

        case 4:
            printf("Digite a aresta de origem: \n");
            scanf("%d", &origem);
            printf("Digite a aresta de chegada: \n");
            scanf("%d", &destino);
            criaAresta(g, origem, destino);
            printf("Aresta %d ----> %d criada", origem, destino);
            Sleep(1000);
            break;

        case 5:;
            Grafo *gTrees = bfs(g);
            printaGrafo(gTrees);
            Sleep(7000);
            break;

        case 6:
            leetcode_785_isBipartite(g) ? wprintf(L"O grafo é bipartido\n") : wprintf(L"O grafo não é bipartido\n");
            Sleep(7000);
            break;

        case 7:;
            int maiorCiclo = leetcode_2360_longest_cycle_in_a_graph(g);
            if (maiorCiclo == -1)
                wprintf(L"O grafo não possui ciclos\n");
            else
                printf("O maior ciclo do grafo tem tamanho %d\n", maiorCiclo);
            Sleep(7000);
            break;

        default:
            rodando = 0;
            break;
        }
    }
    free(g);
    system("cls");
    printGoodBye();
}