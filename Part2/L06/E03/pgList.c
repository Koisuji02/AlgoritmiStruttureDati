#include "pgList.h"

struct pgList_s {
    pg_t pg;
    struct pgList_s * next;
};

pgList_t pgList_init(){
    pgList_t head = malloc(sizeof(struct pgList_s));
    head = NULL;
    return head;
}

void pgList_free(pgList_t pgList){
    pgList_t p1, p2;
    for(p1 = pgList, p2 = pgList->next; p2 != NULL; p1 = p2, p2 = p2->next)
        free(p1);
}

pgList_t pgList_read(FILE *fp, pgList_t pgList){
    pg_t pg1;
    while(!feof(fp)){
        pg_read(fp, &pg1);
        if(!feof(fp)) pgList = pgList_insert(pgList, pg1);
    }
    return pgList;
}

void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray){
    pgList_t node1;
    printf("\nPersonaggi:\n");
    for (node1 = pgList; node1 != NULL; node1 = node1->next){
        pg_print(fp, &node1->pg, invArray);
        printf("\n");
    }

}

pgList_t pgList_insert(pgList_t pgList, pg_t pg){
    pgList_t p1, p2;
    pgList_t tmp = malloc(sizeof(struct pgList_s));
    if(pgList == NULL){
        tmp->pg = pg;
        tmp->next = NULL;
        pgList = tmp;
        return pgList;
    }
    for (p1 = pgList, p2 = pgList->next; p2 != NULL; p1 = p2, p2 = p2->next);
        p2 = malloc(sizeof(struct pgList_s));
        p2->pg = pg;
        p2->next = NULL;
        p1->next = p2;
    return pgList;
}

pgList_t pgList_remove(pgList_t pgList, char * cod){
    pgList_t p1, p2;
    if(pgList == NULL) {printf("\nErrore! Lista vuota!\n"); return pgList;}
    for(p1 = NULL, p2 = pgList; p2 != NULL; p1 = p2, p2 = p2->next) {
        if (strcmp(p2->pg.cod, cod) == 0) {
            if (p2 == pgList) pgList = pgList->next;
            else p1->next = p2->next;
            free(p2);
            return pgList;
        }
    }
    printf("\nPg non trovato!\n"); return pgList;
}

pg_t * pgList_searchByCode(pgList_t pgList, char * cod){
    pgList_t p;
    if(pgList == NULL) {printf("\nErrore! Lista vuota!\n"); return NULL;}
    for(p = pgList; p != NULL; p = p->next) if(strcmp(p->pg.cod, cod) == 0) return &p->pg;
    printf("\nPg non trovato!\n"); return NULL;
}