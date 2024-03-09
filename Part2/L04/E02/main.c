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
}Item;

typedef struct node{
    Item value;
    struct node *next;
}node_t;

typedef enum {c_insertFile, c_insertTastiera, c_ricercaCodice, c_cancellaCodice, c_cancellaDate, c_stampaFile, c_fine, c_error} comando_t;


comando_t comandi();
void menu(node_t *first, char * data_inizio, char * data_fine);
node_t * new_node(Item value, node_t * next);
char * modifica_data(char *data);
char *  to_upper(char * stringa);
node_t * insert(node_t * first, node_t * new);
node_t * insert_file(node_t * first);
node_t * insert_tastiera(node_t * first);
node_t * ricerca_codice(node_t * first);
node_t * cancella_codice(node_t * first, int * p_flag);
node_t * cancella_date(node_t * first, char * data_inizio, char * data_fine, int * p_flag);
void stampa_lista_file(node_t * first);

int main() { // TRASFORMARE POI LE CHIAMATE NEL MAIN IN UN MENU CON SELEZIONE (TIPO TYPEDEF ENUM)
    node_t * first = malloc(sizeof(node_t)); first = NULL;
    char * data_inizio = calloc(MAX_CHAR, sizeof(char)), * data_fine = calloc(MAX_CHAR, sizeof(char));
    menu(first, data_inizio, data_fine);
    free(data_inizio);
    free(data_fine);
    free(first);
    return 0;
}

comando_t comandi(){ // rimetti a posto
    char * stringa = calloc(MAX_CHAR, sizeof(char));
    comando_t c;
    printf("Inserisci il comando da immettere (inserisci_file, inserisci_tastiera, ricerca_codice, cancella_codice, cancella_date, stampa, fine):\n");
    scanf(" %s", stringa);
    if (strcmp(stringa, "inserisci_file") == 0) c = c_insertFile;
    else if (strcmp(stringa, "inserisci_tastiera") == 0) c = c_insertTastiera;
    else if (strcmp(stringa, "ricerca_codice") == 0) c = c_ricercaCodice;
    else if (strcmp(stringa, "cancella_codice") == 0) c = c_cancellaCodice;
    else if (strcmp(stringa, "cancella_date") == 0) c = c_cancellaDate;
    else if (strcmp(stringa, "stampa") == 0) c = c_stampaFile;
    else if (strcmp(stringa, "fine") == 0) c = c_fine;
    else c = c_error;
    free(stringa);
    return c;
}

void menu(node_t *first, char * data_inizio, char * data_fine){
    comando_t c;
    node_t * node1 = malloc(sizeof(node_t));
    int flag = 1, f = 0, *p_flag = &f;
    while(flag){
        c = comandi();
        switch(c){
            case c_stampaFile:
                stampa_lista_file(first);
                break;

            case c_cancellaDate:
                printf("Inserisci date di inizio e fine nel formato AAAA/MM/GG (separate da uno spazio):\n");
                scanf(" %s %s",data_inizio, data_fine); // RIMETTI A POSTO
                do{
                    node1 = cancella_date(first, data_inizio, data_fine, p_flag);
                    if (*p_flag == 1) first = first->next;
                    if(node1 != NULL) printf("%s %s %s %s %s %s %05d\n", node1->value.codice, node1->value.nome, node1->value.cognome, node1->value.data, node1->value.via, node1->value.citta, node1->value.cap);
                    *p_flag = 0;
                } while(node1 != NULL);
                break;

            case c_cancellaCodice:
                node1 = cancella_codice(first, p_flag);
                if (*p_flag == 1) first = first->next;
                if(node1 != NULL) printf("%s %s %s %s %s %s %05d\n", node1->value.codice, node1->value.nome, node1->value.cognome, node1->value.data, node1->value.via, node1->value.citta, node1->value.cap);
                break;

            case c_ricercaCodice:
                node1 = ricerca_codice(first);
                if(node1 != NULL) printf("%s %s %s %s %s %s %05d\n", node1->value.codice, node1->value.nome, node1->value.cognome, node1->value.data, node1->value.via, node1->value.citta, node1->value.cap);
                break;

            case c_insertTastiera:
                first = insert_tastiera(first);
                break;

            case c_insertFile:
                first = insert_file(first); // rimetti a posto
                break;

            case c_fine:
                flag = 0;
                break;

            default:
                printf("Comando errato, riprova!\n");
                break;
        }
    }
    free(node1);
}

char * modifica_data(char *data){
    int giorno, mese, anno;
    sscanf(data, " %d/%d/%d", &giorno, &mese, &anno);
    memset(data, 0, strlen(data)); // svuoto la stringa per poi ririempirla
    sprintf(data, "%d/%02d/%02d", anno, mese, giorno);
    return data;
}

char * to_upper(char * stringa) {
    int i;
    for (i = 0; i < MAX_CHAR; i++) stringa[i] = toupper(stringa[i]);
    return stringa;
}

node_t * new_node(Item value, node_t * next){
    node_t * new = malloc(sizeof(node_t));
    if (new == NULL) return NULL;
    else {
        new->value = value;
        new->next = next;
    }
    return new;
}

node_t * insert(node_t * first, node_t * new){
    node_t *p1, *p2;
    if (first == NULL || strcmp(first->value.data, new->value.data) < 0) first = new_node(new->value, first);
    else if(first->next == NULL && strcmp(first->value.data, new->value.data) > 0) first->next = new_node(new->value, NULL);
    else for (p2 = first->next, p1 = first; p2 != NULL; p1 = p2, p2 = p2->next){
            if(strcmp(p2->value.data, new->value.data) < 0) {
                p1->next = new_node(new->value, p2);
                break;
            }
            if(p2->next == NULL && strcmp(p2->value.data, new->value.data) < 0){
                p2->next = new_node(new->value, NULL);
                break;
            }
        }
    return first;
}

node_t * insert_file(node_t * first) {
    char * nome_file = calloc(MAX_CHAR, sizeof(char));
    printf("Inserisci il nome del file da aprire:\n"); scanf(" %s", nome_file);
    node_t * new = malloc(sizeof(node_t));
    FILE * fin = fopen(nome_file, "r");
    while(!feof(fin)){
        if(!feof(fin)){
            fscanf(fin, " %s %s %s %s %s %s %d", new->value.codice, new->value.nome, new->value.cognome, new->value.data, new->value.via, new->value.citta, &new->value.cap);
            strcpy(new->value.data, modifica_data(new->value.data));
            new->next = NULL;
            first = insert(first, new);
        }
    }
    fclose(fin);
    free(nome_file);
    return first;
}

node_t * insert_tastiera(node_t * first) {
    node_t * new = malloc(sizeof(node_t));
    printf("Inserire una stringa del tipo \"AXXXX nome cognome data via citta cap\":\n");
    scanf(" %s %s %s %s %s %s %d", new->value.codice, new->value.nome, new->value.cognome, new->value.data, new->value.via, new->value.citta, &new->value.cap);
    strcpy(new->value.data, modifica_data(new->value.data));
    new->next = NULL;
    first = insert(first, new);
    return first;
}

node_t * ricerca_codice(node_t * first) {
    node_t * node1 = calloc(1, sizeof(node_t));
    char codice_tmp[MAX_CHAR];
    printf("Inserire codice da ricercare:\n"); scanf("%s", codice_tmp);
    strcpy(codice_tmp, to_upper(codice_tmp));
    for (node1 = first; node1 != NULL; node1 = node1->next) if(strcmp(node1->value.codice, codice_tmp) == 0) return node1;
    return NULL;
}

node_t * cancella_codice(node_t * first, int * p_flag){
    char * codice = calloc(MAX_CHAR, sizeof(char));
    printf("Inserire codice dell'elemento da eliminare:\n"); scanf("%s", codice);
    strcpy(codice, to_upper(codice));
    node_t * p1 = calloc(1,sizeof(node_t)), *p2 = calloc(1,sizeof(node_t));
    if(first == NULL) return NULL;
    for(p2 = first, p1 = NULL; p2 != NULL; p1 = p2, p2 = p2->next){
        if(strcmp(p2->value.codice, codice) == 0){
            if (p2 == first) *p_flag = 1; // bypass del primo
            else p1->next = p2->next; // bypass sul nodo puntato da p2
            break;
        }
    }
    free(codice);
    return p2;
}

node_t * cancella_date(node_t * first, char * data_inizio, char * data_fine, int * p_flag){
    node_t * p1 = calloc(1, sizeof(node_t)), *p2 = calloc(1, sizeof(node_t));
    if(first == NULL) return NULL;
    for(p2 = first, p1 = NULL; p2 != NULL; p1 = p2, p2 = p2->next){
        if(strcmp(p2->value.data, data_inizio) > 0 && strcmp(p2->value.data, data_fine) < 0){
            if (p2 == first) *p_flag = 1; // bypass del primo
            else {
                p1->next = p2->next;
                *p_flag = 0;// bypass sul nodo puntato da p2
            }
            break;
        }
    }
    return p2;
};

void stampa_lista_file(node_t * first){
    char * filename = calloc(MAX_CHAR, sizeof(char));
    node_t * p;
    printf("Inserisci nome file di output:\n"); scanf("%s", filename);
    FILE * fout = fopen(filename, "w");
    for(p = first; p != NULL; p = p->next) fprintf(fout, "%s %s %s %s %s %s %05d\n", p->value.codice, p->value.nome, p->value.cognome, p->value.data, p->value.via, p->value.citta, p->value.cap);
    fclose(fout);
    free(filename);
}
