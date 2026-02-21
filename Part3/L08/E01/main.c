#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100
#define MAX_ELEM 512
#define MAX_DIAG 3
#define MAX_ELEM_DIAG 5

typedef struct {
    char nome[MAX_NAME + 1];
    int tipo;       /* 0 trans, 1 acro indietro, 2 acro avanti */
    int in_dir;     /* 0 spalle, 1 frontale */
    int out_dir;    /* 0 spalle, 1 frontale */
    int prec;       /* 0 puo' essere primo, 1 no */
    int finale;     /* 1 finale, 0 no */
    float valore;
    int diff;
} elem_t;

typedef struct {
    int idx[MAX_ELEM_DIAG];
    int len;
    int diff;
    float score;
    int has_acro;
    int has_two_acro;
} diag_t;

static elem_t elems[MAX_ELEM];
static int N;
static int DD, DP;

static int select_best(int entry_dir, int diff, int pos) {
    int best = -1;
    float best_ratio = -1.0f;
    for (int i = 0; i < N; i++) {
        if (elems[i].prec == 1 && pos == 0) continue;
        if (elems[i].in_dir != entry_dir) continue;
        if (diff + elems[i].diff > DD) continue;
        if (elems[i].diff == 0) continue;
        float ratio = elems[i].valore / (float)elems[i].diff;
        if (ratio > best_ratio) {
            best_ratio = ratio;
            best = i;
        }
    }
    return best;
}

int main(void) {
    FILE *fin = fopen("elementi.txt", "r");
    if (fin == NULL) return 1;
    if (fscanf(fin, " %d", &N) != 1) {
        fclose(fin);
        return 1;
    }
    for (int i = 0; i < N; i++) {
        fscanf(fin, " %s %d %d %d %d %d %f %d",
               elems[i].nome, &elems[i].tipo, &elems[i].in_dir, &elems[i].out_dir,
               &elems[i].prec, &elems[i].finale, &elems[i].valore, &elems[i].diff);
    }
    fclose(fin);

    printf("Inserisci DD e DP:\n");
    scanf(" %d %d", &DD, &DP);

    diag_t diags[MAX_DIAG];
    int total_diff = 0;
    float total_score = 0.0f;
    int has_fwd = 0, has_bwd = 0, has_two_acro = 0;

    for (int d = 0; d < MAX_DIAG; d++) {
        diag_t *diag = &diags[d];
        int entry_dir = 1;
        int last_acro = 0;
        diag->len = 0;
        diag->diff = 0;
        diag->score = 0.0f;
        diag->has_acro = 0;
        diag->has_two_acro = 0;

        for (int pos = 0; pos < MAX_ELEM_DIAG; pos++) {
            int sel = select_best(entry_dir, diag->diff, pos);
            if (sel < 0) break;
            diag->idx[pos] = sel;
            diag->len++;
            diag->diff += elems[sel].diff;
            diag->score += elems[sel].valore;

            if (elems[sel].tipo == 1 || elems[sel].tipo == 2) {
                diag->has_acro = 1;
                if (last_acro) diag->has_two_acro = 1;
                last_acro = 1;
                if (elems[sel].tipo == 1) has_bwd = 1;
                if (elems[sel].tipo == 2) has_fwd = 1;
            } else {
                last_acro = 0;
            }
            if (elems[sel].finale == 1) break;
            entry_dir = elems[sel].out_dir;
        }

        if (!diag->has_acro) {
            printf("Diagonale %d non valida (nessun acrobatico).\n", d + 1);
        }
        if (d == MAX_DIAG - 1 && diag->len > 0) {
            int last = diag->idx[diag->len - 1];
            if (elems[last].finale == 1 && elems[last].diff >= 8) diag->score *= 1.5f;
        }

        total_diff += diag->diff;
        total_score += diag->score;
        if (diag->has_two_acro) has_two_acro = 1;
    }

    printf("Punteggio greedy: %.2f (diff=%d)\n", total_score, total_diff);
    for (int d = 0; d < MAX_DIAG; d++) {
        printf("Diagonale %d: ", d + 1);
        for (int i = 0; i < diags[d].len; i++) {
            printf("%s ", elems[diags[d].idx[i]].nome);
        }
        printf("(diff=%d, score=%.2f)\n", diags[d].diff, diags[d].score);
    }

    if (!(has_fwd && has_bwd && has_two_acro) || total_diff > DP) {
        printf("Attenzione: la soluzione greedy non rispetta tutti i vincoli.\n");
    }
    return 0;
}
