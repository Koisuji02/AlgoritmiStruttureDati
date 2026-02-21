// VERSIONE PIU' COMPATTA SENZA USO DI WRAPPER, MA CHE EFETTUA ANCHE LE ITERAZIONI INUTILI AL FONDO

#include <stdio.h>
#define MAX_N 7

int majority(int *a, int N);

int main(void) {
    int vet[MAX_N] = {1,2,2,3,3,2,2};
    printf("%d\n", majority(vet,MAX_N));
    return 0;
}

int majority(int *a, int N) {
    int i, counter = 0;
    if (N <= 0) return -1;
    for (i = 0; i < N; i++) {
        if (a[0] == a[i]) counter++;
    }
    if (counter > (N + 1) / 2) return a[0];
    return majority(a + 1, N - 1);
}
