#include "equipArray.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct equipArray_s {
    int inUso;
    int idx[EQUIP_SLOT];
};

equipArray_t equipArray_init(void) {
    equipArray_t eq = malloc(sizeof(struct equipArray_s));
    eq->inUso = 0;
    for (int i = 0; i < EQUIP_SLOT; i++) eq->idx[i] = -1;
    return eq;
}

void equipArray_free(equipArray_t equipArray) {
    free(equipArray);
}

int equipArray_inUse(equipArray_t equipArray) {
    return equipArray->inUso;
}

void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray) {
    for (int i = 0; i < equipArray->inUso; i++) {
        inv_print(fp, invArray_getByIndex(invArray, equipArray->idx[i]), 0);
    }
}

void equipArray_update(equipArray_t equipArray, invArray_t invArray) {
    char name[LEN];
    char scelta;
    int pos;
    do {
        printf("\nInserisci A per aggiungere, T per togliere un equipaggiamento o F per fine:\n");
        scanf(" %c", &scelta);
        scelta = (char)toupper((unsigned char)scelta);
        switch (scelta) {
            case 'A':
                if (equipArray->inUso >= EQUIP_SLOT) {
                    printf("Slot pieni!\n");
                    break;
                }
                printf("Inserisci il nome dell'equipaggiamento:\n");
                scanf(" %s", name);
                pos = invArray_searchByName(invArray, name);
                if (pos >= 0) {
                    equipArray->idx[equipArray->inUso++] = pos;
                } else {
                    printf("Nome errato!\n");
                }
                break;
            case 'T':
                if (equipArray->inUso == 0) {
                    printf("Slot vuoti!\n");
                    break;
                }
                printf("Inserisci il nome dell'equipaggiamento:\n");
                scanf(" %s", name);
                pos = invArray_searchByName(invArray, name);
                if (pos >= 0) {
                    int found = 0;
                    for (int i = 0; i < equipArray->inUso; i++) {
                        if (equipArray->idx[i] == pos) {
                            for (int j = i; j < equipArray->inUso - 1; j++)
                                equipArray->idx[j] = equipArray->idx[j + 1];
                            equipArray->idx[equipArray->inUso - 1] = -1;
                            equipArray->inUso--;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) printf("Oggetto non presente in equip.\n");
                } else {
                    printf("Nome errato!\n");
                }
                break;
            case 'F':
                break;
            default:
                printf("Comando errato!\n");
                break;
        }
    } while (scelta != 'F');
}

int equipArray_getEquipByIndex(equipArray_t equipArray, int index) {
    if (index < 0 || index >= equipArray->inUso) return -1;
    return equipArray->idx[index];
}

stat_t equipArray_eqStat(equipArray_t equipArray, invArray_t invArray) {
    stat_t acc;
    stat_init(&acc);
    for (int i = 0; i < equipArray->inUso; i++) {
        const inv_t *invp = invArray_getByIndex(invArray, equipArray->idx[i]);
        stat_t s = inv_getStat(invp);
        acc = stat_somma(&acc, &s);
    }
    return acc;
}
