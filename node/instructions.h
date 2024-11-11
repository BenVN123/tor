#ifndef __INSTRUCTIONS__
#define __INSTRUCTIONS__

#include <stdint.h>

void listen_instructions(int sock, uint8_t *buffer, uint64_t prev_node_public_exp, uint64_t prev_node_modulus, uint64_t private_exp_for_prev_node, uint64_t public_exp_for_prev_node, uint64_t modulus_for_prev_node);

#endif
