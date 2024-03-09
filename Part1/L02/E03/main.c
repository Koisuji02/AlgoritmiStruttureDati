#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_CORSE 1000
#define MAX_CHAR 30

typedef enum comando{r_date,r_codiceTratta, r_lettura, r_partenza, r_capolinea, r_fine, r_error}comando_e;

typedef struct{
    char codice_tratta[MAX_CHAR + 1];
    char partenza [MAX_CHAR + 1];
    char destinazione[MAX_CHAR + 1];
    char data[MAX_CHAR + 1];
    char ora_partenza[MAX_CHAR + 1];
    char ora_arrivo[MAX_CHAR + 1];
    int ritardo;
} corse;

int selezionaDati(corse vetCorse[MAX_CORSE],int nCorse);
comando_e leggicomando(void);
void strtolower(char s[MAX_CHAR]);
void multiordinamento(corse vetCorse[MAX_CORSE], int nCorse, comando_e c);
corse * leggi_file(char filename[MAX_CHAR+1], int * nCorse);


int main(void) {
    int nCorse = 0;
    corse * vetCorse = leggi_file("corse.txt", &nCorse);
    selezionaDati(vetCorse, nCorse);
    return 0;

}

int selezionaDati(corse vetCorse[MAX_CORSE],int nCorse) {
    comando_e c;
    int flag = 1, nr = 0;
    char filename[MAX_CHAR+1];
    corse * vet;
    while (flag) {
        c = leggicomando();
        switch (c) {
            case r_lettura:
                printf("Inserisci nome del file da leggere:\n");
                scanf(" %s", filename);
                free(vetCorse);
                vet = leggi_file(filename, &nr);
                return selezionaDati(vet, nr);
            case r_date:
            case r_codiceTratta:
            case r_partenza:
            case r_capolinea:
                multiordinamento(vetCorse, nCorse, c);
                break;
            case r_fine:
                flag = 0;
                break;
            case r_error:
                break;
            default:;
        }
    }
}

void strtolower(char s[MAX_CHAR + 1]){
    int i;
    for(i=0;s[i]!='\0';i++){
        s[i]=(char)tolower(s[i]);
    }
}

comando_e leggicomando(void){
    comando_e c;
    char comando[MAX_CHAR + 1];
    printf("\nScegli comando:\n");
    scanf(" %s", comando);
    strtolower(comando);
    if (strcmp(comando, "date") == 0) c = r_date;
    else if (strcmp(comando, "lettura") == 0) c = r_lettura;
    else if (strcmp(comando, "tratta") == 0) c = r_codiceTratta;
    else if (strcmp(comando, "partenza") == 0) c = r_partenza;
    else if (strcmp(comando, "capolinea") == 0) c = r_capolinea;
    else if (strcmp(comando, "fine") == 0) c = r_fine;
    else {
        printf("Comando errato!\n");
        c = r_error;
    }
    return c;
}

void multiordinamento(corse *vetCorse, int nCorse, comando_e c) {
    int i, j;
    corse *p_tratta[nCorse], *p_partenza[nCorse], *p_capolinea[nCorse], *p_date[nCorse], *key;
    if(c == r_codiceTratta) {
        for (i = 0; i < nCorse; i++)
            p_tratta[i] = &(vetCorse[i]);
        for (i = 1; i < nCorse; i++) {
            key = p_tratta[i];
            j = i - 1;
            while (j >= 0 && strcmp(p_tratta[j]->codice_tratta, key->codice_tratta) > 0) {
                p_tratta[j + 1] = p_tratta[j];
                j = j - 1;
            }
            p_tratta[j + 1] = key;
        }
        for (i = 0; i < nCorse; i++) printf(" %s %s %s %s %s %s %d\n", p_tratta[i]->codice_tratta,p_tratta[i]->partenza,p_tratta[i]->destinazione,p_tratta[i]->data,p_tratta[i]->ora_partenza,p_tratta[i]->ora_arrivo,p_tratta[i]->ritardo);
    }
    else if(c == r_partenza) {
        for (i = 0; i < nCorse; i++)
            p_partenza[i] = &(vetCorse[i]);
        for (i = 1; i < nCorse; i++) {
            key = p_partenza[i];
            j = i - 1;
            while (j >= 0 && strcmp(p_partenza[j]->partenza, key->partenza) > 0) {
                p_partenza[j + 1] = p_partenza[j];
                j = j - 1;
            }
            p_partenza[j + 1] = key;
        }
        for (i = 0; i < nCorse; i++) printf(" %s %s %s %s %s %s %d\n", p_partenza[i]->codice_tratta,p_partenza[i]->partenza,p_partenza[i]->destinazione,p_partenza[i]->data,p_partenza[i]->ora_partenza,p_partenza[i]->ora_arrivo,p_partenza[i]->ritardo);
    }
    else if(c == r_capolinea) {
        for (i = 0; i < nCorse; i++)
            p_capolinea[i] = &(vetCorse[i]);
        for (i = 1; i < nCorse; i++) {
            key = p_capolinea[i];
            j = i - 1;
            while (j >= 0 && strcmp(p_capolinea[j]->destinazione, key->destinazione) > 0) {
                p_capolinea[j + 1] = p_capolinea[j];
                j = j - 1;
            }
            p_capolinea[j + 1] = key;
        }
        for (i = 0; i < nCorse; i++) printf(" %s %s %s %s %s %s %d\n", p_capolinea[i]->codice_tratta,p_capolinea[i]->partenza,p_capolinea[i]->destinazione,p_capolinea[i]->data,p_capolinea[i]->ora_partenza,p_capolinea[i]->ora_arrivo,p_capolinea[i]->ritardo);
    }
    else if(c == r_date) {
        for (i = 0; i < nCorse; i++)
            p_date[i] = &(vetCorse[i]);
        for (i = 1; i < nCorse; i++) {
            key = p_date[i];
            j = i - 1;
            while (j >= 0 && strcmp(p_date[j]->data, key->data) > 0) {
                p_date[j + 1] = p_date[j];
                j = j - 1;
            }
            p_date[j + 1] = key;
        }
        for(i = 0; i >= 0 && i < nCorse-1; i++) {
            if (strcmp(p_date[i]->data, p_date[i+1]->data) == 0) {
                if(strcmp(p_date[i]->ora_partenza, p_date[i+1]->ora_partenza) > 0) {
                    key = p_date[i];
                    p_date[i] = p_date[i+1];
                    p_date[i+1] = key;
                    i--;
                }
            }
        }
        for (i = 0; i < nCorse; i++) printf(" %s %s %s %s %s %s %d\n", p_date[i]->codice_tratta,p_date[i]->partenza,p_date[i]->destinazione,p_date[i]->data,p_date[i]->ora_partenza,p_date[i]->ora_arrivo,p_date[i]->ritardo);
    }
}

corse * leggi_file(char filename[MAX_CHAR+1], int * nCorse) {
    int i;
    corse * vetCorse;
    FILE *fcorse = fopen(filename, "r");
    if (fcorse != NULL) {
        fscanf(fcorse, " %d", nCorse);
    }
    vetCorse = malloc((*nCorse) * sizeof(corse));
    for (i = 0; i < (*nCorse); i++) {
        fscanf(fcorse, " %s %s %s %s %s %s %d", vetCorse[i].codice_tratta, vetCorse[i].partenza, vetCorse[i].destinazione, vetCorse[i].data, vetCorse[i].ora_arrivo, vetCorse[i].ora_partenza, &vetCorse[i].ritardo);
    }
    fclose(fcorse);
    return vetCorse;
}