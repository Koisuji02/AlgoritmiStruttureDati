#include <stdio.h>
#include <stdlib.h>
#define MAX_CHAR (30+1)

typedef struct{
    int inizio;
    int fine;
}att;

int compare (const void * a, const void * b);
att * attSort(att * v, int N);
att * leggi_file(int * N);
void attSelDP(int N, att * v, int * somme);

int main(){

    int N, *somme;
    att * v = leggi_file(&N);
    somme = calloc(N, sizeof(int));
    attSort(v, N);
    if (v != NULL){ attSelDP(N, v, somme);}
    return 0;
}

att * leggi_file(int * N){
    att * vet_att;
    int i;
    char * filename = calloc(MAX_CHAR, sizeof(char));
    printf("Inserisci il nome del file da aprire:\n"); scanf(" %s", filename);

    FILE * fin = fopen(filename, "r");
    if (fin!=NULL) {
        fscanf(fin, " %d", N);
        vet_att = calloc(*N, sizeof(att));
        for (i = 0; i < *N; i++) fscanf(fin, " %d %d", &vet_att[i].inizio, &vet_att[i].fine);
    }
    else vet_att = NULL;
    fclose(fin);
    free(filename);
    return vet_att;
}

void attSelDP(int N, att * v, int * somme){

    int k, h, massimo, i, j, pos, index, c = 0;
    somme[0] = v[0].fine - v[0].inizio;
    massimo = somme[0];
    att * sol = malloc(N*sizeof(att));

    for (i = 1; i < N; i++){
        k = 1;
        while(v[i-k].fine > v[i].inizio) if((i-k) > 0) k++; else break;
        pos = i-k;
        h = 1;
        while((pos-h) >= 0){
            if(v[pos-h].fine <= v[i].inizio) {
                if(somme[pos-h] > somme[pos])
                    pos -= h;
                else h++;
            }
            else h++;
        }
        if(v[pos].fine <= v[i].inizio)
            somme[i] = somme[pos] + (v[i].fine - v[i].inizio);
        else
            somme[i] = v[i].fine - v[i].inizio;
        if(somme[i] > massimo){
            massimo = somme[i];
            index = i;
        }
    }
    sol[c] = v[index];
    c++;
    i = index-1;
    while(i >= 0){
        j = i;
        if(v[index].inizio >= v[i].fine){
            k = i-1;
            while(k >= 0){
                if(v[index].inizio >= v[k].fine){
                    if(somme[k] > somme[i]) j = k;
                }
                k--;
            }
            index = j;
            sol[c] = v[j];
            c++;
        }
        i= j-1;
    }
    printf("Coppie:\n");
    for(i = c-1; i >= 0; i--) printf("(%d,%d) ", sol[i].inizio, sol[i].fine);
    printf("\nMassimo: %d", massimo);
    free(sol);
}

att * attSort(att * v, int N){
    qsort(v, N, sizeof(att), compare);
    return v;
}

int compare (const void * a, const void * b) {
    att *att_1 = (att *)a;
    att *att_2 = (att *)b;
	if(att_1->inizio != att_2->inizio) return ( att_1->inizio - att_2->inizio );
	else  return ( att_1->fine - att_2->fine );

}