#ifndef __RSA__
#define __RSA__

#include <stdint.h>
#include <stdio.h>

void generate_new_rsa_key(uint64_t *private_exp, uint64_t *public_exp,
                          uint64_t *modulus);
void rsa_convert_data(uint8_t *dest, uint64_t *data, size_t size, uint64_t exp,
                      uint64_t n);

#endif
