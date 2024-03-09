#include <stdio.h>
#include <stdlib.h>

typedef struct tessera{
    char colore1;
    int valore1;
    char colore2;
    int valore2;
}tessera_t, *p_tessere;

typedef struct casella{
    int index;
    int rotazione;
}casella_t, *p_caselle, ** mat_caselle;

p_tessere file_tessere(int * N);
mat_caselle file_matrice(int * r, int * c);
p_caselle converti_mat_vet(mat_caselle mat , int r, int c);
mat_caselle converti_vet_mat(p_caselle vet , int r, int c);
p_caselle punteggio_massimo(p_caselle v_caselle, p_tessere v_tessere, int N, int r, int c);
int punteggio_massimoR(p_caselle v_caselle, p_caselle v_max, p_tessere v_tessere, int * mark, int N, int r, int c, int pos, int punteggio_locale, int * massimo);
void ruota_tessera(p_tessere v_tessere, int i);


int main(){
    int N, r, c, i, j;
    p_tessere v_tessere = file_tessere(&N);
    mat_caselle mat = file_matrice(&r, &c), mat_max;
    p_caselle v_caselle, vet_max;
    v_caselle = converti_mat_vet(mat, r, c);
    vet_max = punteggio_massimo(v_caselle, v_tessere, N, r, c);
    mat_max = converti_vet_mat(vet_max, r, c);
    printf(" Matrice con punteggio massimo:\n");
    for(i = 0; i < r; i++){
        for(j = 0; j < c; j++){
        	if(mat_max[i][j].rotazione == 1) printf("%c %d %c %d \t", v_tessere[mat_max[i][j].index].colore2, v_tessere[mat_max[i][j].index].valore2, v_tessere[mat_max[i][j].index].colore1, v_tessere[mat_max[i][j].index].valore1);
        	else if(mat_max[i][j].rotazione == 0) printf("%c %d %c %d \t", v_tessere[mat_max[i][j].index].colore1, v_tessere[mat_max[i][j].index].valore1, v_tessere[mat_max[i][j].index].colore2, v_tessere[mat_max[i][j].index].valore2);
        }
        printf("\n");
    }
    return 0;
}

p_tessere file_tessere(int * N){

    int i;
    p_tessere vet;
    FILE * fin = fopen("tiles.txt", "r");
    fscanf(fin, " %d", N);
    vet = malloc((*N) * sizeof(tessera_t));

    for (i = 0; i < (*N); i++) fscanf(fin, " %c %d %c %d", &vet[i].colore1, &vet[i].valore1, &vet[i].colore2, &vet[i].valore2);
    return vet;
}

mat_caselle file_matrice(int * r, int * c){

    mat_caselle mat;
    int i, j;
    FILE * fin = fopen("board.txt", "r");
    fscanf(fin, " %d %d", r, c);
    mat = malloc((*r) * sizeof(p_caselle));
    for(i = 0; i < (*r); i++) mat[i] = malloc((*c) * sizeof(casella_t));

    for(i = 0; i < (*r); i++){
        for(j = 0; j < (*c); j++) fscanf(fin, " %d/%d", &mat[i][j].index, &mat[i][j].rotazione);
    }
    return mat;
}

p_caselle converti_mat_vet(mat_caselle mat , int r, int c){

    int i, j, k;
    p_caselle vet = malloc(r*c * sizeof(casella_t));
    for (i = 0; i < r; i++){
        k = i*r;
        for (j = 0; j < c; j++){
            vet[k+j] = mat[i][j];
        }
    }
    return vet;

}

mat_caselle converti_vet_mat(p_caselle vet , int r, int c){

    int i, j, k;
    mat_caselle mat = malloc(r * sizeof(p_caselle));
    for (i = 0; i < r; i++) mat[i] = malloc(c * sizeof(casella_t));
    for (i = 0; i < r; i++){
        k = i*r;
        for (j = 0; j < c; j++){
            mat[i][j] = vet[k+j];
        }
    }
    return mat;
}

p_caselle punteggio_massimo(p_caselle v_caselle, p_tessere v_tessere, int N, int r, int c){

    int i, massimo = 0, *mark = calloc(N, sizeof(int));
    p_caselle v_max = malloc(r*c * sizeof(casella_t));


    for(i = 0; i < r*c; i++){
        if (v_caselle[i].index != -1) mark[v_caselle[i].index] = 1;
    }
    massimo = punteggio_massimoR(v_caselle, v_max, v_tessere, mark, N, r, c, 0, 0, &massimo);
    return v_max;
}

int punteggio_massimoR(p_caselle v_caselle, p_caselle v_max, p_tessere v_tessere, int * mark, int N, int r, int c, int pos, int punteggio_locale, int * massimo){

    int i, k = 0, flag, punti_tmp;

    if(pos >= r*c){
        //RIGHE
        while(k < r*c){
            flag = 0;
            punti_tmp = 0;
            for(i = 0; i < c; i++){
                if(v_caselle[k+i].index == -1) flag = 1;
                if(flag == 0) if(v_tessere[v_caselle[k+i].index].colore1 != v_tessere[v_caselle[k].index].colore1) flag = 1;
                if(flag == 0) punti_tmp += v_tessere[v_caselle[k+i].index].valore1;
            }
            if (flag == 0) punteggio_locale += punti_tmp;
            k += r;
        }
        k = 0;
        //COLONNE
        while(k < c){
            flag = 0;
            punti_tmp = 0;
            for(i = 0; i < r*c; i += c){
                if(v_caselle[k+i].index == -1) flag = 1;
                if(flag == 0) if(v_tessere[v_caselle[k+i].index].colore2 != v_tessere[v_caselle[k].index].colore2) flag = 1;
                if(flag == 0) punti_tmp += v_tessere[v_caselle[k+i].index].valore2;
            }
            if (flag == 0) punteggio_locale += punti_tmp;
            k++;
        }
        if(punteggio_locale > *massimo){
            *massimo = punteggio_locale;
            for (i = 0; i < r*c; i++){
                v_max[i].index = v_caselle[i].index;
                v_max[i].rotazione = v_caselle[i].rotazione;
            }
        }
        return *massimo;
    }

    for(i = 0; i < N; i++) {
        if (v_caselle[pos].index == -1) {
            if (mark[i] == 0) {
                v_caselle[pos].index = i;
                v_caselle[pos].rotazione = 0;
                mark[i] = 1;
                punteggio_massimoR(v_caselle, v_max, v_tessere, mark, N, r, c, pos + 1, punteggio_locale, massimo);
                //RUOTATO
                ruota_tessera(v_tessere, i);
                v_caselle[pos].rotazione = 1;
                punteggio_massimoR(v_caselle, v_max, v_tessere, mark, N, r, c, pos + 1, punteggio_locale, massimo);
                ruota_tessera(v_tessere, i);
                //TOLTO
                v_caselle[pos].index = -1;
                v_caselle[pos].rotazione = -1;
                mark[i] = 0;
            }
        }
    }
    if(v_caselle[pos].index != -1) punteggio_massimoR(v_caselle, v_max, v_tessere, mark, N, r, c, pos+1, punteggio_locale, massimo);
    return * massimo;
}

void ruota_tessera(p_tessere v_tessere, int i){
    char colore_tmp = v_tessere[i].colore1;
    int valore_tmp = v_tessere[i].valore1;
    v_tessere[i].colore1 = v_tessere[i].colore2;
    v_tessere[i].colore2 = colore_tmp;
    v_tessere[i].valore1 = v_tessere[i].valore2;
    v_tessere[i].valore2 = valore_tmp;
}
