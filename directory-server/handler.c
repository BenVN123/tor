#include "handler.h"

#include <errno.h>
#include <stdio.h>

#include "endpoints.h"
#include "picohttpparser.h"

void call_endpoint(int sock, HTTPRequest *request);
void parse_request(int sock, HTTPRequest *request);

void handle_connection(int sock) {
    HTTPRequest request;

    if (parse_request(sock, &request) != -1) {
        call_endpoint(sock, &request);
    }
}

void call_endpoint(int sock, HTTPRequest *request) {
    if (strncmp(request->method, "POST", MIN(request->method_len, 4)) == 0) {
        if (strncmp(request->path, RELAYUPDATE_PATH,
                    MIN(request->path_len, RELAYUPDATE_PATH_LEN)) == 0) {
        } else {
            perror("POST path %s not valid", request->path);
        }
    } else if (strncmp(request->method, "GET", MIN(request->method_len, 3)) ==
               0) {
        if (strncmp(request->path, CONSENSUS_PATH,
                    MIN(request->path_len, CONSENSUS_PATH_LEN)) == 0) {
        } else if (strncmp(request->path, MICRODESC_PATH,
                           MIN(request->path_len, MICRODESC_PATH_LEN)) == 0) {
        } else if (strncmp(request->path, RELAYDESC_PATH,
                           MIN(request->path_len, RELAYDESC_PATH_LEN)) == 0) {
        } else if (strncmp(request->path, STATUSALL_PATH,
                           MIN(request->path_len, STATUSALL_PATH_LEN)) == 0) {
        } else if (strncmp(request->path, STATUSTOR_PATH,
                           MIN(request->path_len, STATUSTOR_PATH_LEN)) == 0) {
        } else {
            perror("GET path %s not valid", request->path);
        }
    } else {
        perror("%s not an available method", request->method);
    }
}

int parse_request(int sock, HTTPRequest *request) {
    int pret;
    size_t buflen = 0;
    size_t prevbuflen = 0;
    ssize_t rret;

    while (1) {
        while ((rret = read(sock, request->buffer + buflen,
                            sizeof(request->buffer) - buflen)) == -1 &&
               errno == EINTR);

        if (rret <= 0) {
            perror("ioerror when parsing request\n");
            return -1;
        }

        prevbuflen = buflen;
        buflen += rret;
        request->num_headers =
            sizeof(request->headers) / sizeof(request->headers[0]);
        pret = phr_parse_request(
            request->buffer, buflen, request->method, &(request->method_len),
            request->path, &(request->path_len), &(request->minor_version),
            request->headers, &(request->num_headers), prevbuflen);

        if (pret > 0) {
            return 0;
        } else if (pret == -1) {
            perror("parserror when parsing request\n");
            return -1;
        }
        assert(pret == -2);

        if (buflen == sizeof(request->buffer)) {
            perror("request is too long\n");
            return -1;
        }
    }
}
