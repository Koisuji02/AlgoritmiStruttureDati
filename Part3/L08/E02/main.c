#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 31

typedef struct edge_s {
    int v;
    int w;
    struct edge_s *next;
} edge_t;

typedef struct {
    int V;
    int **madj;
    edge_t **ladj;
    char **names;
} graph_t;

static int name_index(graph_t *g, const char *name) {
    for (int i = 0; i < g->V; i++) {
        if (strcmp(g->names[i], name) == 0) return i;
    }
    return -1;
}

static int add_name(graph_t *g, const char *name) {
    g->names = realloc(g->names, (g->V + 1) * sizeof(char *));
    g->names[g->V] = malloc(MAX_NAME);
    strcpy(g->names[g->V], name);
    g->V += 1;
    return g->V - 1;
}

static graph_t *graph_init(void) {
    graph_t *g = malloc(sizeof(graph_t));
    g->V = 0;
    g->madj = NULL;
    g->ladj = NULL;
    g->names = NULL;
    return g;
}

static void graph_free(graph_t *g) {
    if (g->madj != NULL) {
        for (int i = 0; i < g->V; i++) free(g->madj[i]);
        free(g->madj);
    }
    if (g->ladj != NULL) {
        for (int i = 0; i < g->V; i++) {
            edge_t *e = g->ladj[i];
            while (e != NULL) {
                edge_t *tmp = e->next;
                free(e);
                e = tmp;
            }
        }
        free(g->ladj);
    }
    for (int i = 0; i < g->V; i++) free(g->names[i]);
    free(g->names);
    free(g);
}

static void ensure_matrix(graph_t *g) {
    int oldV = g->V;
    if (g->madj == NULL) {
        g->madj = malloc(oldV * sizeof(int *));
        for (int i = 0; i < oldV; i++) {
            g->madj[i] = calloc(oldV, sizeof(int));
        }
        return;
    }
}

static void graph_add_edge(graph_t *g, int u, int v, int w) {
    ensure_matrix(g);
    g->madj[u][v] = w;
    g->madj[v][u] = w;
}

static graph_t *g_sort_ctx = NULL;
static int cmp_names(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return strcmp(g_sort_ctx->names[ia], g_sort_ctx->names[ib]);
}

static void print_vertices_sorted(graph_t *g) {
    int *idx = malloc(g->V * sizeof(int));
    for (int i = 0; i < g->V; i++) idx[i] = i;
    g_sort_ctx = g;
    qsort(idx, g->V, sizeof(int), cmp_names);

    for (int i = 0; i < g->V; i++) {
        int v = idx[i];
        printf("%s:", g->names[v]);
        int *nei = malloc(g->V * sizeof(int));
        int n = 0;
        for (int j = 0; j < g->V; j++) {
            if (g->madj[v][j] > 0) nei[n++] = j;
        }
        if (n > 1) qsort(nei, n, sizeof(int), cmp_names);
        for (int k = 0; k < n; k++) {
            printf(" %s(%d)", g->names[nei[k]], g->madj[v][nei[k]]);
        }
        printf("\n");
        free(nei);
    }
    free(idx);
}

static void build_list_from_matrix(graph_t *g) {
    if (g->ladj != NULL) return;
    g->ladj = malloc(g->V * sizeof(edge_t *));
    for (int i = 0; i < g->V; i++) g->ladj[i] = NULL;
    for (int i = 0; i < g->V; i++) {
        for (int j = i + 1; j < g->V; j++) {
            if (g->madj[i][j] > 0) {
                edge_t *e1 = malloc(sizeof(edge_t));
                edge_t *e2 = malloc(sizeof(edge_t));
                e1->v = j; e1->w = g->madj[i][j]; e1->next = g->ladj[i]; g->ladj[i] = e1;
                e2->v = i; e2->w = g->madj[i][j]; e2->next = g->ladj[j]; g->ladj[j] = e2;
            }
        }
    }
}

static int clique_matrix(graph_t *g, int a, int b, int c) {
    return g->madj[a][b] > 0 && g->madj[a][c] > 0 && g->madj[b][c] > 0;
}

static int adjacent_list(edge_t *head, int v) {
    for (; head != NULL; head = head->next) if (head->v == v) return 1;
    return 0;
}

static int clique_list(graph_t *g, int a, int b, int c) {
    if (g->ladj == NULL) return 0;
    return adjacent_list(g->ladj[a], b) && adjacent_list(g->ladj[a], c) && adjacent_list(g->ladj[b], c);
}

int main(void) {
    char filename[256];
    printf("Nome file grafo: ");
    scanf(" %255s", filename);
    FILE *fin = fopen(filename, "r");
    if (fin == NULL) return 1;

    graph_t *g = graph_init();
    char id1[MAX_NAME], r1[MAX_NAME], id2[MAX_NAME], r2[MAX_NAME];
    int w;
    while (fscanf(fin, " %30s %30s %30s %30s %d", id1, r1, id2, r2, &w) == 5) {
        int i1 = name_index(g, id1);
        int i2 = name_index(g, id2);
        if (i1 < 0) i1 = add_name(g, id1);
        if (i2 < 0) i2 = add_name(g, id2);
        if (g->madj == NULL) {
            g->madj = malloc(g->V * sizeof(int *));
            for (int i = 0; i < g->V; i++) g->madj[i] = calloc(g->V, sizeof(int));
        } else if (g->V > 0 && g->madj != NULL) {
            for (int i = 0; i < g->V - 1; i++) {
                g->madj[i] = realloc(g->madj[i], g->V * sizeof(int));
                g->madj[i][g->V - 1] = 0;
            }
            g->madj = realloc(g->madj, g->V * sizeof(int *));
            g->madj[g->V - 1] = calloc(g->V, sizeof(int));
        }
        graph_add_edge(g, i1, i2, w);
    }
    fclose(fin);

    int scelta = -1;
    while (scelta != 0) {
        printf("\n1) Stampa vertici/archi\n2) Verifica tripla (matrice)\n3) Verifica tripla (lista)\n4) Genera lista adiacenza\n0) Fine\n");
        scanf(" %d", &scelta);
        if (scelta == 1) {
            print_vertices_sorted(g);
        } else if (scelta == 2) {
            char a[MAX_NAME], b[MAX_NAME], c[MAX_NAME];
            printf("Inserisci tre vertici: ");
            scanf(" %30s %30s %30s", a, b, c);
            int ia = name_index(g, a), ib = name_index(g, b), ic = name_index(g, c);
            if (ia < 0 || ib < 0 || ic < 0) printf("Vertici non trovati.\n");
            else printf("%s\n", clique_matrix(g, ia, ib, ic) ? "Completo" : "Non completo");
        } else if (scelta == 3) {
            char a[MAX_NAME], b[MAX_NAME], c[MAX_NAME];
            printf("Inserisci tre vertici: ");
            scanf(" %30s %30s %30s", a, b, c);
            int ia = name_index(g, a), ib = name_index(g, b), ic = name_index(g, c);
            if (ia < 0 || ib < 0 || ic < 0) printf("Vertici non trovati.\n");
            else printf("%s\n", clique_list(g, ia, ib, ic) ? "Completo" : "Non completo");
        } else if (scelta == 4) {
            build_list_from_matrix(g);
            printf("Lista di adiacenza generata.\n");
        }
    }

    graph_free(g);
    return 0;
}
