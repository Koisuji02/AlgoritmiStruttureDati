#ifndef PG_H_DEFINED
#define PG_H_DEFINED

#include <stdio.h>
#include "equipArray.h"

typedef struct pg_s {
    char cod[LEN];
    char nome[LEN];
    char classe[LEN];
    stat_t b_stat, eq_stat;
    equipArray_t equip;
} pg_t;

int pg_read(FILE *fp, pg_t *pgp);
void pg_print(FILE *fp, const pg_t *pgp, invArray_t invArray);
void pg_clean(pg_t *pgp);
void pg_updateEquip(pg_t *pgp, invArray_t invArray);

#endif
