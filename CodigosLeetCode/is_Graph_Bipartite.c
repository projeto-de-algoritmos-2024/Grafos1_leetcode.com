typedef struct No{
    int vertice;
    struct No *prox;
}No;

typedef struct Fila{
    No *inicio;
    No *fim;
}Fila;

Fila *criaFila(){
    Fila *f = (Fila*)malloc(sizeof(Fila));
    f->inicio = NULL;
    f->fim = NULL;
    return f;
}

void insereFila(Fila *f, int vertice){
    No *novo = (No*)malloc(sizeof(No));
    novo->vertice = vertice;
    novo->prox = NULL;

    if(f->inicio == NULL){
        f->inicio = novo;
        f->fim = novo;
    }else{
        f->fim->prox = novo;
        f->fim = novo;
    }
}

int removeFila(Fila *f){
    No *aux = f->inicio;
    int vertice = aux->vertice;
    f->inicio = f->inicio->prox;
    free(aux);
    return vertice;
}

int filaVazia(Fila *f){
    return f->inicio == NULL;
}


bool isBipartite(int** graph, int graphSize, int* graphColSize) {
    int *cor = (int*)calloc(graphSize, sizeof(int));
    Fila *f = criaFila();
    int *visitado = (int*)calloc(graphSize, sizeof(int));
    int bipartido = 1;

    for(int i = 0; i < graphSize; i++){
        if(!visitado[i]){
            insereFila(f, i);
            visitado[i] = 1;
            cor[i] = 1;
        }

        while(!filaVazia(f)){
            int vertice = removeFila(f);
            int *adj = graph[vertice];
            for(int j = 0; j < graphColSize[vertice]; j++){
                if(!visitado[adj[j]]){
                    insereFila(f, adj[j]);
                    visitado[adj[j]] = 1;
                    cor[adj[j]] = !cor[vertice];
                }
                else if(cor[adj[j]] == cor[vertice]){
                    bipartido = 0;
                }
            }
        }
    }
    if(bipartido)
        return true;
    return false;
}