#include <stdio.h>
#include <stdlib.h>

typedef struct {int estremo1; int estremo2;} arco_t;

int compare (int * a, int* b) {
    return (*a - *b);
}

int trova_archi(int * vetNodi ,arco_t * vetArchi, int E, int * sol, int pos, int val, int start, int end, int counter, int controllo);
void print_archi(int * sol, int E);
void print_consecutivi(int * sol, int * vetNodi, int E, int index, int num_rimasti);

int main() {
    int N, E, i, k, indice = 0, * sol, * vetNodi, flag1, flag2;
    arco_t * vetArchi;
    FILE * fin = fopen("grafo.txt", "r");
    fscanf(fin, "%d %d", &N, &E);
    vetArchi = malloc(E * sizeof(arco_t));
    for (i = 0; i < E; i++) fscanf(fin, " %d %d", &(vetArchi[i].estremo1), &(vetArchi[i].estremo2));
    vetNodi = malloc(N * sizeof(int)); vetNodi[0] = -1;
    for (i = 0; i < E; i++) {
        flag1  = 1;
        flag2 = 1;
        for (k = 0; k < indice; k++) {
            if (vetArchi[i].estremo1 == vetNodi[k]) flag1 = 0;
            if (vetArchi[i].estremo2 == vetNodi[k]) flag2 = 0;
        }
        if (flag1) {
            vetNodi[indice] = vetArchi[i].estremo1;
            indice++;
        }
        if (flag2) {
            vetNodi[indice] = vetArchi[i].estremo2;
            indice++;
        }
    }
    fclose(fin);
    sol = calloc(E, sizeof(int));
    trova_archi(vetNodi, vetArchi, E, sol, 0, 0, 0, E, 0, 1);
    return 0;
}

int trova_archi(int * vetNodi ,arco_t * vetArchi, int E, int * sol, int pos, int val, int start, int end, int counter, int controllo) {
    int i, k, j, temp, num_rimasti, index;
    if (controllo) {
        for (j = 1; (pos + j) < end; j++) {
            for (i = start; i < end; i++) {
                if (vetNodi[pos + val] == vetArchi[i].estremo1 || vetNodi[pos + val] == vetArchi[i].estremo2) {
                    sol[i] = vetNodi[pos + val];
                    counter++;
                } else {
                    counter += trova_archi(vetNodi, vetArchi, E, sol, pos, val+j, i, i + 1, counter, 0);
                }
            }
            if (counter == E) {
                printf("( "); print_archi(sol, E); printf(")\n");
                qsort (sol, 4, sizeof(int), compare);
                num_rimasti = 0;
                for (k = 0; k < E; k++) {
                    if(vetNodi[k] > sol[E-1]) {
                        num_rimasti++;
                        if(num_rimasti == 1) index = k;
                    }
                }
                print_consecutivi(sol, vetNodi, E, index, num_rimasti);
            }
            counter = 0;
        }
    }
    else {
        temp = counter;
        for (i = start; i < end; i++) {
            if (vetNodi[pos + val] == vetArchi[i].estremo1 || vetNodi[pos + val] == vetArchi[i].estremo2) {
                sol[i] = vetNodi[pos + val];
                counter++;
            } else {
                if ((pos+val+1)<E) counter += trova_archi(vetNodi, vetArchi, E, sol, pos, val + 1, i, i + 1, counter, 0);
            }
        }
        return (counter-temp);
    }
    if((pos+1) < E) return trova_archi(vetNodi,vetArchi, E, sol, pos+1, 0, 0,E, 0,1);
    return 0;
}

void print_archi(int * sol, int E) {
    int i, k, flag, tmp[E];
    for (i = 0; i < E; i++) {
        flag = 1;
        for (k = 0; k < i; k++) if (sol[i] == tmp[k]) flag = 0;
        tmp[i] = sol[i];
        if (flag) printf("%d ", sol[i]);
    }
}

void print_consecutivi(int * sol, int * vetNodi, int E, int index, int num_rimasti) {
    int k = 0, c, l;
    while (k < num_rimasti) {
        for(c = index; c < index + num_rimasti ; c++) {
            if (c+k < E) {
                printf("( "); print_archi(sol, E);
                for (l = c; l <= (c+k) && l< E; l++) printf("%d ", vetNodi[l]);
                printf(")\n");
            }
        }
        k++;
    }
}
