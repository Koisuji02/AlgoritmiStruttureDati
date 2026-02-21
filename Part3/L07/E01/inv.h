#ifndef INV_H_DEFINED
#define INV_H_DEFINED

#include <stdio.h>

#define LEN 100
#define MIN_STAT 0
#define MIN_ASS 1

typedef struct stat_s {
    int hp, mp, atk, def, mag, spr;
} stat_t;

typedef struct inv_s {
    char nome[LEN];
    char tipo[LEN];
    stat_t stat;
} inv_t;

void stat_read(FILE *fp, stat_t *statp);
void stat_print(FILE *fp, const stat_t *statp, int soglia);
void stat_init(stat_t *statp);
stat_t stat_somma(const stat_t *statp1, const stat_t *statp2);

void inv_read(FILE *fp, inv_t *invp);
void inv_print(FILE *fp, const inv_t *invp, int flag);
stat_t inv_getStat(const inv_t *invp);

#endif
