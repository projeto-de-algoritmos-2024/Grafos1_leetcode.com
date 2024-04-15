unsigned short int inicio = 0;
unsigned short int fim = 0;

void enfileira(int *fila, int x)
{
    fila[fim++] = x;
}

int desenfileira(int *fila)
{
    return fila[inicio++];
}

int findCircleNum(int **isConnected, int isConnectedSize, int *isConnectedColSize)
{
    int contador = 0;
    unsigned short int visitados[isConnectedSize];
    int *fila = (int *)malloc(isConnectedSize * sizeof(int));
    for (int i = 0; i < isConnectedSize; i++)
    {
        visitados[i] = 0;
    }

    for (unsigned short int i = 0; i < isConnectedSize; i++)
    {
        if (!visitados[i])
        {
            contador++;
            enfileira(fila, i);
            visitados[i] = 1;

            while (fim > inicio)
            {
                int no = desenfileira(fila);
                for (int j = 0; j < isConnectedColSize[no]; j++)
                {
                    if (isConnected[no][j] == 1 && !visitados[j])
                    {
                        visitados[j] = 1;
                        enfileira(fila, j);
                    }
                }
            }
            inicio = 0;
            fim = 0;
        }
    }
    return contador;
}