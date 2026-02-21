#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char col_h;
    int val_h;
    char col_v;
    int val_v;
} tile_t;

typedef struct {
    int idx;
    int rot;
} cell_t;

static tile_t *read_tiles(int *N) {
    int i;
    FILE *fin = fopen("tiles.txt", "r");
    tile_t *tiles;
    if (fin == NULL) return NULL;
    if (fscanf(fin, " %d", N) != 1) {
        fclose(fin);
        return NULL;
    }
    tiles = malloc(*N * sizeof(tile_t));
    for (i = 0; i < *N; i++) {
        fscanf(fin, " %c %d %c %d", &tiles[i].col_h, &tiles[i].val_h, &tiles[i].col_v, &tiles[i].val_v);
    }
    fclose(fin);
    return tiles;
}

static cell_t **read_board(int *R, int *C) {
    int i, j;
    FILE *fin = fopen("board.txt", "r");
    cell_t **board;
    if (fin == NULL) return NULL;
    if (fscanf(fin, " %d %d", R, C) != 2) {
        fclose(fin);
        return NULL;
    }
    board = malloc(*R * sizeof(cell_t *));
    for (i = 0; i < *R; i++) board[i] = malloc(*C * sizeof(cell_t));
    for (i = 0; i < *R; i++) {
        for (j = 0; j < *C; j++) {
            fscanf(fin, " %d/%d", &board[i][j].idx, &board[i][j].rot);
        }
    }
    fclose(fin);
    return board;
}

static void get_colors(const tile_t *t, int rot, char *ch, int *vh, char *cv, int *vv) {
    if (rot == 0) {
        *ch = t->col_h;
        *vh = t->val_h;
        *cv = t->col_v;
        *vv = t->val_v;
    } else {
        *ch = t->col_v;
        *vh = t->val_v;
        *cv = t->col_h;
        *vv = t->val_h;
    }
}

static int score_board(cell_t **board, tile_t *tiles, int R, int C) {
    int r, c, score = 0;

    for (r = 0; r < R; r++) {
        char col = 0;
        int ok = 1, sum = 0;
        for (c = 0; c < C; c++) {
            char ch, cv;
            int vh, vv;
            if (board[r][c].idx < 0) { ok = 0; break; }
            get_colors(&tiles[board[r][c].idx], board[r][c].rot, &ch, &vh, &cv, &vv);
            if (c == 0) col = ch;
            if (ch != col) { ok = 0; break; }
            sum += vh;
        }
        if (ok) score += sum;
    }

    for (c = 0; c < C; c++) {
        char col = 0;
        int ok = 1, sum = 0;
        for (r = 0; r < R; r++) {
            char ch, cv;
            int vh, vv;
            if (board[r][c].idx < 0) { ok = 0; break; }
            get_colors(&tiles[board[r][c].idx], board[r][c].rot, &ch, &vh, &cv, &vv);
            if (r == 0) col = cv;
            if (cv != col) { ok = 0; break; }
            sum += vv;
        }
        if (ok) score += sum;
    }

    return score;
}

static void copy_board(cell_t **dst, cell_t **src, int R, int C) {
    int r, c;
    for (r = 0; r < R; r++) {
        for (c = 0; c < C; c++) dst[r][c] = src[r][c];
    }
}

static void solve(cell_t **board, cell_t **best, tile_t *tiles, int N, int R, int C, int pos, int *used, int *best_score) {
    int r = pos / C;
    int c = pos % C;
    int i, rot;

    if (pos == R * C) {
        int score = score_board(board, tiles, R, C);
        if (score > *best_score) {
            *best_score = score;
            copy_board(best, board, R, C);
        }
        return;
    }

    if (board[r][c].idx != -1) {
        solve(board, best, tiles, N, R, C, pos + 1, used, best_score);
        return;
    }

    for (i = 0; i < N; i++) {
        if (!used[i]) {
            used[i] = 1;
            for (rot = 0; rot < 2; rot++) {
                board[r][c].idx = i;
                board[r][c].rot = rot;
                solve(board, best, tiles, N, R, C, pos + 1, used, best_score);
            }
            board[r][c].idx = -1;
            board[r][c].rot = -1;
            used[i] = 0;
        }
    }
}

int main(void) {
    int N, R, C, i, j, best_score = -1;
    int *used;
    tile_t *tiles = read_tiles(&N);
    cell_t **board = read_board(&R, &C);
    cell_t **best;

    if (tiles == NULL || board == NULL) return 1;

    used = calloc(N, sizeof(int));
    best = malloc(R * sizeof(cell_t *));
    for (i = 0; i < R; i++) best[i] = malloc(C * sizeof(cell_t));

    for (i = 0; i < R; i++) {
        for (j = 0; j < C; j++) {
            if (board[i][j].idx != -1) used[board[i][j].idx] = 1;
        }
    }

    solve(board, best, tiles, N, R, C, 0, used, &best_score);

    printf("Matrice con punteggio massimo (score=%d):\n", best_score);
    for (i = 0; i < R; i++) {
        for (j = 0; j < C; j++) {
            printf("%d/%d ", best[i][j].idx, best[i][j].rot);
        }
        printf("\n");
    }

    for (i = 0; i < R; i++) {
        free(board[i]);
        free(best[i]);
    }
    free(board);
    free(best);
    free(tiles);
    free(used);
    return 0;
}
