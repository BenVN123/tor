#include "instructions.h"

#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "cells.h"
#include "circuits.h"
#include "cryptography/rsa.h"

void handle_create_cell(int sock, ControlCell *control_cell,
                        CircuitListNode *circuit_list,
                        public_key_class *node_pub,
                        private_key_class *node_priv,
                        public_key_class *prev_pub);

void listen_instructions(int sock, uint8_t *cell, public_key_class *node_pub,
                         private_key_class *node_priv,
                         public_key_class *prev_pub) {
    CircuitListNode *circuit_list = new_circuit_list();

    while (1) {
        ssize_t bytes_received = recv(sock, cell, sizeof(cell), 0);

        if (is_relay_cell(cell)) {
            RelayCell *relay_cell = new_relay_cell(cell);
            free(relay_cell);
        } else {
            ControlCell *control_cell = new_control_cell(cell);

            switch (control_cell->cmd) {
                case PADDING:
                    break;
                case CREATE:
                    handle_create_cell(sock, control_cell, circuit_list,
                                       node_pub, node_priv, prev_pub);
                    break;
                case DESTROY:
                    break;
                default:
                    break;
            }

            free(control_cell);
        }
    }
}

void handle_create_cell(int sock, ControlCell *control_cell,
                        CircuitListNode *circuit_list,
                        public_key_class *node_pub,
                        private_key_class *node_priv,
                        public_key_class *prev_pub) {
    if (retrieve_circuit(circuit_list, control_cell->circ_id) == NULL) {
        Circuit *circuit = malloc(sizeof(Circuit));
        circuit->circ_id = control_cell->circ_id;
        circuit->adj_circ = NULL;
        circuit->addr_len = sizeof(circuit->address);

        if (getpeername(sock, (struct sockaddr *)&(circuit->address),
                        &(circuit->addr_len)) == -1) {
            perror("failed to get peer name for CREATE cell\n");
            free(circuit);
            return;
        }

        char *ip = malloc(sizeof(char) * INET_ADDRSTRLEN);
        if (inet_ntop(AF_INET, &(circuit->address.sin_addr), ip, sizeof(ip)) ==
            NULL) {
            perror("inet_ntop failed for CREATE cell\n");
            free(circuit);
            free(ip);
            return;
        }
        free(ip);
        add_circuit(circuit_list, circuit);

        long long *encrypted = malloc(sizeof(long long) * 63);
        for (int i = 0; i < 63; ++i) {
            encrypted[i] = 0;
            for (int j = 0; j < 8; ++j) {
                encrypted[i] =
                    (encrypted[i] << 8u) & control_cell->data[(i * 8) + j];
            }
        }

        char *decrypted = rsa_decrypt(
    } else {
        perror(
            "previous node chose a circID that already exists for CREATE cell, "
            "which shouldn't be possible\n");
        exit(EXIT_FAILURE);
    }
}
