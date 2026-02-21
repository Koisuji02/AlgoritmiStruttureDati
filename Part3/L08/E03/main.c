#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TIT 21
#define MAX_DATE 11

typedef struct quote_s {
    char date[MAX_DATE];
    double sum_val;
    int sum_qty;
    struct quote_s *left;
    struct quote_s *right;
} quote_t;

typedef struct titolo_s {
    char code[MAX_TIT];
    quote_t *root;
    struct titolo_s *next;
} titolo_t;

static titolo_t *list_insert_sorted(titolo_t *head, const char *code);
static titolo_t *list_search(titolo_t *head, const char *code);
static quote_t *bst_insert(quote_t *root, const char *date, double val, int qty);
static quote_t *bst_search(quote_t *root, const char *date);
static void bst_minmax_interval(quote_t *root, const char *d1, const char *d2, double *minv, double *maxv);
static void bst_minmax_all(quote_t *root, double *minv, double *maxv);
static int bst_height(quote_t *root);
static void bst_to_array(quote_t *root, quote_t **arr, int *idx);
static quote_t *array_to_bst(quote_t **arr, int l, int r);
static int bst_count(quote_t *root);
static void bst_free(quote_t *root);

int main(void) {
    titolo_t *head = NULL;
    titolo_t *current = NULL;
    int scelta = -1;

    while (scelta != 0) {
        printf("\n1) Leggi file\n2) Cerca titolo\n3) Quotazione per data\n4) Min/Max intervallo\n5) Min/Max globale\n6) Bilancia BST\n0) Fine\n");
        scanf(" %d", &scelta);

        if (scelta == 1) {
            char filename[256];
            int T;
            printf("Nome file: ");
            scanf(" %255s", filename);
            FILE *fin = fopen(filename, "r");
            if (fin == NULL) continue;
            fscanf(fin, " %d", &T);
            for (int i = 0; i < T; i++) {
                char code[MAX_TIT];
                int N;
                fscanf(fin, " %s %d", code, &N);
                titolo_t *tit = list_search(head, code);
                if (tit == NULL) head = list_insert_sorted(head, code);
                tit = list_search(head, code);
                for (int j = 0; j < N; j++) {
                    char date[MAX_DATE], time[6];
                    double val;
                    int qty;
                    fscanf(fin, " %10s %5s %lf %d", date, time, &val, &qty);
                    tit->root = bst_insert(tit->root, date, val, qty);
                }
            }
            fclose(fin);
            printf("File acquisito.\n");
        } else if (scelta == 2) {
            char code[MAX_TIT];
            printf("Codice titolo: ");
            scanf(" %20s", code);
            current = list_search(head, code);
            if (current == NULL) printf("Titolo non trovato.\n");
            else printf("Titolo selezionato: %s\n", current->code);
        } else if (scelta == 3) {
            if (current == NULL) { printf("Seleziona un titolo.\n"); continue; }
            char date[MAX_DATE];
            printf("Data (aaaa/mm/gg): ");
            scanf(" %10s", date);
            quote_t *q = bst_search(current->root, date);
            if (q == NULL) printf("Quotazione non trovata.\n");
            else printf("Quotazione: %.4f\n", q->sum_val / q->sum_qty);
        } else if (scelta == 4) {
            if (current == NULL) { printf("Seleziona un titolo.\n"); continue; }
            char d1[MAX_DATE], d2[MAX_DATE];
            double minv = 1e18, maxv = -1e18;
            printf("Intervallo date (aaaa/mm/gg aaaa/mm/gg): ");
            scanf(" %10s %10s", d1, d2);
            bst_minmax_interval(current->root, d1, d2, &minv, &maxv);
            if (minv > maxv) printf("Nessuna quotazione nell'intervallo.\n");
            else printf("Min: %.4f Max: %.4f\n", minv, maxv);
        } else if (scelta == 5) {
            if (current == NULL) { printf("Seleziona un titolo.\n"); continue; }
            double minv = 1e18, maxv = -1e18;
            bst_minmax_all(current->root, &minv, &maxv);
            if (minv > maxv) printf("Nessuna quotazione.\n");
            else printf("Min: %.4f Max: %.4f\n", minv, maxv);
        } else if (scelta == 6) {
            if (current == NULL) { printf("Seleziona un titolo.\n"); continue; }
            int h = bst_height(current->root);
            int n = bst_count(current->root);
            int min_h = 0;
            int tmp = n;
            while (tmp > 0) { min_h++; tmp >>= 1; }
            if (min_h == 0) min_h = 1;
            printf("Soglia S: ");
            double S; scanf(" %lf", &S);
            if ((double)h / (double)min_h > S) {
                quote_t **arr = malloc(n * sizeof(quote_t *));
                int idx = 0;
                bst_to_array(current->root, arr, &idx);
                current->root = array_to_bst(arr, 0, n - 1);
                free(arr);
                printf("BST bilanciato.\n");
            } else {
                printf("Bilanciamento non necessario.\n");
            }
        }
    }

    while (head != NULL) {
        titolo_t *tmp = head->next;
        bst_free(head->root);
        free(head);
        head = tmp;
    }
    return 0;
}

static titolo_t *list_insert_sorted(titolo_t *head, const char *code) {
    titolo_t *node = malloc(sizeof(titolo_t));
    strcpy(node->code, code);
    node->root = NULL;
    node->next = NULL;

    if (head == NULL || strcmp(head->code, code) > 0) {
        node->next = head;
        return node;
    }

    titolo_t *p = head;
    while (p->next != NULL && strcmp(p->next->code, code) < 0) p = p->next;
    node->next = p->next;
    p->next = node;
    return head;
}

static titolo_t *list_search(titolo_t *head, const char *code) {
    for (; head != NULL; head = head->next) {
        if (strcmp(head->code, code) == 0) return head;
    }
    return NULL;
}

static quote_t *bst_insert(quote_t *root, const char *date, double val, int qty) {
    if (root == NULL) {
        quote_t *node = malloc(sizeof(quote_t));
        strcpy(node->date, date);
        node->sum_val = val * qty;
        node->sum_qty = qty;
        node->left = node->right = NULL;
        return node;
    }
    int cmp = strcmp(date, root->date);
    if (cmp == 0) {
        root->sum_val += val * qty;
        root->sum_qty += qty;
    } else if (cmp < 0) {
        root->left = bst_insert(root->left, date, val, qty);
    } else {
        root->right = bst_insert(root->right, date, val, qty);
    }
    return root;
}

static quote_t *bst_search(quote_t *root, const char *date) {
    if (root == NULL) return NULL;
    int cmp = strcmp(date, root->date);
    if (cmp == 0) return root;
    if (cmp < 0) return bst_search(root->left, date);
    return bst_search(root->right, date);
}

static void bst_minmax_interval(quote_t *root, const char *d1, const char *d2, double *minv, double *maxv) {
    if (root == NULL) return;
    if (strcmp(root->date, d1) > 0) bst_minmax_interval(root->left, d1, d2, minv, maxv);
    if (strcmp(root->date, d1) >= 0 && strcmp(root->date, d2) <= 0) {
        double v = root->sum_val / root->sum_qty;
        if (v < *minv) *minv = v;
        if (v > *maxv) *maxv = v;
    }
    if (strcmp(root->date, d2) < 0) bst_minmax_interval(root->right, d1, d2, minv, maxv);
}

static void bst_minmax_all(quote_t *root, double *minv, double *maxv) {
    if (root == NULL) return;
    bst_minmax_all(root->left, minv, maxv);
    double v = root->sum_val / root->sum_qty;
    if (v < *minv) *minv = v;
    if (v > *maxv) *maxv = v;
    bst_minmax_all(root->right, minv, maxv);
}

static int bst_height(quote_t *root) {
    if (root == NULL) return 0;
    int hl = bst_height(root->left);
    int hr = bst_height(root->right);
    return (hl > hr ? hl : hr) + 1;
}

static int bst_count(quote_t *root) {
    if (root == NULL) return 0;
    return 1 + bst_count(root->left) + bst_count(root->right);
}

static void bst_to_array(quote_t *root, quote_t **arr, int *idx) {
    if (root == NULL) return;
    bst_to_array(root->left, arr, idx);
    arr[(*idx)++] = root;
    bst_to_array(root->right, arr, idx);
}

static quote_t *array_to_bst(quote_t **arr, int l, int r) {
    if (l > r) return NULL;
    int m = (l + r) / 2;
    quote_t *root = arr[m];
    root->left = array_to_bst(arr, l, m - 1);
    root->right = array_to_bst(arr, m + 1, r);
    return root;
}

static void bst_free(quote_t *root) {
    if (root == NULL) return;
    bst_free(root->left);
    bst_free(root->right);
    free(root);
}
