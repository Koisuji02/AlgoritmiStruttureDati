#include "pgList.h"
#include <stdlib.h>
#include <string.h>

struct pgList_s {
    pg_t pg;
    struct pgList_s *next;
};

pgList_t pgList_init(void) {
    return NULL;
}

void pgList_free(pgList_t pgList) {
    pgList_t curr = pgList;
    while (curr != NULL) {
        pgList_t next = curr->next;
        pg_clean(&curr->pg);
        free(curr);
        curr = next;
    }
}

pgList_t pgList_read(FILE *fp, pgList_t pgList) {
    pg_t pg1;
    while (pg_read(fp, &pg1) != 0) {
        pgList = pgList_insert(pgList, pg1);
    }
    return pgList;
}

void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray) {
    pgList_t node1;
    printf("\nPersonaggi:\n");
    for (node1 = pgList; node1 != NULL; node1 = node1->next) {
        pg_print(fp, &node1->pg, invArray);
        printf("\n");
    }
}

pgList_t pgList_insert(pgList_t pgList, pg_t pg) {
    pgList_t node = malloc(sizeof(struct pgList_s));
    pgList_t tail;
    if (node == NULL) return pgList;
    node->pg = pg;
    node->next = NULL;
    if (pgList == NULL) return node;
    tail = pgList;
    while (tail->next != NULL) tail = tail->next;
    tail->next = node;
    return pgList;
}

pgList_t pgList_remove(pgList_t pgList, const char *cod) {
    pgList_t curr = pgList;
    pgList_t prev = NULL;
    if (pgList == NULL) {
        printf("\nErrore! Lista vuota!\n");
        return pgList;
    }
    while (curr != NULL) {
        if (strcmp(curr->pg.cod, cod) == 0) {
            if (prev == NULL) pgList = curr->next;
            else prev->next = curr->next;
            pg_clean(&curr->pg);
            free(curr);
            return pgList;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("\nPg non trovato!\n");
    return pgList;
}

pg_t *pgList_searchByCode(pgList_t pgList, const char *cod) {
    pgList_t p;
    if (pgList == NULL) {
        printf("\nErrore! Lista vuota!\n");
        return NULL;
    }
    for (p = pgList; p != NULL; p = p->next) {
        if (strcmp(p->pg.cod, cod) == 0) return &p->pg;
    }
    printf("\nPg non trovato!\n");
    return NULL;
}
