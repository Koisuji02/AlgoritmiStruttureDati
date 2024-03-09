#include <stdio.h>
#include <stdlib.h>
#define MAX_CHAR (30+1)

typedef struct{
    int inizio;
    int fine;
}att;

att * leggi_file(int * num_elementi);
void attSel(int N, att * v);
int attSel_r(int N, att * v, int * sol, int * mark, int j, int durata_locale, int * durata_max, int pos_tmp);

int main(){
    int num_elementi;
    att * v = leggi_file(&num_elementi);
    if (v != NULL){
        attSel(num_elementi, v);
    }
    return 0;
}

att * leggi_file(int * num_elementi){
    att * vet_att;
    int i;
    char * filename = calloc(MAX_CHAR, sizeof(char));
    printf("Inserisci il nome del file da aprire:\n"); scanf(" %s", filename);
    FILE * fin = fopen(filename, "r");
    if (fin!=NULL) {
        fscanf(fin, " %d", num_elementi);
        vet_att = calloc(*num_elementi, sizeof(att));
        for (i = 0; i < *num_elementi; i++) fscanf(fin, " %d %d", &vet_att[i].inizio, &vet_att[i].fine);
    }
    else vet_att = NULL;
    fclose(fin);
    free(filename);
    return vet_att;
}

void attSel(int N, att * v){
    int i, *sol = calloc(N, sizeof(int)), *mark = calloc(N, sizeof(int)), durata_max = 0, durata_locale;
    for(i = 0; i < N; i++){
        durata_locale = attSel_r(N, v, sol, mark, i, 0,  &durata_max, 0);
        if(durata_locale > durata_max) durata_max = durata_locale;
    }
    printf("Coppie:\n");
    for (i = 0; i < N; i++){
        if (sol[i] == 1) printf("(%d,%d)\n", v[i].inizio, v[i].fine);
    }
    printf("Durata massima: %d\n", durata_max);
}

int attSel_r(int N, att * v, int * sol, int * mark, int j, int durata_locale, int * durata_max, int pos_tmp){
    int i;
    if(j >= N){
        if (durata_locale > *durata_max){
            for(i = 0; i < N; i++) sol[i] = mark[i];
            *durata_max = durata_locale;
        }
        return durata_locale;
    }
    if(j == 0 || (j > 0 && !(v[j].inizio < v[pos_tmp].fine && v[pos_tmp].inizio < v[j].fine))) {
        mark[j] = 1;
        durata_locale += (v[j].fine - v[j].inizio);
        durata_locale = attSel_r(N, v, sol, mark, j + 1, durata_locale, durata_max, j);
        mark[j] = 0;
        durata_locale -= (v[j].fine - v[j].inizio);
        durata_locale = attSel_r(N, v, sol, mark, j + 1, durata_locale, durata_max, j-1);

    }
    else durata_locale = attSel_r(N, v, sol, mark, j  + 1, durata_locale, durata_max, pos_tmp);
    return durata_locale;
}