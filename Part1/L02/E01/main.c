#include <stdio.h>

int gcd(int a, int b);

int main() {
    int a, b, max_div;
    printf("Inserisci i due numeri: "); scanf("%d %d", &a, &b);
    max_div = gcd(a,b);
    printf("GCD: %d\n", max_div);
    return 0;
}

int gcd(int a, int b) {
    int tmp;
    if (a < b) {
        tmp = a;
        a = b;
        b = tmp;
    }
    if (b == 0) return a;
    return gcd(b, a % b);
}
