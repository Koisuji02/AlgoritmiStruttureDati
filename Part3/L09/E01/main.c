#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 31

typedef struct {
    int u;
    int v;
    int w;
} edge_t;

static int has_cycle_util(int v, int *state, int **adj, int N) {
    state[v] = 1;
    for (int u = 0; u < N; u++) {
        if (adj[v][u] > 0) {
            if (state[u] == 1) return 1;
            if (state[u] == 0 && has_cycle_util(u, state, adj, N)) return 1;
        }
    }
    state[v] = 2;
    return 0;
}

static int is_dag(int **adj, int N) {
    int *state = calloc(N, sizeof(int));
    int ok = 1;
    for (int i = 0; i < N && ok; i++) {
        if (state[i] == 0 && has_cycle_util(i, state, adj, N)) ok = 0;
    }
    free(state);
    return ok;
}

static void topo_sort(int **adj, int N, int *order) {
    int *indeg = calloc(N, sizeof(int));
    int *queue = malloc(N * sizeof(int));
    int head = 0, tail = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) if (adj[i][j] > 0) indeg[j]++;
    }
    for (int i = 0; i < N; i++) if (indeg[i] == 0) queue[tail++] = i;

    int k = 0;
    while (head < tail) {
        int v = queue[head++];
        order[k++] = v;
        for (int u = 0; u < N; u++) {
            if (adj[v][u] > 0) {
                indeg[u]--;
                if (indeg[u] == 0) queue[tail++] = u;
            }
        }
    }
    free(indeg);
    free(queue);
}

int main(void) {
    FILE *fin = fopen("grafo.txt", "r");
    if (fin == NULL) return 1;

    int N;
    if (fscanf(fin, " %d", &N) != 1) {
        fclose(fin);
        return 1;
    }

    char **names = malloc(N * sizeof(char *));
    for (int i = 0; i < N; i++) {
        names[i] = malloc(MAX_NAME);
        fscanf(fin, " %30s", names[i]);
    }

    edge_t *edges = NULL;
    int E = 0;
    int cap = 0;
    char id1[MAX_NAME], id2[MAX_NAME];
    int w;
    while (fscanf(fin, " %30s %30s %d", id1, id2, &w) == 3) {
        int u = -1, v = -1;
        for (int i = 0; i < N; i++) {
            if (strcmp(names[i], id1) == 0) u = i;
            if (strcmp(names[i], id2) == 0) v = i;
        }
        if (u < 0 || v < 0) continue;
        if (E == cap) {
            cap = cap == 0 ? 16 : cap * 2;
            edges = realloc(edges, cap * sizeof(edge_t));
        }
        edges[E].u = u;
        edges[E].v = v;
        edges[E].w = w;
        E++;
    }
    fclose(fin);

    int **adj = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) adj[i] = calloc(N, sizeof(int));
    for (int i = 0; i < E; i++) adj[edges[i].u][edges[i].v] = edges[i].w;

    int best_remove = E + 1;
    int best_weight = -1;
    unsigned long best_mask = 0;

    unsigned long total_masks = (E >= (int)(8 * sizeof(unsigned long))) ? 0 : (1UL << E);
    if (total_masks == 0) {
        printf("Numero di archi troppo grande per la ricerca esaustiva.\n");
        for (int i = 0; i < N; i++) free(adj[i]);
        free(adj);
        for (int i = 0; i < N; i++) free(names[i]);
        free(names);
        free(edges);
        return 0;
    }
    for (unsigned long mask = 0; mask < total_masks; mask++) {
        int removed = 0;
        int weight = 0;
        for (int i = 0; i < E; i++) {
            if (mask & (1UL << i)) {
                removed++;
                weight += edges[i].w;
                adj[edges[i].u][edges[i].v] = 0;
            }
        }
        if (removed <= best_remove && is_dag(adj, N)) {
            if (removed < best_remove || weight > best_weight) {
                best_remove = removed;
                best_weight = weight;
                best_mask = mask;
            }
        }
        for (int i = 0; i < E; i++) {
            if (mask & (1UL << i)) adj[edges[i].u][edges[i].v] = edges[i].w;
        }
    }

    printf("Rimozione minima: %d archi, peso massimo: %d\n", best_remove, best_weight);
    for (int i = 0; i < E; i++) {
        if (best_mask & (1UL << i)) {
            printf("Rimuovi: %s -> %s (%d)\n", names[edges[i].u], names[edges[i].v], edges[i].w);
            adj[edges[i].u][edges[i].v] = 0;
        }
    }

    int *order = malloc(N * sizeof(int));
    topo_sort(adj, N, order);

    int *indeg = calloc(N, sizeof(int));
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) if (adj[i][j] > 0) indeg[j]++;
    for (int s = 0; s < N; s++) {
        if (indeg[s] == 0) {
            int *dist = malloc(N * sizeof(int));
            for (int i = 0; i < N; i++) dist[i] = -1;
            dist[s] = 0;
            for (int i = 0; i < N; i++) {
                int v = order[i];
                if (dist[v] >= 0) {
                    for (int u = 0; u < N; u++) {
                        if (adj[v][u] > 0 && dist[u] < dist[v] + adj[v][u]) {
                            dist[u] = dist[v] + adj[v][u];
                        }
                    }
                }
            }
            printf("\nDistanze massime dalla sorgente %s:\n", names[s]);
            for (int i = 0; i < N; i++) {
                printf("%s: %d\n", names[i], dist[i]);
            }
            free(dist);
        }
    }

    free(indeg);
    free(order);
    for (int i = 0; i < N; i++) free(adj[i]);
    free(adj);
    for (int i = 0; i < N; i++) free(names[i]);
    free(names);
    free(edges);
    return 0;
}
