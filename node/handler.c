#include "handler.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "cryptography/rsa.h"
#include "instructions.h"

int receive_client_rsa(int sock, char *buffer, public_key_class *pub);
int send_node_rsa(int sock, public_key_class *pub, private_key_class *priv);

void handle_connection(int sock) {
    char char_buffer[512];
    uint8_t *byte_buffer = (uint8_t *)char_buffer;
    public_key_class *node_pub;
    private_key_class *node_priv;
    public_key_class *prev_pub;

    if (!receive_client_rsa(sock, char_buffer, prev_pub)) {
        return;
    }

    if (!send_node_rsa(sock, node_pub, node_priv)) {
        return;
    }

    listen_instructions(sock, byte_buffer, node_pub, node_priv, prev_pub);
}

int receive_client_rsa(int sock, char *buffer, public_key_class *pub) {
    ssize_t bytes_received = recv(sock, buffer, sizeof(buffer), 0);
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

    pub->exponent = strtoull(line1, NULL, 10);
    pub->modulus = strtoull(line2, NULL, 10);

    if (pub->exponent == 0 || pub->modulus == 0) {
        perror("client rsa public exponent or modulus not valid\n");
        return 0;
    }

    return 1;
}

int send_node_rsa(int sock, public_key_class *pub, private_key_class *priv) {
    pub = malloc(sizeof(public_key_class));
    priv = malloc(sizeof(private_key_class));
    rsa_gen_keys(pub, priv, PRIME_SOURCE_FILE);

    char message[50];
    snprintf(message, 25, "%lld\n%lld\n", pub->exponent, pub->modulus);
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
