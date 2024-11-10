#include "handler.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "cell_utils.h"
#include "rsa.h"

int receive_client_rsa(int sock, char *buffer, uint64_t *public_exp,
                       uint64_t *modulus);
int send_node_rsa(int sock, uint64_t *private_exp, uint64_t *public_exp,
                  uint64_t *modulus);

void handle_connection(int sock) {
    char char_buffer[512];
    uint8_t *byte_buffer = (uint8_t *)char_buffer;
    uint64_t prev_node_public_exp;
    uint64_t prev_node_modulus;
    uint64_t private_exp_for_prev_node;
    uint64_t public_exp_for_prev_node;
    uint64_t modulus_for_prev_node;

    if (!receive_client_rsa(sock, char_buffer, &prev_node_public_exp,
                            &prev_node_modulus)) {
        return;
    }

    if (!send_node_rsa(sock, &private_exp_for_prev_node,
                       &public_exp_for_prev_node, &modulus_for_prev_node)) {
        return;
    }

    // TODO: create a cell listener that will read incoming cells from this
    // specific IP, figure out next node based on CircID, create new CircID, or
    // drop if invalid CircID and not a CREATE command.
}

int receive_client_rsa(int sock, char *buffer, uint64_t *public_exp,
                       uint64_t *modulus) {
    ssize_t bytes_received = read(sock, buffer, sizeof(buffer));
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
    } else if (bytes_received == 0) {
        perror("no rsa received\n");
        return 0;
    } else {
        perror("read failure\n");
        exit(EXIT_FAILURE);
    }

    char *line1 = strtok(buffer, "\n");
    if (line1 == NULL) {
        perror("missing client rsa public expoenent\n");
        return 0;
    }

    char *line2 = strtok(NULL, "\n");
    if (line2 == NULL) {
        perror("missing client rsa modulus\n");
        return 0;
    }

    *public_exp = strtoull(line1, NULL, 10);
    *modulus = strtoull(line2, NULL, 10);

    if (*public_exp == 0 || *modulus == 0) {
        perror("client rsa public exponent or modulus not valid\n");
        return 0;
    }

    return 1;
}

int send_node_rsa(int sock, uint64_t *private_exp, uint64_t *public_exp,
                  uint64_t *modulus) {
    generate_new_rsa_key(private_exp, public_exp, modulus);
    char message[50];
    snprintf(message, 25, "%llu\n%llu\n", *public_exp, *modulus);
    ssize_t bytes_sent = send(sock, message, strlen(message), 0);
    if (bytes_sent > 0) {
        return 1;
    } else if (bytes_sent == 0) {
        perror("could not send rsa\n");
        return 0;
    } else {
        perror("smt went wrong sending rsa key....\n");
        exit(EXIT_FAILURE);
    }
}
