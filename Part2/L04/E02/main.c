#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CHAR 50

typedef struct {
    char codice[MAX_CHAR];
    char nome[MAX_CHAR];
    char cognome[MAX_CHAR];
    char data[MAX_CHAR];
    char via[MAX_CHAR];
    char citta[MAX_CHAR];
    int cap;
} Item;

typedef struct node {
    Item value;
    struct node *next;
} node_t;

typedef enum {
    c_insertFile,
    c_insertTastiera,
    c_ricercaCodice,
    c_cancellaCodice,
    c_cancellaDate,
    c_stampaFile,
    c_fine,
    c_error
} comando_t;

static comando_t comandi(void);
static node_t *new_node(Item value, node_t *next);
static void to_upper(char *stringa);
static void date_to_key(const char *src, char *dst);
static node_t *insert_sorted(node_t *first, Item value);
static node_t *insert_file(node_t *first);
static node_t *insert_tastiera(node_t *first);
static node_t *ricerca_codice(node_t *first);
static node_t *cancella_codice(node_t **first);
static node_t *cancella_date(node_t **first, const char *data_inizio, const char *data_fine);
static void stampa_lista_file(node_t *first);

int main(void) {
    node_t *first = NULL;
    int flag = 1;

    while (flag) {
        comando_t c = comandi();
        node_t *removed = NULL;
        char data_inizio[MAX_CHAR], data_fine[MAX_CHAR];

        switch (c) {
            case c_insertFile:
                first = insert_file(first);
                break;
            case c_insertTastiera:
                first = insert_tastiera(first);
                break;
            case c_ricercaCodice:
                removed = ricerca_codice(first);
                if (removed != NULL)
                    printf("%s %s %s %s %s %s %05d\n", removed->value.codice, removed->value.nome, removed->value.cognome,
                           removed->value.data, removed->value.via, removed->value.citta, removed->value.cap);
                break;
            case c_cancellaCodice:
                removed = cancella_codice(&first);
                if (removed != NULL) {
                    printf("%s %s %s %s %s %s %05d\n", removed->value.codice, removed->value.nome, removed->value.cognome,
                           removed->value.data, removed->value.via, removed->value.citta, removed->value.cap);
                    free(removed);
                }
                break;
            case c_cancellaDate:
                printf("Inserisci date di inizio e fine nel formato GG/MM/AAAA (separate da uno spazio):\n");
                scanf(" %s %s", data_inizio, data_fine);
                date_to_key(data_inizio, data_inizio);
                date_to_key(data_fine, data_fine);
                do {
                    removed = cancella_date(&first, data_inizio, data_fine);
                    if (removed != NULL) {
                        printf("%s %s %s %s %s %s %05d\n", removed->value.codice, removed->value.nome, removed->value.cognome,
                               removed->value.data, removed->value.via, removed->value.citta, removed->value.cap);
                        free(removed);
                    }
                } while (removed != NULL);
                break;
            case c_stampaFile:
                stampa_lista_file(first);
                break;
            case c_fine:
                flag = 0;
                break;
            default:
                printf("Comando errato, riprova!\n");
                break;
        }
    }

    while (first != NULL) {
        node_t *tmp = first;
        first = first->next;
        free(tmp);
    }
    return 0;
}

static comando_t comandi(void) {
    char stringa[MAX_CHAR];
    printf("Inserisci il comando (inserisci_file, inserisci_tastiera, ricerca_codice, cancella_codice, cancella_date, stampa, fine):\n");
    scanf(" %s", stringa);
    if (strcmp(stringa, "inserisci_file") == 0) return c_insertFile;
    if (strcmp(stringa, "inserisci_tastiera") == 0) return c_insertTastiera;
    if (strcmp(stringa, "ricerca_codice") == 0) return c_ricercaCodice;
    if (strcmp(stringa, "cancella_codice") == 0) return c_cancellaCodice;
    if (strcmp(stringa, "cancella_date") == 0) return c_cancellaDate;
    if (strcmp(stringa, "stampa") == 0) return c_stampaFile;
    if (strcmp(stringa, "fine") == 0) return c_fine;
    return c_error;
}

static node_t *new_node(Item value, node_t *next) {
    node_t *node = malloc(sizeof(node_t));
    if (node == NULL) return NULL;
    node->value = value;
    node->next = next;
    return node;
}

static void to_upper(char *stringa) {
    int i;
    for (i = 0; stringa[i] != '\0'; i++) stringa[i] = (char)toupper((unsigned char)stringa[i]);
}

static void date_to_key(const char *src, char *dst) {
    int g, m, a;
    if (sscanf(src, " %d/%d/%d", &g, &m, &a) == 3) {
        sprintf(dst, "%04d/%02d/%02d", a, m, g);
    }
}

static node_t *insert_sorted(node_t *first, Item value) {
    node_t *node = new_node(value, NULL);
    node_t *p = first;
    node_t *prev = NULL;

    if (first == NULL || strcmp(first->value.data, value.data) < 0) {
        node->next = first;
        return node;
    }
    while (p != NULL && strcmp(p->value.data, value.data) >= 0) {
        prev = p;
        p = p->next;
    }
    prev->next = node;
    node->next = p;
    return first;
}

static node_t *insert_file(node_t *first) {
    char nome_file[MAX_CHAR];
    FILE *fin;
    Item item;
    printf("Inserisci il nome del file da aprire:\n");
    scanf(" %s", nome_file);
    fin = fopen(nome_file, "r");
    if (fin == NULL) return first;
    while (fscanf(fin, " %s %s %s %s %s %s %d", item.codice, item.nome, item.cognome, item.data, item.via, item.citta, &item.cap) == 7) {
        date_to_key(item.data, item.data);
        first = insert_sorted(first, item);
    }
    fclose(fin);
    return first;
}

static node_t *insert_tastiera(node_t *first) {
    Item item;
    printf("Inserire una stringa del tipo \"AXXXX nome cognome data via citta cap\":\n");
    scanf(" %s %s %s %s %s %s %d", item.codice, item.nome, item.cognome, item.data, item.via, item.citta, &item.cap);
    date_to_key(item.data, item.data);
    return insert_sorted(first, item);
}

static node_t *ricerca_codice(node_t *first) {
    char codice_tmp[MAX_CHAR];
    node_t *p;
    printf("Inserire codice da ricercare:\n");
    scanf(" %s", codice_tmp);
    to_upper(codice_tmp);
    for (p = first; p != NULL; p = p->next) {
        if (strcmp(p->value.codice, codice_tmp) == 0) return p;
    }
    return NULL;
}

static node_t *cancella_codice(node_t **first) {
    char codice[MAX_CHAR];
    node_t *p = *first;
    node_t *prev = NULL;
    printf("Inserire codice dell'elemento da eliminare:\n");
    scanf(" %s", codice);
    to_upper(codice);
    while (p != NULL) {
        if (strcmp(p->value.codice, codice) == 0) {
            if (prev == NULL) *first = p->next;
            else prev->next = p->next;
            p->next = NULL;
            return p;
        }
        prev = p;
        p = p->next;
    }
    return NULL;
}

static node_t *cancella_date(node_t **first, const char *data_inizio, const char *data_fine) {
    node_t *p = *first;
    node_t *prev = NULL;
    while (p != NULL) {
        if (strcmp(p->value.data, data_inizio) >= 0 && strcmp(p->value.data, data_fine) <= 0) {
            if (prev == NULL) *first = p->next;
            else prev->next = p->next;
            p->next = NULL;
            return p;
        }
        prev = p;
        p = p->next;
    }
    return NULL;
}

static void stampa_lista_file(node_t *first) {
    char filename[MAX_CHAR];
    node_t *p;
    FILE *fout;
    printf("Inserisci nome file di output:\n");
    scanf(" %s", filename);
    fout = fopen(filename, "w");
    if (fout == NULL) return;
    for (p = first; p != NULL; p = p->next) {
        fprintf(fout, "%s %s %s %s %s %s %05d\n", p->value.codice, p->value.nome, p->value.cognome,
                p->value.data, p->value.via, p->value.citta, p->value.cap);
    }
    fclose(fout);
}
