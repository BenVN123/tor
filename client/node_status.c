#include "node_status.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "cJSON.h"
#include "picohttpparser.h"

int send_request_statusall(void);
void parse_response(int sock, HTTPResponse *response);
void update_nodes_json(const char *msg, int msg_len);

void update_node_statuses(void) {
    HTTPResponse response;

    int sock = send_request_statusall();
    parse_response(sock, &response);
    update_nodes_json(response.msg, response.msg_len);

    close(sock);
}

void update_nodes_json(const char *msg, int msg_len) {
    cJSON *json = cJSON_ParseWithLength(msg, msg_len);

    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            perror(error_ptr);
            exit(EXIT_FAILURE);
        }
    }

    char *json_string = cJSON_PrintUnformatted(json);
    FILE *fp = fopen("nodes.json", "w");
    if (fp != NULL) {
        fputs(json_string, fp);
        fclose(fp);
    } else {
        perror("uhhhhhh file error?");
        exit(EXIT_FAILURE);
    }

    cJSON_free(json_string);
    cJSON_Delete(json);
}

int send_request_statusall(void) {
    int sock;
    long valread;
    struct sockaddr_in serv_addr;
    char *request = "GET" STATUSALL_PATH "HTTP/1.1\n";

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket creation error :O\n");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(DA_PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("invalid directory server address :(\n");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection failed :b\n");
        exit(EXIT_FAILURE);
    }

    send(sock, request, strlen(request), 0);
    return sock;
}

void parse_response(int sock, HTTPResponse *response) {
    int pret;
    size_t buflen = 0;
    size_t prevbuflen = 0;
    ssize_t rret;

    while (1) {
        while ((rret = read(sock, response->buffer + buflen,
                            sizeof(response->buffer) - buflen)) == -1 &&
               errno == EINTR);

        if (rret <= 0) {
            perror("invalid response from directory server >:(");
            exit(EXIT_FAILURE);
        }

        prevbuflen = buflen;
        buflen += rret;
        response->num_headers =
            sizeof(response->headers) / sizeof(response->headers[0]);

        pret = phr_parse_response(
            response->buffer, buflen, &(response->minor_version),
            &(response->status), &(response->msg), &(response->msg_len),
            response->headers, &(response->num_headers), prevbuflen);

        if (pret > 0) {
            return;
        } else if (pret == -1) {
            perror("invalid response from directory server >:(");
            exit(EXIT_FAILURE);
        }

        if (pret == -2) {
            perror("WTF HAPPENNEDDDD :(((");
            exit(EXIT_FAILURE);
        }

        if (buflen == sizeof(response->buffer)) {
            perror("response is above average...");
            exit(EXIT_FAILURE);
        }
    }
}
