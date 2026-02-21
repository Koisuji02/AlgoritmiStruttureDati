#ifndef EQUIPARRAY_H_DEFINED
#define EQUIPARRAY_H_DEFINED

#include <stdio.h>
#include "invArray.h"

#define EQUIP_SLOT 8

typedef struct equipArray_s *equipArray_t;

equipArray_t equipArray_init(void);
void equipArray_free(equipArray_t equipArray);
int equipArray_inUse(equipArray_t equipArray);
void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray);
void equipArray_update(equipArray_t equipArray, invArray_t invArray);
int equipArray_getEquipByIndex(equipArray_t equipArray, int index);
stat_t equipArray_eqStat(equipArray_t equipArray, invArray_t invArray);

#endif
