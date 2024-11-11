#include "circuits.h"

#include <stdlib.h>

CircuitListNode *new_circuit_list(void) {
    CircuitListNode *list = malloc(sizeof(CircuitListNode));
    list->circuit = NULL;
    list->next = list;
    return list;
}

void add_circuit(CircuitListNode *list, Circuit *circuit) {
    if (list->circuit == NULL) {
        list->circuit = circuit;
    } else {
        CircuitListNode *new_item = malloc(sizeof(CircuitListNode));
        new_item->next = list->next;
        new_item->circuit = circuit;
        list->next = new_item;
    }
}

void remove_circuit(CircuitListNode *list, uint16_t circ_id) {
    CircuitListNode *curr_node = list;
    CircuitListNode *target_node;

    do {
        target_node = curr_node->next;
        if (target_node->circuit->circ_id == circ_id) {
            curr_node->next = target_node->next;
            free(target_node->circuit);
            free(target_node);
            return;
        } else {
            curr_node = curr_node->next;
        }
    } while (curr_node != list);
}

Circuit *retrieve_circuit(CircuitListNode *list, uint16_t circ_id) {
    CircuitListNode *curr_node = list->next;

    while (curr_node != list) {
        if (curr_node->circuit->circ_id == circ_id) {
            return curr_node->circuit;
        }
    }

    return NULL;
}

void destroy_circuit_list(CircuitListNode *list) {
    CircuitListNode *curr = list;
    CircuitListNode *next = list->next;

    while (curr->circuit != NULL) {
        free(curr->circuit);
        free(curr);
        curr = next;
        next = curr->next;
    }
}
