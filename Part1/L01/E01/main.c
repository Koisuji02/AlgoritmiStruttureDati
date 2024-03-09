#include <stdio.h>
#include <string.h>
#include <ctype.h>

char * cercaRegexp(char *src, char *regexp);

int main() {
    char src[5] = {'E', 'e','5', 't', '2'};
    char regexp[14] = {'\\','A','[','a','e','i',']','5', 't', '[','1','2','3',']'};
    char *p = cercaRegexp(src, regexp);
    if (p == NULL) printf("Sequenza non trovata!");
    printf("%c", *p);
    return 0;
}


char * cercaRegexp(char *src, char *regexp) {
    int j, flag = 1, len_regexp = 0, i = 0, traslo = 0, posizione = 0, riparti = 1, counter = 0, k = 0;
    char * puntatore;

    for(j = 0; j < strlen(regexp); j++) {
        if (regexp[j] == '\\') len_regexp += 0;
        else if (regexp[j] == '[') flag = 0;
        else if(regexp[j] == ']') {
            flag = 1;
            len_regexp ++;
        }
        else if(flag == 1) len_regexp ++;
    }

    while (regexp[i+k] != '\0' || src[i+traslo] != '\0') {
        switch(regexp[i+k]) {

            case '.':
                if (riparti == 0) posizione = traslo;
                counter ++;
                break;

            case '[':
                if (riparti == 0) posizione = traslo;
                k ++;
                if (regexp[i+k] == '^') {
                    k ++;
                    if (regexp[i+k] == src[i+traslo]) flag = 1;
                    else flag = 0;
                    k++;
                }
                else {
                    while (regexp[i+k] != ']') {
                        if (regexp[i+k] == src[i+traslo]) flag = 0;
                        k++;
                    }
                }
                if (flag == 0) counter ++;
                break;

            case '\\':
                if (riparti == 0) posizione = traslo;
                k ++;
                if (regexp[i+k] == 'a') {
                    if (islower(src[i+traslo])) counter ++;
                }
                if (regexp[i+k] == 'A') {
                    if (isupper(src[i+traslo])) counter ++;
                }
                break;

            default:
                if (src[i+traslo] != regexp[i+k]) {
                    riparti = 0;
                    i = -1;
                    traslo ++;
                    counter = 0;
                    k = 0;
                    flag = 1;
                }
                else {
                    riparti = 1;
                    counter ++;
                }
                break;
        }

        i++;
    }

    if (counter != len_regexp) puntatore = NULL;
    else puntatore = &src[posizione];
    return puntatore;
}
