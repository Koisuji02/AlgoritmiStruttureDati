#include <stdio.h>
#include <stdlib.h>

#define MAX_CHAR (30 + 1)

typedef struct {
    int inizio;
    int fine;
} att;

static int compare_end(const void *a, const void *b) {
    const att *x = (const att *)a;
    const att *y = (const att *)b;
    if (x->fine != y->fine) return x->fine - y->fine;
    return x->inizio - y->inizio;
}

static att *leggi_file(int *N) {
    att *v;
    int i;
    char filename[MAX_CHAR];
    printf("Inserisci il nome del file da aprire:\n");
    scanf(" %s", filename);
    FILE *fin = fopen(filename, "r");
    if (fin == NULL) return NULL;
    if (fscanf(fin, " %d", N) != 1) {
        fclose(fin);
        return NULL;
    }
    v = calloc(*N, sizeof(att));
    for (i = 0; i < *N; i++) fscanf(fin, " %d %d", &v[i].inizio, &v[i].fine);
    fclose(fin);
    return v;
}

static int last_compatible(att *v, int i) {
    int j;
    for (j = i - 1; j >= 0; j--) {
        if (v[j].fine <= v[i].inizio) return j;
    }
    return -1;
}

int main(void) {
    int N, i, j;
    att *v = leggi_file(&N);
    int *p, *dp, *take;

    if (v == NULL) return 1;
    qsort(v, N, sizeof(att), compare_end);

    p = malloc(N * sizeof(int));
    for (i = 0; i < N; i++) p[i] = last_compatible(v, i);

    dp = calloc(N, sizeof(int));
    take = calloc(N, sizeof(int));

    for (i = 0; i < N; i++) {
        int durata = v[i].fine - v[i].inizio;
        int with_i = durata + (p[i] >= 0 ? dp[p[i]] : 0);
        int without_i = (i > 0 ? dp[i - 1] : 0);
        if (with_i > without_i) {
            dp[i] = with_i;
            take[i] = 1;
        } else {
            dp[i] = without_i;
            take[i] = 0;
        }
    }

    printf("Coppie:\n");
    for (i = N - 1; i >= 0; ) {
        if (take[i]) {
            printf("(%d,%d) ", v[i].inizio, v[i].fine);
            i = p[i];
        } else {
            i--;
        }
    }
    printf("\nMassimo: %d\n", dp[N - 1]);

    free(v);
    free(p);
    free(dp);
    free(take);
    return 0;
}
