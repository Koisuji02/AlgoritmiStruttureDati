#include "pg.h"

int pg_read(FILE *fp, pg_t *pgp){
    stat_t statp;
    if (fp != NULL) {
        fscanf(fp, " %s %s %s", pgp->cod, pgp->nome, pgp->classe);
        stat_read(fp, &statp);
        pgp->b_stat = statp;
        pgp->equip = equipArray_init();
        return 1;
    }
    return 0;
}

void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray){

    stat_t somma = stat_somma(&pgp->b_stat, &pgp->eq_stat);
    fprintf(fp, "\n-> %s %s %s ", pgp->cod, pgp->nome, pgp->classe);
    stat_print(fp, &pgp->b_stat, MIN_STAT);
    if (equipArray_inUse(pgp->equip) != 0) {
        fprintf(fp, "\n->-> Statistiche totali: ");
        stat_print(fp, &somma, MIN_STAT);
        fprintf(fp, "\n->-> Statistiche equipaggiamento: ");
        stat_print(fp, &pgp->eq_stat, MIN_STAT);
        fprintf(fp, "\n->-> Equipaggiamento in uso: ");
        equipArray_print(fp, pgp->equip, invArray);
    }

}

void pg_clean(pg_t *pgp){
    equipArray_free(pgp->equip);
    pgp->equip = equipArray_init();
}

void pg_updateEquip(pg_t *pgp, invArray_t invArray){
    equipArray_update(pgp->equip, invArray);
    pgp->eq_stat = equipArray_eqStat(pgp->equip);
}