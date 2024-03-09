#include <stdio.h>
#include <stdlib.h>

int ** malloc2dR(int nr, int nc);
void separa(int ** matrice, int nr, int nc, int * v_bianco, int * v_nero);

int main() {
    int nr, nc, ** matrice, i, j, * v_bianco, * v_nero;
    FILE * fin = fopen("mat.txt", "r");
    fscanf(fin, " %d %d ", &nr, &nc);
    matrice = malloc2dR(nr, nc);
    v_bianco = malloc((((nr*nc)/2)+1) * sizeof(int));
    v_nero = malloc((((nr*nc)/2)+1) * sizeof(int));
    for (i = 0; i < nr; i++) {
        for(j = 0; j <nc; j++) {
            fscanf(fin, " %d", &(matrice[i][j]));
        }
    }
    separa(matrice, nr, nc, v_bianco, v_nero);
    fclose(fin);
    for (i = 0; i < nr; i++) free(matrice[i]);
    free(matrice);
}

int ** malloc2dR(int nr, int nc) {
    int ** matrice, i;
    matrice = malloc(nr * sizeof(int *));
    for (i = 0; i < nr; i++) matrice[i] = malloc(nc * sizeof(int));
    return matrice;
}

void separa(int ** matrice, int nr, int nc, int *v_bianco, int *v_nero) {
    int i, j, k = 0;
    for (i = 0; i < nr; i++) {
        for (j = 0; j < nc; j++) {
            if ((i+j)%2 == 0 || (i == 0 && j == 0)) v_bianco[k] = matrice[i][j];
            else {
                v_nero[k] = matrice[i][j];
                k++;
            }
        }
    }
    printf("Vettore bianco:\n");
    for(i = 0; v_bianco[i] != '\0'; i++) printf("%d ", v_bianco[i]);
    printf("\n");
    
    printf("Vettore nero:\n");
    for(i = 0; v_nero[i] != '\0'; i++) printf("%d ", v_nero[i]);
    printf("\n");
    
    free(v_bianco);
    free(v_nero);
}
