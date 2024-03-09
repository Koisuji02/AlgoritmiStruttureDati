// VERSIONE PIU' COMPATTA SENZA USO DI WRAPPER, MA CHE EFETTUA ANCHE LE ITERAZIONI INUTILI AL FONDO

#include <stdio.h>
#define MAX_N 7

int majority(int *a, int N);

void main() {
    int c, vet[MAX_N] = {1,2,2,3,3,2,2};
    printf("%d\n", majority(vet,MAX_N));
}

int majority(int *a, int N) {
    int k = 0, counter = 0;
    if(*a == '\0') return -1;
    while(*(a+k) != '\0') {
    	if (*a == *(a+k)) counter ++;
    	k++;
	}
    if (counter > N/2) return *a;
    return majority(a+1, N);
}
