#include <stdio.h>

void creaCollana(int z, int r, int t, int s, int * len_max);
int inserisci_zaffiri(int z, int r, int t, int s, int counter, int * len_max);
int inserisci_rubini(int z, int r, int t, int s, int counter, int * len_max);
int inserisci_topazi(int z, int r, int t, int s, int counter, int * len_max);
int inserisci_smeraldi(int z, int r, int t, int s, int counter, int * len_max);


int main(){
    int z, r, t, s, nCollane, i, len = 0;
    FILE * fin = fopen("very_easy_test_set.txt", "r");
    if (fin != NULL) {
        fscanf(fin, " %d", &nCollane);
        for (i = 0; i < nCollane; i++) {
            fscanf(fin, " %d %d %d %d", &z, &r, &t, &s);
            creaCollana(z, r, t, s, &len);
            printf("TEST #%02d:\nZaffiri: %d Rubini: %d Topazi: %d Smeraldi: %d TOTALE: %d\nLunghezza massima: %d\n\n", i+1, z, r, t, s, nCollane, len);
            len = 0;
        }
        fclose(fin);
    }
    return 0;
}

void creaCollana(int z, int r, int t, int s, int * len_max) {
    int lunghezza;
    lunghezza = inserisci_zaffiri(z, r, t, s, 0, len_max);
    lunghezza = inserisci_rubini(z, r, t, s, 0, len_max);
    lunghezza = inserisci_topazi(z, r, t, s, 0, len_max);
    lunghezza = inserisci_smeraldi(z, r, t, s, 0, len_max);
}

int inserisci_zaffiri(int z, int r, int t, int s, int counter, int * len_max) {
    if (r > 0) counter = inserisci_rubini(z, r-1, t, s, counter +1, len_max);
    if (z > 0) counter = inserisci_zaffiri(z-1, r, t, s, counter+1, len_max);
    if(counter > *len_max) *len_max = counter;
    return --counter;
}

int inserisci_smeraldi(int z, int r, int t, int s, int counter, int * len_max) {
    if (t > 0) counter = inserisci_topazi(z, r, t-1, s, counter +1, len_max);
    if (s > 0) counter = inserisci_smeraldi(z, r, t, s-1, counter+1, len_max);
    if(counter > *len_max) *len_max = counter;
    return --counter;
}

int inserisci_rubini(int z, int r, int t, int s, int counter, int * len_max) {
    if (s > 0) counter = inserisci_smeraldi(z, r, t, s-1, counter+1, len_max);
    if (t > 0) counter = inserisci_topazi(z, r, t-1, s, counter +1, len_max);
    if(counter > *len_max) *len_max = counter;
    return --counter;
}

int inserisci_topazi(int z, int r, int t, int s, int counter, int * len_max) {
    if (z > 0) counter = inserisci_zaffiri(z-1, r, t, s, counter+1, len_max);
    if (r > 0) counter = inserisci_rubini(z, r-1, t, s, counter +1, len_max);
    if(counter > *len_max) *len_max = counter;
    return --counter;
}
