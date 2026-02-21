#include <stdio.h>
#include <stdlib.h>

#define MAX_CHAR (30 + 1)

typedef struct {
    int inizio;
    int fine;
} att;

static att *leggi_file(int *num_elementi);
static int compatibile(att *v, int *sel, int pos);
static void attSel_r(att *v, int N, int pos, int *sel, int *best, int durata);

int main(void) {
    int num_elementi;
    att *v = leggi_file(&num_elementi);
    if (v != NULL) {
        int *sel = calloc(num_elementi, sizeof(int));
        int *best = calloc(num_elementi, sizeof(int));
        int i, durata = 0;

        attSel_r(v, num_elementi, 0, sel, best, durata);
        printf("Coppie:\n");
        for (i = 0; i < num_elementi; i++) {
            if (best[i]) printf("(%d,%d)\n", v[i].inizio, v[i].fine);
        }
        free(sel);
        free(best);
        free(v);
    }
    return 0;
}

static att *leggi_file(int *num_elementi) {
    att *vet_att;
    int i;
    char filename[MAX_CHAR];
    printf("Inserisci il nome del file da aprire:\n");
    scanf(" %s", filename);
    FILE *fin = fopen(filename, "r");
    if (fin != NULL) {
        if (fscanf(fin, " %d", num_elementi) != 1) {
            fclose(fin);
            return NULL;
        }
        vet_att = calloc(*num_elementi, sizeof(att));
        for (i = 0; i < *num_elementi; i++) {
            fscanf(fin, " %d %d", &vet_att[i].inizio, &vet_att[i].fine);
        }
    } else {
        vet_att = NULL;
    }
    if (fin != NULL) fclose(fin);
    return vet_att;
}

static int compatibile(att *v, int *sel, int pos) {
    int i;
    for (i = 0; i < pos; i++) {
        if (sel[i]) {
            if (v[i].inizio < v[pos].fine && v[pos].inizio < v[i].fine) return 0;
        }
    }
    return 1;
}

static void attSel_r(att *v, int N, int pos, int *sel, int *best, int durata) {
    static int durata_max = 0;
    int i;
    if (pos == N) {
        if (durata > durata_max) {
            durata_max = durata;
            for (i = 0; i < N; i++) best[i] = sel[i];
        }
        return;
    }

    sel[pos] = 0;
    attSel_r(v, N, pos + 1, sel, best, durata);

    if (compatibile(v, sel, pos)) {
        sel[pos] = 1;
        attSel_r(v, N, pos + 1, sel, best, durata + (v[pos].fine - v[pos].inizio));
        sel[pos] = 0;
    }
}
