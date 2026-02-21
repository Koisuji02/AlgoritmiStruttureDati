#include <stdio.h>
#include <string.h>
#include <ctype.h>

static int match_atom(char c, const char *re, int *consumed);
char *cercaRegexp(char *src, char *regexp);

int main(void) {
    char src[] = "Ee5t2";
    char regexp[] = "\\A[aei]5t[123]";
    char *p = cercaRegexp(src, regexp);
    if (p == NULL) {
        printf("Sequenza non trovata!\n");
    } else {
        printf("Trovata: %s\n", p);
    }
    return 0;
}

static int match_atom(char c, const char *re, int *consumed) {
    if (re[0] == '.') {
        *consumed = 1;
        return 1;
    }
    if (re[0] == '\\') {
        *consumed = 2;
        if (re[1] == 'a') return islower((unsigned char)c) != 0;
        if (re[1] == 'A') return isupper((unsigned char)c) != 0;
        return 0;
    }
    if (re[0] == '[') {
        int i = 1;
        int neg = 0;
        int ok = 0;
        if (re[i] == '^') {
            neg = 1;
            i++;
        }
        while (re[i] != ']' && re[i] != '\0') {
            if (re[i] == c) ok = 1;
            i++;
        }
        if (re[i] != ']') {
            *consumed = 1;
            return 0;
        }
        *consumed = i + 1;
        return neg ? !ok : ok;
    }
    *consumed = 1;
    return re[0] == c;
}

char *cercaRegexp(char *src, char *regexp) {
    size_t i, j;
    size_t len_src = strlen(src);

    for (i = 0; i < len_src; i++) {
        size_t s = i;
        j = 0;
        while (regexp[j] != '\0' && s < len_src) {
            int consumed = 0;
            if (!match_atom(src[s], &regexp[j], &consumed)) break;
            s++;
            j += (size_t)consumed;
        }
        if (regexp[j] == '\0') return &src[i];
    }
    return NULL;
}
