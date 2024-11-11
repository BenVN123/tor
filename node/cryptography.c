#include "cryptography.h"

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// TODO: implement AES encryption, diffie-hellman shared key hash, and fix issue
// with uint32/64_t datatypes :(

int is_prime(uint64_t n);
uint32_t random_prime(void);
int gcd(uint64_t p, uint64_t q);
uint64_t extended_euclidean(uint64_t a, uint64_t b);
void find_exponents(uint64_t totient, uint64_t *public, uint64_t *private);
void prime_factors(uint32_t n, uint32_t *factors, size_t *size);
int is_primitive_root(uint32_t p, uint32_t g, uint32_t *factors, size_t f_size);
uint32_t generate_primitive_root(uint32_t prime);
uint32_t random_32bit_int(void);

void generate_rsa_key(uint64_t *private_exp, uint64_t *public_exp,
                      uint64_t *modulus) {
    uint32_t p = random_prime();
    uint32_t q = random_prime();
    *modulus = p * q;
    uint64_t totient = (p - 1) * (q - 1);

    find_exponents(totient, public_exp, private_exp);
}

void rsa_convert_data(uint8_t *dest, uint64_t *data, size_t size, uint64_t exp,
                      uint64_t n) {
    for (unsigned int i = 0; i < size; ++i) {
        dest[i] = (uint8_t)((uint64_t)pow((uint64_t)data[i], exp) % n);
    }
}

void generate_dh_params(uint32_t *prime, uint32_t *root) {
    *prime = random_prime();
    *root = generate_primitive_root(*prime);
}

void generate_dh_keys(uint32_t prime, uint32_t root, uint32_t *public_key,
                      uint32_t *private_key) {
    *private_key = random_32bit_int();
    *public_key = pow(root, private_key) % prime;
}

uint32_t compute_shared_dh_secret(uint32_t private_key, uint32_t public_key,
                                  uint32_t prime) {
    return pow(public_key, private_key) % prime;
}

void prime_factors(uint32_t n, uint32_t *factors, size_t *size) {
    uint32_t max_size = 10;
    factors = malloc(sizeof(uint32_t) * max_size);
    *size = 0;

    uint32_t d = 2;
    while (d * d <= n) {
        while (n % d == 0) {
            factors[*size] = d;
            n /= d;
            ++(*size);

            if (*size == max_size) {
                max_size += 10;
                factors = realloc(factors, sizeof(uint32_t) * max_size);
            }
        }
        ++d;
    }
    if (n > 1) {
        factors[*size] = n;
    }
}

uint32_t generate_primitive_root(uint32_t prime) {
    uint32_t *factors;
    size_t size;
    prime_factors(p - 1, factors, &size);

    for (uint32_t i = 0; i < prime; ++i) {
        if (is_primitive_root(prime, i, factors, size)) {
            free(factors);
            return i;
        }
    }

    free(factors);
    perror("something happened, this prime has no primitive root...\n");
    exit(EXIT_FAILURE);
}

int is_primitive_root(uint32_t p, uint32_t g, uint32_t *factors size_t f_size) {
    for (int i = 0; i < f_size; ++i) {
        if (pow(g, (p - 1) / factors[i]) % p == 1) {
            return 0;
        }
    }

    return 1;
}

int is_prime(uint32_t n) {
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

uint32_t random_32bit_int(void) { return ((uint32_t)rand() << 16u) | rand(); }

uint32_t random_prime(void) {
    uint32_t n;
    do {
        n = random_32bit_int();
    } while (!is_prime(n));
    return n;
}

int gcd(uint64_t p, uint64_t q) {
    uint64_t a, b, temp;

    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
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
