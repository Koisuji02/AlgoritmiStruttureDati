#ifndef INVARRAY_H_DEFINED
#define INVARRAY_H_DEFINED

#include <stdio.h>
#include "inv.h"

typedef struct invArray_s *invArray_t;

invArray_t invArray_init(void);
void invArray_free(invArray_t invArray);
void invArray_read(FILE *fp, invArray_t invArray);
void invArray_print(FILE *fp, invArray_t invArray);

const inv_t *invArray_getByIndex(invArray_t invArray, int index);
int invArray_searchByName(invArray_t invArray, const char *name);
int invArray_len(invArray_t invArray);

#endif
