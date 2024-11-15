#ifndef __INSTRUCTIONS__
#define __INSTRUCTIONS__

#include <stdint.h>

#include "cryptography/rsa.h"

void listen_instructions(int sock, uint8_t *buffer, public_key_class *node_pub,
                         private_key_class *node_priv,
                         public_key_class *prev_pub);

#endif
