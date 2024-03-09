#include "equipArray.h"

struct equipArray_s {
    int inUso; //numero degli equip del pg
    inv_t  ** vettEq;
};

equipArray_t equipArray_init(){
    equipArray_t equipArray = malloc(sizeof(equipArray_t));
    equipArray->vettEq = malloc(EQUIP_SLOT * sizeof(inv_t*));
    equipArray->inUso = 0;
    return equipArray;
}

void equipArray_free(equipArray_t equipArray){
    int i;
    for (i = 0; i < equipArray->inUso; i++) free(equipArray->vettEq[i]);
    free(equipArray->vettEq);
    free(equipArray);
}

int equipArray_inUse(equipArray_t equipArray){
    return equipArray->inUso;
}

void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray){ // PENSO CHE SE NE ANDRà
    int i;
    for (i = 0; i < equipArray->inUso; i++) inv_print(fp, equipArray->vettEq[i], 0);
}

void equipArray_update(equipArray_t equipArray, invArray_t invArray){
    char * name = calloc(LEN, sizeof(char)), scelta;
    int i,j, flag;
    do {
        printf("\nInserisci A per aggiungere, T per togliere un equipaggiamento o F per fine:\n");
        scanf(" %c", &scelta);
        scelta = toupper(scelta);
        flag = 0;
        switch (scelta){
            case 'A':
                if(equipArray->inUso < EQUIP_SLOT){
                    printf("Inserisci il nome dell'equipaggiamento:\n");
                    scanf(" %s", name);
                    equipArray->vettEq[equipArray->inUso] = malloc(sizeof(inv_t));
                    for(i = 0; i < invArray_len(invArray); i++)
                        if (strcmp(name, (invArray_getByIndex(invArray, i))->nome) == 0) {
                            equipArray->vettEq[equipArray->inUso] = invArray_getByIndex(invArray, i);
                            flag = 1;
                        }
                    if (flag == 1) equipArray->inUso++;
                    else{
                        printf("\nErrore, nome errato!\n");
                        free(equipArray->vettEq[equipArray->inUso]);
                    }
                }
                else
                    printf("Slot pieni! Togli qualcosa per reinserire.\n");
                break;

            case 'T':
                if(equipArray->inUso > 0) {
                    printf("Inserisci il nome dell'equipaggiamento:\n");
                    scanf(" %s", name);
                    for(i = 0; i < equipArray->inUso; i++) {
                        if (strcmp(name, equipArray->vettEq[i]->nome) == 0) {
                            for (j = i; j < equipArray->inUso - 1; j++)
                                equipArray->vettEq[j] = equipArray->vettEq[j + 1];
                            //free(equipArray->vettEq[equipArray->inUso-1]);
                            equipArray->vettEq[equipArray->inUso-1] = NULL;
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 1) equipArray->inUso--;
                    else{
                        printf("\nErrore, nome errato!\n");
                    }
                }
                else
                    printf("Slot vuoti! Inserisci qualcosa per togliere.\n");
                break;

            case 'F':
                printf("\nEquipaggiamento aggiornato:");
                equipArray_print(stdout, equipArray, invArray);
                printf("\n");
                break;

            default:
                printf("\nComando errato!");
                break;
        }
    } while(toupper(scelta) != 'F');
    free(name);
}

int equipArray_getEquipByIndex(equipArray_t equipArray, int index,  invArray_t invArray){
    int i;
    for(i = 0; i < invArray_len(invArray); i++)
        if(strcmp(equipArray->vettEq[index]->nome, (invArray_getByIndex(invArray, i))->nome) == 0)
            return i;
    return -1;
}

stat_t equipArray_eqStat(equipArray_t equipArray){
    int i;
    stat_t stat_tmp;
    stat_init(&stat_tmp);
    for (i = 0; i < equipArray->inUso; i++){
        stat_tmp.hp += equipArray->vettEq[i]->stat.hp;
        stat_tmp.mp += equipArray->vettEq[i]->stat.mp;
        stat_tmp.atk += equipArray->vettEq[i]->stat.atk;
        stat_tmp.def += equipArray->vettEq[i]->stat.def;
        stat_tmp.mag += equipArray->vettEq[i]->stat.mag;
        stat_tmp.spr += equipArray->vettEq[i]->stat.spr;
    }
    return stat_tmp;
}