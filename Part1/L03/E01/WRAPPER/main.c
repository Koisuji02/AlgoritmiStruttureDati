// VERSIONE CHE USA UN WRAPPER, MA CHE EFFETTUA "PRUNING" SUI CASI FINALI

#include <stdio.h>
#define MAX_N 7

int majority(int *a, int N);
int majorityR(int *a, int pos, int N);

int main() {
    int c, vet[MAX_N] = {3,3,9,4,3,5,3};
    c = majority(vet,MAX_N);
    printf("%d\n", c);
    return 0;
}

int majority(int *a, int N) {
    int c, pos = 0;
    c = majorityR(a,pos,N);
    return c;
}

int majorityR(int *a, int pos, int N) {
    int i, counter = 0;
    if (pos > N/2) return -1; //PRUNING SUI CASI INUTILI FINALI
    for (i = pos; i < N; i++)
        if (a[pos] == a[i]) counter ++;
    if (counter > N/2) return a[pos];
    return majorityR(a, pos+1, N);
}
