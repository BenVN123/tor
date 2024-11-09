#include "rsa.h"

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned int is_prime(uint64_t n);
unsigned long random_prime(void);
unsigned int gcd(uint64_t p, uint64_t q);
uint64_t extended_euclidean(uint64_t a, uint64_t b);
void find_exponents(uint64_t totient, uint64_t *public, uint64_t *private);
void save_key(const char *filename, uint64_t exp, uint64_t n);

void generate_new_rsa_key(void) {
    uint32_t p = random_prime();
    uint32_t q = random_prime();
    uint64_t n = p * q;
    uint64_t totient = (p - 1) * (q - 1);

    uint64_t public_exp;
    uint64_t private_exp;
    find_exponents(totient, &public_exp, &private_exp);

    save_key("node_rsa_keys/id_rsa", private_exp, n);
    save_key("node_rsa_keys/id_rsa.pub", public_exp, n);
}

void rsa_convert_data(uint8_t *dest, uint64_t *data, size_t size, uint64_t exp,
                      uint64_t n) {
    for (unsigned int i = 0; i < size; ++i) {
        dest[i] = (uint8_t)((uint64_t)pow((uint64_t)data[i], exp) % n);
    }
}

unsigned int is_prime(uint64_t n) {
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

unsigned int gcd(uint64_t p, uint64_t q) {
    uint64_t a, b, mod;
    if (p > q) {
        a = p;
        b = q;
    } else {
        a = q;
        b = p;
    }

    while (b > 0) {
        mod = a % b;
        a = b;
        b = mod;
    }

    return a;
}

uint64_t extended_euclidean(uint64_t a, uint64_t b) {
    int64_t old_r = a;
    int64_t r = b;
    int64_t old_s = 1;
    int64_t s = 0;
    int64_t old_t = 0;
    int64_t t = 1;
    int64_t quotient;
    int64_t tmp;

    while (r != 0) {
        quotient = old_r / r;
        tmp = old_r;
        old_r = r;
        r = tmp - quotient * r;
        tmp = old_r;
        old_s = s;
        s = tmp - quotient * s;
        tmp = old_t;
        t = old_t - quotient * t;
    }

    return (uint64_t)(s % b);
}

void find_exponents(uint64_t totient, uint64_t *public, uint64_t *private) {
    *public = 65537;
    if (gcd(*public, totient) != 1) {
        *public = 17;
    }

    *private = extended_euclidean(*public, totient);
}

void save_key(const char *filename, uint64_t exp, uint64_t n) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("could not save rsa key");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%llu\n%llu\n", exp, n);
    fclose(file);
}
