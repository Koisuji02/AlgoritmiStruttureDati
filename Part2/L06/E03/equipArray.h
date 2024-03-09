#ifndef EQUIPARRAY_H_DEFINED
#define EQUIPARRAY_H_DEFINED

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define EQUIP_SLOT 8

#include "invArray.h"

/* ADT di prima classe collezione di oggetti di equipaggiamento */
typedef struct equipArray_s *equipArray_t;

/* creatore e disruttore */
equipArray_t equipArray_init();
void equipArray_free(equipArray_t equipArray);

/* contatore di quanti equipaggiamenti sono in uso */
int equipArray_inUse(equipArray_t equipArray);

/* scrittura su file */
void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray);

/* modifica equipaggiamento */
void equipArray_update(equipArray_t equipArray, invArray_t invArray);

/* torna indice (nel vettore inventario) dell'oggetto in posizione index di equipArray */
int equipArray_getEquipByIndex(equipArray_t equipArray, int index,  invArray_t invArray);

/* somma le stats degli equipaggiamenti di un certo pg */
stat_t equipArray_eqStat(equipArray_t equipArray);

#endif
