typedef struct
{
    int node;
    int visited_mask;
    int cost;
} State;

typedef struct QueueNode
{
    State state;
    struct QueueNode *next;
} QueueNode;

typedef struct
{
    QueueNode *head;
    QueueNode *tail;
} Queue;

Queue *createQueue()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->head = q->tail = NULL;
    return q;
}

void enqueue(Queue *q, State state)
{
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    newNode->state = state;
    newNode->next = NULL;
    if (q->tail != NULL)
    {
        q->tail->next = newNode;
    }
    q->tail = newNode;
    if (q->head == NULL)
    {
        q->head = newNode;
    }
}

State dequeue(Queue *q)
{
    QueueNode *temp = q->head;
    State state = temp->state;
    q->head = q->head->next;
    if (q->head == NULL)
    {
        q->tail = NULL;
    }
    free(temp);
    return state;
}

int isEmpty(Queue *q)
{
    return q->head == NULL;
}

int shortestPathLength(int **graph, int graphSize, int *graphColSize)
{
    int all_visited = (1 << graphSize) - 1;
    int visited[graphSize][1 << graphSize];
    memset(visited, 0, sizeof(visited));
    Queue *queue = createQueue();

    for (int i = 0; i < graphSize; i++)
    {
        State initial = {i, 1 << i, 0};
        enqueue(queue, initial);
        visited[i][1 << i] = 1;
    }

    while (!isEmpty(queue))
    {
        State current = dequeue(queue);
        if (current.visited_mask == all_visited)
        {
            return current.cost;
        }
        for (int j = 0; j < graphColSize[current.node]; j++)
        {
            int next = graph[current.node][j];
            int next_mask = current.visited_mask | (1 << next);
            if (!visited[next][next_mask])
            {
                visited[next][next_mask] = 1;
                State next_state = {next, next_mask, current.cost + 1};
                enqueue(queue, next_state);
            }
        }
    }

    return -1;
}