#ifndef __CIRCUITS__
#define __CIRCUITS__

#include <netinet/in.h>
#include <stdint.h>

#include "cryptography/ecdh.h"

typedef struct Circuit {
    uint16_t circ_id;
    struct sockaddr_in address;
    socklen_t addr_len;
    uint8_t public_key[ECC_PUB_KEY_SIZE];
    uint8_t private_key[ECC_PRV_KEY_SIZE];
    uint8_t shared_secret[ECC_PUB_KEY_SIZE];
    struct Circuit *adj_circ;
} Circuit;

typedef struct CircuitListNode {
    Circuit *circuit;
    struct CircuitListNode *next;
} CircuitListNode;

CircuitListNode *new_circuit_list(void);
void add_circuit(CircuitListNode *list, Circuit *circuit);
void remove_circuit(CircuitListNode *list, uint16_t circ_id);
Circuit *retrieve_circuit(CircuitListNode *list, uint16_t circ_id);
void destroy_circuit_list(CircuitListNode *list);

#endif
