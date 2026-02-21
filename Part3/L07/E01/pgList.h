#ifndef PGLIST_H_DEFINED
#define PGLIST_H_DEFINED

#include <stdio.h>
#include "pg.h"

typedef struct pgList_s *pgList_t;

pgList_t pgList_init(void);
void pgList_free(pgList_t pgList);
pgList_t pgList_read(FILE *fp, pgList_t pgList);
void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray);
pgList_t pgList_insert(pgList_t pgList, pg_t pg);
pgList_t pgList_remove(pgList_t pgList, const char *cod);
pg_t *pgList_searchByCode(pgList_t pgList, const char *cod);

#endif
