#include <stdio.h>
#include <stdlib.h>

typedef enum { ZAFFIRO, RUBINO, TOPAZIO, SMERALDO } gem_t;

static int can_follow(gem_t prev, gem_t next) {
    if (prev == ZAFFIRO) return next == ZAFFIRO || next == RUBINO;
    if (prev == SMERALDO) return next == SMERALDO || next == TOPAZIO;
    if (prev == RUBINO) return next == SMERALDO || next == TOPAZIO;
    return next == ZAFFIRO || next == RUBINO;
}

static void backtrack(int z, int r, int t, int s, gem_t last,
                      char *sol, int pos, char *best, int *best_len) {
    if (pos > *best_len) {
        int i;
        *best_len = pos;
        for (i = 0; i < pos; i++) best[i] = sol[i];
        best[pos] = '\0';
    }
    if (z == 0 && r == 0 && t == 0 && s == 0) return;

    if (z > 0 && (pos == 0 || can_follow(last, ZAFFIRO))) {
        sol[pos] = 'Z';
        backtrack(z - 1, r, t, s, ZAFFIRO, sol, pos + 1, best, best_len);
    }
    if (r > 0 && (pos == 0 || can_follow(last, RUBINO))) {
        sol[pos] = 'R';
        backtrack(z, r - 1, t, s, RUBINO, sol, pos + 1, best, best_len);
    }
    if (t > 0 && (pos == 0 || can_follow(last, TOPAZIO))) {
        sol[pos] = 'T';
        backtrack(z, r, t - 1, s, TOPAZIO, sol, pos + 1, best, best_len);
    }
    if (s > 0 && (pos == 0 || can_follow(last, SMERALDO))) {
        sol[pos] = 'S';
        backtrack(z, r, t, s - 1, SMERALDO, sol, pos + 1, best, best_len);
    }
}

int main(void) {
    int z, r, t, s, nCollane, i;
    FILE *fin = fopen("very_easy_test_set.txt", "r");
    if (fin == NULL) return 1;
    if (fscanf(fin, " %d", &nCollane) != 1) {
        fclose(fin);
        return 1;
    }

    for (i = 0; i < nCollane; i++) {
        int best_len = 0;
        char *best;
        char *sol;
        if (fscanf(fin, " %d %d %d %d", &z, &r, &t, &s) != 4) break;
        sol = malloc((z + r + t + s + 1) * sizeof(char));
        best = malloc((z + r + t + s + 1) * sizeof(char));
        best[0] = '\0';

        backtrack(z, r, t, s, ZAFFIRO, sol, 0, best, &best_len);

        printf("TEST #%02d:\nZaffiri: %d Rubini: %d Topazi: %d Smeraldi: %d\n", i + 1, z, r, t, s);
        printf("Lunghezza massima: %d\n", best_len);
        printf("Collana: %s\n\n", best);

        free(sol);
        free(best);
    }

    fclose(fin);
    return 0;
}
