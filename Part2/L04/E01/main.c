#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int u;
    int v;
} edge_t;

static int is_cover(edge_t *edges, int E, int *sel) {
    int i;
    for (i = 0; i < E; i++) {
        if (!sel[edges[i].u] && !sel[edges[i].v]) return 0;
    }
    return 1;
}

static void print_cover(int *sel, int N) {
    int i, first = 1;
    printf("(");
    for (i = 0; i < N; i++) {
        if (sel[i]) {
            if (!first) printf(", ");
            printf("%d", i);
            first = 0;
        }
    }
    printf(")\n");
}

static void enum_covers(int pos, int N, edge_t *edges, int E, int *sel) {
    if (pos == N) {
        if (is_cover(edges, E, sel)) {
            print_cover(sel, N);
        }
        return;
    }
    sel[pos] = 0;
    enum_covers(pos + 1, N, edges, E, sel);
    sel[pos] = 1;
    enum_covers(pos + 1, N, edges, E, sel);
}

int main(void) {
    int N, E, i;
    edge_t *edges;
    int *sel;
    FILE *fin = fopen("grafo.txt", "r");
    if (fin == NULL) return 1;
    if (fscanf(fin, " %d %d", &N, &E) != 2) {
        fclose(fin);
        return 1;
    }
    edges = malloc(E * sizeof(edge_t));
    for (i = 0; i < E; i++) {
        fscanf(fin, " %d %d", &edges[i].u, &edges[i].v);
    }
    fclose(fin);

    sel = calloc(N, sizeof(int));
    enum_covers(0, N, edges, E, sel);

    free(sel);
    free(edges);
    return 0;
}
