#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_CHAR 30

typedef enum comando { r_date, r_codiceTratta, r_lettura, r_partenza, r_capolinea, r_ricerca, r_stampa, r_fine, r_error } comando_e;

typedef struct {
    char codice_tratta[MAX_CHAR + 1];
    char partenza[MAX_CHAR + 1];
    char destinazione[MAX_CHAR + 1];
    char data[MAX_CHAR + 1];
    char ora_partenza[MAX_CHAR + 1];
    char ora_arrivo[MAX_CHAR + 1];
    int ritardo;
} corsa_t;

static comando_e leggicomando(void);
static void stampa(corsa_t *vetCorse, int nCorse);
static void multiordinamento(corsa_t *vetCorse, int nCorse, comando_e c);
static void ricerca(corsa_t *vetCorse, int nCorse);
static corsa_t *leggi_file(const char *filename, int *nCorse);
static int starts_with_case_insensitive(const char *s, const char *prefix);
static int compare_prefix_case_insensitive(const char *s, const char *prefix);
static int ricerca_dicotomica(corsa_t *vetCorse, int nCorse, const char *prefix);

int main(void) {
    int nCorse = 0;
    corsa_t *vetCorse = leggi_file("corse.txt", &nCorse);
    comando_e c;
    int flag = 1;

    while (flag) {
        c = leggicomando();
        switch (c) {
            case r_lettura: {
                char filename[MAX_CHAR + 1];
                printf("Inserisci nome del file da leggere:\n");
                scanf(" %s", filename);
                free(vetCorse);
                vetCorse = leggi_file(filename, &nCorse);
            } break;
            case r_stampa:
                stampa(vetCorse, nCorse);
                break;
            case r_date:
            case r_codiceTratta:
            case r_partenza:
            case r_capolinea:
                multiordinamento(vetCorse, nCorse, c);
                break;
            case r_ricerca:
                ricerca(vetCorse, nCorse);
                break;
            case r_fine:
                flag = 0;
                break;
            default:
                printf("Comando errato!\n");
                break;
        }
    }
    free(vetCorse);
    return 0;
}

static comando_e leggicomando(void) {
    char comando[MAX_CHAR + 1];
    printf("\nScegli comando (lettura, stampa, date, tratta, partenza, capolinea, ricerca, fine):\n");
    scanf(" %s", comando);
    for (int i = 0; comando[i] != '\0'; i++) comando[i] = (char)tolower((unsigned char)comando[i]);
    if (strcmp(comando, "date") == 0) return r_date;
    if (strcmp(comando, "lettura") == 0) return r_lettura;
    if (strcmp(comando, "tratta") == 0) return r_codiceTratta;
    if (strcmp(comando, "partenza") == 0) return r_partenza;
    if (strcmp(comando, "capolinea") == 0) return r_capolinea;
    if (strcmp(comando, "ricerca") == 0) return r_ricerca;
    if (strcmp(comando, "stampa") == 0) return r_stampa;
    if (strcmp(comando, "fine") == 0) return r_fine;
    return r_error;
}

static void stampa(corsa_t *vetCorse, int nCorse) {
    char target[MAX_CHAR + 1];
    FILE *out = stdout;
    printf("Stampa su video o file? (video/file)\n");
    scanf(" %s", target);
    for (int i = 0; target[i] != '\0'; i++) target[i] = (char)tolower((unsigned char)target[i]);
    if (strcmp(target, "file") == 0) {
        char filename[MAX_CHAR + 1];
        printf("Nome file di output:\n");
        scanf(" %s", filename);
        out = fopen(filename, "w");
        if (out == NULL) return;
    }
    for (int i = 0; i < nCorse; i++) {
        fprintf(out, " %s %s %s %s %s %s %d\n",
                vetCorse[i].codice_tratta, vetCorse[i].partenza, vetCorse[i].destinazione,
                vetCorse[i].data, vetCorse[i].ora_partenza, vetCorse[i].ora_arrivo, vetCorse[i].ritardo);
    }
    if (out != stdout) fclose(out);
}

static void multiordinamento(corsa_t *vetCorse, int nCorse, comando_e c) {
    int i, j;
    corsa_t **p = malloc(nCorse * sizeof(corsa_t *));
    for (i = 0; i < nCorse; i++) p[i] = &vetCorse[i];

    for (i = 1; i < nCorse; i++) {
        corsa_t *key = p[i];
        j = i - 1;
        while (j >= 0) {
            int cmp = 0;
            if (c == r_codiceTratta) cmp = strcmp(p[j]->codice_tratta, key->codice_tratta);
            else if (c == r_partenza) cmp = strcmp(p[j]->partenza, key->partenza);
            else if (c == r_capolinea) cmp = strcmp(p[j]->destinazione, key->destinazione);
            else {
                cmp = strcmp(p[j]->data, key->data);
                if (cmp == 0) cmp = strcmp(p[j]->ora_partenza, key->ora_partenza);
            }
            if (cmp <= 0) break;
            p[j + 1] = p[j];
            j--;
        }
        p[j + 1] = key;
    }

    for (i = 0; i < nCorse; i++) {
        printf(" %s %s %s %s %s %s %d\n",
               p[i]->codice_tratta, p[i]->partenza, p[i]->destinazione,
               p[i]->data, p[i]->ora_partenza, p[i]->ora_arrivo, p[i]->ritardo);
    }
    free(p);
}

static void ricerca(corsa_t *vetCorse, int nCorse) {
    char prefix[MAX_CHAR + 1];
    char scelta[MAX_CHAR + 1];
    int found = 0;

    printf("Inserisci la stazione di partenza (anche parzialmente):\n");
    scanf(" %s", prefix);
    printf("Scegli tra ricerca lineare o dicotomica:\n");
    scanf(" %s", scelta);
    for (int i = 0; scelta[i] != '\0'; i++) scelta[i] = (char)tolower((unsigned char)scelta[i]);

    if (strcmp(scelta, "lineare") == 0) {
        for (int i = 0; i < nCorse; i++) {
            if (starts_with_case_insensitive(vetCorse[i].partenza, prefix)) {
                printf("-> Trovata la corsa: %s %s %s %s %s %s %d\n",
                       vetCorse[i].codice_tratta, vetCorse[i].partenza, vetCorse[i].destinazione,
                       vetCorse[i].data, vetCorse[i].ora_partenza, vetCorse[i].ora_arrivo, vetCorse[i].ritardo);
                found = 1;
            }
        }
        if (!found) printf("Corsa non trovata!\n");
    } else if (strcmp(scelta, "dicotomica") == 0) {
        ricerca_dicotomica(vetCorse, nCorse, prefix);
    }
}

static int ricerca_dicotomica(corsa_t *vetCorse, int nCorse, const char *prefix) {
    int i, left = 0, right = nCorse - 1, found = -1;
    corsa_t **p = malloc(nCorse * sizeof(corsa_t *));
    for (i = 0; i < nCorse; i++) p[i] = &vetCorse[i];

    for (i = 1; i < nCorse; i++) {
        corsa_t *key = p[i];
        int j = i - 1;
        while (j >= 0 && strcmp(p[j]->partenza, key->partenza) > 0) {
            p[j + 1] = p[j];
            j--;
        }
        p[j + 1] = key;
    }

    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = compare_prefix_case_insensitive(p[mid]->partenza, prefix);
        if (cmp == 0) {
            found = mid;
            break;
        }
        if (cmp < 0) left = mid + 1;
        else right = mid - 1;
    }

    if (found == -1) {
        printf("Corsa non trovata!\n");
        free(p);
        return -1;
    }
    while (found > 0 && starts_with_case_insensitive(p[found - 1]->partenza, prefix)) found--;
    while (found < nCorse && starts_with_case_insensitive(p[found]->partenza, prefix)) {
        printf("-> Trovata la corsa: %s %s %s %s %s %s %d\n",
               p[found]->codice_tratta, p[found]->partenza, p[found]->destinazione,
               p[found]->data, p[found]->ora_partenza, p[found]->ora_arrivo, p[found]->ritardo);
        found++;
    }
    free(p);
    return 0;
}

static corsa_t *leggi_file(const char *filename, int *nCorse) {
    int i;
    corsa_t *vetCorse;
    FILE *fcorse = fopen(filename, "r");
    if (fcorse == NULL) return NULL;
    if (fscanf(fcorse, " %d", nCorse) != 1) {
        fclose(fcorse);
        return NULL;
    }
    vetCorse = malloc((*nCorse) * sizeof(corsa_t));
    for (i = 0; i < (*nCorse); i++) {
        fscanf(fcorse, " %s %s %s %s %s %s %d",
               vetCorse[i].codice_tratta, vetCorse[i].partenza, vetCorse[i].destinazione,
               vetCorse[i].data, vetCorse[i].ora_partenza, vetCorse[i].ora_arrivo, &vetCorse[i].ritardo);
    }
    fclose(fcorse);
    return vetCorse;
}

static int starts_with_case_insensitive(const char *s, const char *prefix) {
    size_t i = 0;
    for (; prefix[i] != '\0'; i++) {
        if (s[i] == '\0') return 0;
        if (tolower((unsigned char)s[i]) != tolower((unsigned char)prefix[i])) return 0;
    }
    return 1;
}

static int compare_prefix_case_insensitive(const char *s, const char *prefix) {
    size_t i = 0;
    for (; prefix[i] != '\0'; i++) {
        if (s[i] == '\0') return -1;
        if (tolower((unsigned char)s[i]) != tolower((unsigned char)prefix[i]))
            return tolower((unsigned char)s[i]) - tolower((unsigned char)prefix[i]);
    }
    return 0;
}
