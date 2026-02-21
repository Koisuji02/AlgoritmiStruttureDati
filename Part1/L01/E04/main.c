#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_CORSE 1000
#define MAX_CHAR 30

typedef enum comando{r_date, r_stampa,r_codiceTratta, r_partenza, r_capolinea, r_ricerca, r_fine, r_error, r_multisorting}comando_e;

typedef struct{
    char codice_tratta[MAX_CHAR + 1];
    char partenza [MAX_CHAR + 1];
    char destinazione[MAX_CHAR + 1];
    char data[MAX_CHAR + 1];
    char ora_partenza[MAX_CHAR + 1];
    char ora_arrivo[MAX_CHAR + 1];
    int ritardo;
} corse;

void selezionaDati(corse vetCorse[MAX_CORSE],int nCorse);
comando_e leggicomando(void);
void strtolower(char s[MAX_CHAR]);
void stampa(corse vetCorse[MAX_CORSE], int nCorse);
void insertion_sort(corse vetCorse[MAX_CORSE],int nCorse, comando_e c);
void multiordinamento(corse vetCorse[MAX_CORSE], int nCorse, comando_e c);
void ricerca(corse vetCorse[MAX_CORSE], int nCorse);
int ricerca_dicotomica(corse vetCorse[MAX_CORSE], int inizio, int nCorse, char str_input[MAX_CHAR +1]);


int main(void) {
    int nCorse = 0, i;
    corse vetCorse[MAX_CORSE];
    FILE *fcorse = fopen("corse.txt", "r");
    if (fcorse != NULL) {
        fscanf(fcorse, " %d", &nCorse);
    }
    for (i = 0; i < nCorse; i++) {
        fscanf(fcorse, " %s %s %s %s %s %s %d", vetCorse[i].codice_tratta, vetCorse[i].partenza, vetCorse[i].destinazione, vetCorse[i].data, vetCorse[i].ora_partenza, vetCorse[i].ora_arrivo, &vetCorse[i].ritardo);
    }
    selezionaDati(vetCorse, nCorse);
    fclose(fcorse);
    return 0;

}

void selezionaDati(corse vetCorse[MAX_CORSE],int nCorse) {
    comando_e c;
    char parola[MAX_CHAR+1];
    int flag = 1;
    while (flag) {
    	c = leggicomando();
        switch (c) {
            case r_stampa:
                stampa(vetCorse, nCorse);
                break;
            case r_date:
            case r_codiceTratta:
            case r_partenza:
            case r_capolinea:
                insertion_sort(vetCorse, nCorse, c);
                break;
            case r_multisorting:
            	while(flag) {
            		printf("\nScegli in base a cosa ordinare:\n");
            		scanf(" %s", parola);
            		if (strcmp(parola, "date") == 0) c = r_date;
    				else if (strcmp(parola, "stampa") == 0) c = r_stampa;
    				else if (strcmp(parola, "tratta") == 0) c = r_codiceTratta;
    				else if (strcmp(parola, "partenza") == 0) c = r_partenza;
    				else if (strcmp(parola, "capolinea") == 0) c = r_capolinea;
    				else if (strcmp(parola, "fine") == 0) break;
    				else {
    					printf("Errore!");
					}
            		multiordinamento(vetCorse, nCorse, c);
            	}
            	break;
            case r_ricerca:
                ricerca(vetCorse, nCorse);
                break;
            case r_fine:
                flag = 0;
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
    else if (strcmp(comando, "stampa") == 0) c = r_stampa;
    else if (strcmp(comando, "tratta") == 0) c = r_codiceTratta;
    else if (strcmp(comando, "partenza") == 0) c = r_partenza;
    else if (strcmp(comando, "capolinea") == 0) c = r_capolinea;
    else if (strcmp(comando, "ricerca") == 0) c = r_ricerca;
    else if (strcmp(comando, "fine") == 0) c = r_fine;
    else if (strcmp(comando, "multiordinamento") == 0) c = r_multisorting;
    else {
        printf("Comando errato!\n");
        c = r_error;
    }
    return c;
}

void stampa(corse vetCorse[MAX_CORSE], int nCorse) {
    comando_e c;
    char comando[MAX_CHAR+1], inizio[MAX_CHAR+1], fine[MAX_CHAR+1], codiceTratta[MAX_CHAR+1], fermata[MAX_CHAR+1];
    int i, counter = 0;
    printf("Scegli in base a cosa stampare: ");
    scanf(" %s", comando);
    strtolower(comando);
    if (strcmp(comando, "date") == 0) c = r_date;
    else if (strcmp(comando, "tratta") == 0) c = r_codiceTratta;
    else if (strcmp(comando, "partenza") == 0) c = r_partenza;
    else if (strcmp(comando, "capolinea") == 0) c = r_capolinea;
    else c = r_error;
    switch (c) {

        case r_date:
            printf("Inserisci l'intervallo di date:\n");
            scanf(" %s %s", inizio, fine);
            for (i = 0; i < nCorse; i++) {
                if (strcmp(vetCorse[i].data, inizio) >= 0 && strcmp(vetCorse[i].data, fine) <= 0) {
                    printf(" %s %s %s %s %s %s %d\n", vetCorse[i].codice_tratta, vetCorse[i].partenza, vetCorse[i].destinazione, vetCorse[i].data, vetCorse[i].ora_arrivo, vetCorse[i].ora_partenza, vetCorse[i].ritardo);
                    counter++;
                }
            }
            if (counter == 0) printf("Non sono state trovate corse nell'intervallo indicato!\n");
            break;

        case r_codiceTratta:
            printf("Inserire il codice tratta:\n");
            scanf(" %s", codiceTratta);
            for (i = 0; i < strlen(codiceTratta); i++) {
                codiceTratta[i] = (char)toupper(codiceTratta[i]);
            }
            for (i = 0; i < nCorse; i++) {
                if (strcmp(codiceTratta, vetCorse[i].codice_tratta) == 0) {
                    printf(" %s %s %s %s %s %s %d\n", vetCorse[i].codice_tratta, vetCorse[i].partenza, vetCorse[i].destinazione, vetCorse[i].data, vetCorse[i].ora_arrivo, vetCorse[i].ora_partenza, vetCorse[i].ritardo);
                    counter ++;
                }
            }
            if (counter == 0) printf("Non sono state trovate corse con questo codice tratta!\n");
            break;

        case r_partenza:
            printf("Inserire fermata di partenza:\n");
            scanf(" %s", fermata);
            strtolower(fermata);
            for (i = 0; i < nCorse; i++) {
                strtolower(vetCorse[i].partenza);
                if (strcmp(fermata, vetCorse[i].partenza) == 0) {
                    printf(" %s %s %s %s %s %s %d\n", vetCorse[i].codice_tratta, vetCorse[i].partenza, vetCorse[i].destinazione, vetCorse[i].data, vetCorse[i].ora_arrivo, vetCorse[i].ora_partenza, vetCorse[i].ritardo);
                    counter ++;
                }
            }
            if (counter == 0) printf("Non ci sono corse che partono da %s!\n", fermata);
            break;

        case r_capolinea:
            printf("Inserire fermata di arrivo:\n");
            scanf(" %s", fermata);
            strtolower(fermata);
            for (i = 0; i < nCorse; i++) {
                strtolower(vetCorse[i].destinazione);
                if (strcmp(fermata, vetCorse[i].destinazione) == 0) {
                    printf(" %s %s %s %s %s %s %d\n", vetCorse[i].codice_tratta, vetCorse[i].partenza, vetCorse[i].destinazione, vetCorse[i].data, vetCorse[i].ora_arrivo, vetCorse[i].ora_partenza, vetCorse[i].ritardo);
                    counter ++;
                }
            }
            if (counter == 0) printf("Non ci sono corse che arrivano a %s!\n", fermata);
            break;

        case r_error:
            printf("Parametro di stampa errato. Errore!");
            break;

        default:;
    }
}

void insertion_sort(corse vetCorse[MAX_CORSE],int nCorse, comando_e c) {
    int i, j;
    corse key;
    for (i = 1; i < nCorse; i++) {
        key = vetCorse[i];
        j = i - 1;
        if(c == r_codiceTratta) {
            while (j >= 0 && strcmp(vetCorse[j].codice_tratta, key.codice_tratta) > 0) {
                vetCorse[j + 1] = vetCorse[j];
                j = j - 1;
            }
        }
        else if(c == r_partenza) {
            while (j >= 0 && strcmp(vetCorse[j].partenza, key.partenza) > 0) {
                vetCorse[j + 1] = vetCorse[j];
                j = j - 1;
            }
        }
        else if(c == r_capolinea) {
            while (j >= 0 && strcmp(vetCorse[j].destinazione, key.destinazione) > 0) {
                vetCorse[j + 1] = vetCorse[j];
                j = j - 1;
            }
        }
        else {
            while (j >= 0 && (strcmp(vetCorse[j].data, key.data) > 0 || (strcmp(vetCorse[j].data, key.data) == 0 && (strcmp(vetCorse[j].ora_partenza, key.ora_partenza) > 0)))) {
                vetCorse[j + 1] = vetCorse[j];
                j = j - 1;
            }
        }
        vetCorse[j + 1] = key;
    }
}

static void stampa_ordinato(corse *p[], int nCorse) {
    int i;
    for (i = 0; i < nCorse; i++) {
        printf(" %s %s %s %s %s %s %d\n", p[i]->codice_tratta, p[i]->partenza, p[i]->destinazione,
               p[i]->data, p[i]->ora_arrivo, p[i]->ora_partenza, p[i]->ritardo);
    }
}

void multiordinamento(corse vetCorse[MAX_CORSE],int nCorse, comando_e c) {
    int i, j;
    corse *p_tratta[MAX_CORSE], *p_partenza[MAX_CORSE], *p_capolinea[MAX_CORSE], *p_date[MAX_CORSE], *key;
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
        stampa_ordinato(p_tratta, nCorse);
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
        stampa_ordinato(p_partenza, nCorse);
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
        stampa_ordinato(p_capolinea, nCorse);
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
        stampa_ordinato(p_date, nCorse);
    }
}

void ricerca(corse vetCorse[MAX_CORSE], int nCorse) {
    char str_input[MAX_CHAR+1], scelta[MAX_CHAR+1];
    int i,j, counter = 0, inizio = 0, flag = 1;
    printf("Inserisci la stazione di partenza (anche parzialmente):\n");
    scanf(" %s", str_input);
    printf("Scegli tra ricerca lineare o dicotomica:\n");
    scanf(" %s", scelta);
    strtolower(scelta);
    if (strcmp(scelta, "lineare") == 0) {
        for (i = 0; i < nCorse; i++) {
            for (j = 0; j < strlen(str_input); j++) {
                if (tolower(str_input[j]) == tolower(vetCorse[i].partenza[j])) counter++;
            }
            if (counter == j) {
                printf("-> Trovata la corsa: %s %s %s %s %s %s %d\n", vetCorse[i].codice_tratta, vetCorse[i].partenza, vetCorse[i].destinazione, vetCorse[i].data, vetCorse[i].ora_arrivo, vetCorse[i].ora_partenza, vetCorse[i].ritardo);
                flag = 0;
            }
            counter = 0;
        }
        if (flag == 1) printf("Corsa non trovata!\n");
    }
    if (strcmp(scelta, "dicotomica") == 0) {
        insertion_sort(vetCorse, nCorse, r_partenza);
    	ricerca_dicotomica(vetCorse, inizio, nCorse, str_input);
    }
}

int ricerca_dicotomica(corse vetCorse[MAX_CORSE], int inizio, int nCorse, char str_input[MAX_CHAR +1]) {
	int m, j, i, counter = 0;
	if (inizio > nCorse) {
        printf("Corsa non trovata!\n");
        return -1;
    }
	m = (inizio + nCorse)/2;
	for (j = 0; j < strlen(str_input); j++) {
        if (tolower(str_input[j]) == tolower(vetCorse[m].partenza[j])) counter++;
        else if(tolower(str_input[j]) < tolower(vetCorse[m].partenza[j])) return ricerca_dicotomica(vetCorse, inizio, m-1, str_input);
        else return ricerca_dicotomica(vetCorse, m+1, nCorse, str_input);
    }
    if (counter == j) {
    	for (i = 0; i < nCorse; i++) {
    		if(strcmp(vetCorse[m].partenza,vetCorse[i].partenza) == 0) printf("-> Trovata la corsa: %s %s %s %s %s %s %d\n", vetCorse[i].codice_tratta, vetCorse[i].partenza,
               vetCorse[i].destinazione, vetCorse[i].data, vetCorse[i].ora_arrivo, vetCorse[i].ora_partenza,
               vetCorse[i].ritardo);
    	}
        return 0;
    }
}
