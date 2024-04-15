void dfs_ciclo_maior(int *edges, int v, bool visited[], int pathIndex[], int *currentPathIndex, int *maxCycleLength, int *startPosition) {
    visited[v] = true;
    pathIndex[v] = *currentPathIndex;
    (*currentPathIndex)++;

    int adj = edges[v];
    if (adj != -1) {
        if (!visited[adj]) {
            dfs_ciclo_maior(edges, adj, visited, pathIndex, currentPathIndex, maxCycleLength, startPosition);
        } else if (pathIndex[adj] != -1) {
            int cycleLength = *currentPathIndex - pathIndex[adj];
            if (cycleLength > *maxCycleLength) {
                *maxCycleLength = cycleLength;
            }
        }
    }

    pathIndex[v] = -1;
}

int longestCycle(int *edges, int edgesSize) {
    bool *visited = (bool *)calloc(edgesSize, sizeof(bool));
    int *pathIndex = (int *)malloc(edgesSize * sizeof(int));
    for (int i = 0; i < edgesSize; i++) {
        pathIndex[i] = -1;
    }

    int maxCycleLength = 0;
    int currentPathIndex = 0;

    for (int i = 0; i < edgesSize; i++) {
        if (!visited[i]) {
            dfs_ciclo_maior(edges, i, visited, pathIndex, &currentPathIndex, &maxCycleLength, pathIndex);
        }
    }

    free(visited);
    free(pathIndex);

    if (maxCycleLength == 0) {
        return -1;
    }
    return maxCycleLength;
}