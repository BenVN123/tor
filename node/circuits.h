#ifndef __CIRCUITS__
#define __CIRCUITS__

#include <stdint.h>
#include <netinet/in.h>

typedef struct Circuit {
    uint16_t circ_id;
    struct sockaddr_in address;
    socklen_t addr_len;
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
