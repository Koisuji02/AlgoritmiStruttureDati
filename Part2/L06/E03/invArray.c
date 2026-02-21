#include "invArray.h"

struct invArray_s {
    inv_t * vet;
    int N;
};

invArray_t invArray_init(){
    invArray_t invArray = malloc(sizeof(struct invArray_s));
    invArray->vet = NULL;
    invArray->N = 0;
    return invArray;
}

void invArray_free(invArray_t invArray){
    free(invArray->vet);
    free(invArray);
}

void invArray_read(FILE *fp, invArray_t invArray){
    int i;
    if(fp != NULL){
        fscanf(fp, " %d", &invArray->N);
        invArray->vet = malloc(invArray->N * sizeof(inv_t));
        for(i = 0; i < invArray->N; i++) inv_read(fp, &invArray->vet[i]);
    }
}

void invArray_print(FILE *fp, invArray_t invArray){
    int i;
    printf("\nInventario:\n");
    for (i = 0; i < invArray->N; i++){
        inv_print(fp, &invArray->vet[i], 1);
    }
}

void invArray_printByIndex(FILE *fp, invArray_t invArray, int index) {
    inv_print(fp, &invArray->vet[index], 1);
}

inv_t *invArray_getByIndex(invArray_t invArray, int index){
    return &invArray->vet[index];
}

int invArray_searchByName(invArray_t invArray, char *name){
    int i;
    for(i = 0; i < invArray->N; i++) if(strcmp(invArray->vet[i].nome, name) == 0) return i;
    return -1;
}

int invArray_len(invArray_t invArray){
    return invArray->N;
}