#ifndef __CRYPTOGRAPHY__
#define __CRYPTOGRAPHY__

#include <stdint.h>
#include <stdio.h>

// RSA
void generate_rsa_key(uint64_t *private_exp, uint64_t *public_exp,
                      uint64_t *modulus);
void rsa_convert_data(uint8_t *dest, uint64_t *data, size_t size, uint64_t exp,
                      uint64_t n);

// Diffie-Hellman
void generate_dh_params(uint32_t *prime, uint32_t *root);
void generate_dh_keys(uint32_t prime, uint32_t root, uint32_t *public_key,
                      uint32_t *private_key);
uint32_t compute_shared_dh_secret(uint32_t private_key, uint32_t public_key,
                                  uint32_t, uint32_t prime);

#endif
