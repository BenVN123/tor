#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

int is_prime(int n);
unsigned long random_prime(void);

void generate_new_rsa_key(void) {
    uint32_t p = random_prime();
    uint32_t q = random_prime();
    uint64_t n = p * q;
    uint64_t totient = (p - 1) * (q - 1);
}

int is_prime(int n) {
    if (n <= 1) {
        return 0;
    }
    if (n == 2) {
        return 1;
    }
    if (n % 2 == 0) {
        return 0;
    }

    int limit = (int)sqrt(n);
    for (int i = 3; i <= limit; i += 2) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

unsigned long random_prime(void) {
    unsigned long long n;
    do {
        n = ((uint32_t)rand() << 16u) | rand();
    } while (!is_prime(n));
    return n;
}
