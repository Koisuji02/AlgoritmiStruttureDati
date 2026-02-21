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

static diag_t best_diag[MAX_DIAG];
static float best_score = -1.0f;

static void solve_program(int d, int total_diff, float total_score, int has_fwd, int has_bwd, int has_two_acro);
static void build_diag(int d, diag_t *diag, int pos, int entry_dir, int diff, float score, int has_acro, int last_acro, int has_two_acro, int has_fwd, int has_bwd, int total_diff, float total_score);

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

    solve_program(0, 0, 0.0f, 0, 0, 0);

    if (best_score < 0.0f) {
        printf("Nessuna soluzione trovata.\n");
        return 0;
    }

    printf("Miglior punteggio: %.2f\n", best_score);
    for (int d = 0; d < MAX_DIAG; d++) {
        printf("Diagonale %d: ", d + 1);
        for (int i = 0; i < best_diag[d].len; i++) {
            printf("%s ", elems[best_diag[d].idx[i]].nome);
        }
        printf("(diff=%d, score=%.2f)\n", best_diag[d].diff, best_diag[d].score);
    }

    return 0;
}

static void solve_program(int d, int total_diff, float total_score, int has_fwd, int has_bwd, int has_two_acro) {
    if (d == MAX_DIAG) {
        if (has_fwd && has_bwd && has_two_acro && total_diff <= DP) {
            if (total_score > best_score) {
                best_score = total_score;
            }
        }
        return;
    }

    diag_t diag;
    diag.len = 0;
    diag.diff = 0;
    diag.score = 0.0f;
    diag.has_acro = 0;
    diag.has_two_acro = 0;
    build_diag(d, &diag, 0, 1, 0, 0.0f, 0, 0, 0, has_fwd, has_bwd, total_diff, total_score);
}

static void build_diag(int d, diag_t *diag, int pos, int entry_dir, int diff, float score, int has_acro, int last_acro, int has_two_acro, int has_fwd, int has_bwd, int total_diff, float total_score) {
    if (pos > 0) {
        if (has_acro && diff <= DD && total_diff + diff <= DP) {
            float diag_score = score;
            if (d == MAX_DIAG - 1) {
                int last_idx = diag->idx[pos - 1];
                if (elems[last_idx].finale == 1 && elems[last_idx].diff >= 8) diag_score *= 1.5f;
            }
            diag->len = pos;
            diag->diff = diff;
            diag->score = diag_score;
            diag->has_acro = has_acro;
            diag->has_two_acro = has_two_acro;

            int fwd = has_fwd;
            int bwd = has_bwd;
            for (int i = 0; i < pos; i++) {
                if (elems[diag->idx[i]].tipo == 2) fwd = 1;
                if (elems[diag->idx[i]].tipo == 1) bwd = 1;
            }
            solve_program(d + 1, total_diff + diff, total_score + diag_score, fwd, bwd, has_two_acro || diag->has_two_acro);
        }
    }

    if (pos == MAX_ELEM_DIAG) return;

    for (int i = 0; i < N; i++) {
        if (elems[i].prec == 1 && pos == 0) continue;
        if (elems[i].in_dir != entry_dir) continue;
        if (diff + elems[i].diff > DD) continue;

        diag->idx[pos] = i;
        int is_acro = (elems[i].tipo == 1 || elems[i].tipo == 2);
        int new_has_acro = has_acro || is_acro;
        int new_has_two_acro = has_two_acro || (last_acro && is_acro);

        if (elems[i].finale == 1) {
            build_diag(d, diag, pos + 1, elems[i].out_dir, diff + elems[i].diff,
                       score + elems[i].valore, new_has_acro, is_acro, new_has_two_acro,
                       has_fwd, has_bwd, total_diff, total_score);
        } else {
            build_diag(d, diag, pos + 1, elems[i].out_dir, diff + elems[i].diff,
                       score + elems[i].valore, new_has_acro, is_acro, new_has_two_acro,
                       has_fwd, has_bwd, total_diff, total_score);
        }
    }
}
