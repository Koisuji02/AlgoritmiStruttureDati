#include "inv.h"

void stat_read(FILE *fp, stat_t *statp){
    fscanf(fp, " %d %d %d %d %d %d", &statp->hp, &statp->mp, &statp->atk, &statp->def, &statp->mag, &statp->spr);
}

void stat_print(FILE *fp, stat_t *statp, int soglia){
    if (soglia == MIN_STAT) {
        if (statp->hp < soglia) statp->hp = soglia;
        if (statp->mp < soglia) statp->mp = soglia;
        if (statp->atk < soglia) statp->atk = soglia;
        if (statp->def < soglia) statp->def = soglia;
        if (statp->mag < soglia) statp->mag = soglia;
        if (statp->spr < soglia) statp->spr = soglia;
    }
    fprintf(fp, " %d %d %d %d %d %d", statp->hp, statp->mp, statp->atk, statp->def, statp->mag, statp->spr);
}

void stat_init(stat_t * statp){
    statp->hp = 0;
    statp->mp = 0;
    statp->atk = 0;
    statp->def = 0;
    statp->mag = 0;
    statp->spr = 0;
}

stat_t stat_somma(stat_t * statp1, stat_t * statp2){
    stat_t somma;
    somma.hp = statp1->hp + statp2 ->hp;
    somma.mp = statp1->mp + statp2 ->mp;
    somma.atk = statp1->atk + statp2 ->atk;
    somma.def = statp1->def + statp2 ->def;
    somma.mag = statp1->mag + statp2 ->mag;
    somma.spr = statp1->spr + statp2 ->spr;
    return somma;
}

void inv_read(FILE *fp, inv_t *invp) {

    stat_t statp;
    fscanf(fp, " %s %s", invp->nome, invp->tipo);
    stat_read(fp, &statp);
    invp->stat = statp;
}

void inv_print(FILE *fp, inv_t *invp, int flag){
    fprintf(fp, "\n->->-> %s %s", invp->nome, invp->tipo);
    if (flag == 0) stat_print(fp, &invp->stat, MIN_STAT);
    else stat_print(fp, &invp->stat, MIN_ASS);
}

stat_t inv_getStat(inv_t *invp){ // penso che sparirà
    return invp->stat;
}
