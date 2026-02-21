#include <stdio.h>
#include <stdlib.h>

int ** malloc2dR(int nr, int nc);
void separa(int ** matrice, int nr, int nc, int **v_bianco, int *nb, int **v_nero, int *nn);

int main() {
    int nr, nc, ** matrice, i, j, * v_bianco, * v_nero, nb = 0, nn = 0;
    FILE * fin = fopen("mat.txt", "r");
    fscanf(fin, " %d %d ", &nr, &nc);
    matrice = malloc2dR(nr, nc);
    for (i = 0; i < nr; i++) {
        for(j = 0; j <nc; j++) {
            fscanf(fin, " %d", &(matrice[i][j]));
        }
    }
    separa(matrice, nr, nc, &v_bianco, &nb, &v_nero, &nn);
    fclose(fin);
    printf("Vettore bianco:\n");
    for (i = 0; i < nb; i++) printf("%d ", v_bianco[i]);
    printf("\n");
    printf("Vettore nero:\n");
    for (i = 0; i < nn; i++) printf("%d ", v_nero[i]);
    printf("\n");
    for (i = 0; i < nr; i++) free(matrice[i]);
    free(matrice);
    free(v_bianco);
    free(v_nero);
    return 0;
}

int ** malloc2dR(int nr, int nc) {
    int ** matrice, i;
    matrice = malloc(nr * sizeof(int *));
    for (i = 0; i < nr; i++) matrice[i] = malloc(nc * sizeof(int));
    return matrice;
}

void separa(int ** matrice, int nr, int nc, int **v_bianco, int *nb, int **v_nero, int *nn) {
    int i, j;
    int size_b = (nr * nc + 1) / 2;
    int size_n = (nr * nc) / 2;
    *v_bianco = malloc(size_b * sizeof(int));
    *v_nero = malloc(size_n * sizeof(int));
    *nb = 0;
    *nn = 0;
    for (i = 0; i < nr; i++) {
        for (j = 0; j < nc; j++) {
            if ((i + j) % 2 == 0) {
                (*v_bianco)[(*nb)++] = matrice[i][j];
            } else {
                (*v_nero)[(*nn)++] = matrice[i][j];
            }
        }
    }
}
