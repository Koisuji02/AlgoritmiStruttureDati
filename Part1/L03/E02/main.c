#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAR 255

typedef struct{
    int n;
    char ** elenco;
}scelta_t;

int genera_playlist(scelta_t * playlist, int amici, char ** sol, int pos, int counter, int ** mark, int i);

int main() {
    int amici, i, j, **mark, cnt;
    scelta_t * playlist;
    char ** sol;
    FILE *fin = fopen("brani.txt", "r");
    fscanf(fin," %d", &amici);
    playlist = malloc(amici * sizeof(scelta_t));
    sol = malloc(amici * sizeof(char*));
    mark = malloc(amici * sizeof(int*));
    for(i = 0; i < amici; i++){
        fscanf(fin, " %d", &playlist[i].n);
        playlist[i].elenco = malloc(playlist[i].n * sizeof(char*));
        sol[i] = malloc(MAX_CHAR * sizeof(char));
        mark[i] = calloc(playlist[i].n, sizeof(int));
        for(j = 0; j < playlist[i].n; j++) {
            playlist[i].elenco[j] = malloc(MAX_CHAR * sizeof(char));
            fscanf(fin, " %s", playlist[i].elenco[j]);
        }
    }
    fclose(fin);
    cnt = genera_playlist(playlist, amici, sol, 0, 0, mark, 0);
    printf("\nNumero di scelte possibili: %d\n", cnt);

    for(i = 0; i < amici; i++) {
        for(j = 0; j < playlist[i].n; j++) free(playlist[i].elenco[j]);
        free(playlist[i].elenco);
        free(mark[i]);
        free(sol[i]);
    }
    free(playlist);
    free(sol);
    free(mark);
    return 0;
}

int genera_playlist(scelta_t * playlist, int amici, char ** sol, int pos, int counter, int ** mark, int i) {
    int j, k;
    if (pos >= amici) {
        for(k = 0; k < amici; k++) printf(" %s |", sol[k]);
        printf("\n");
        return counter + 1;
    }
    for (j = 0; j < playlist[i].n; j++) {
        if (mark[i][j] == 0) {
            mark[i][j] = 1;
            strcpy(sol[pos], playlist[i].elenco[j]);
            counter = genera_playlist(playlist, amici, sol, pos+1, counter, mark, i+1);
            mark[i][j] = 0;
        }
    }
    return counter;
}
