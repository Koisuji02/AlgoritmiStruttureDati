#include <stdio.h>

int gcd(int a, int b);

int main() {
    int a, b, max_div;
    printf("Inserisci i due numeri: "); scanf("%d %d", &a, &b);
    max_div = gcd(a,b);
    printf("GCD: %d\n", max_div);
}

int gcd(int a, int b) {
    int tmp;
    if (a < b) {
        tmp = a;
        a = b;
        b = tmp;
    }
    if ((a%b) == 0) return b;
    else if (((a % 2) == 0) && ((b % 2) == 0)) return (2*gcd((a/2), (b/2)));
    else if (((a % 2) != 0) && ((b % 2) == 0)) return (gcd(a, (b/2)));
    else if (((a % 2) == 0) && ((b % 2) != 0)) return (gcd((a/2), b));
    else if (((a % 2) != 0) && ((b % 2) != 0)) return (gcd(((a-b)/2), b));
}
