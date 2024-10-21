#include "handler.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void send_response(int sock, int error_code, const char *msg, int msg_len) {
    char response[15 + msg_len];
    snprintf(response, 15 + msg_len, "HTTP/1.1 %d\n\n%s", error_code, msg);
    write(sock, response, strlen(response);
}

void call_endpoint(int sock, HTTPRequest *request) {
    if (strncmp(request->method, "POST", MIN(request->method_len, 4)) == 0) {
        if (strncmp(request->path, RELAYUPDATE_PATH,
                    MIN(request->path_len, RELAYUPDATE_PATH_LEN)) == 0) {
        } else if (strncmp(request->path, NEWRELAY_PATH,
                           MIN(request->path_len, NEWRELAY_PATH_LEN)) == 0) {
        } else {
            send_response(sock, 404, "POST path " request->path " not valid",
                          20 + request->path_len);
        }
    } else if (strncmp(request->method, "GET", MIN(request->method_len, 3)) ==
               0) {
        if (strncmp(request->path, STATUSALL_PATH,
                    MIN(request->path_len, STATUSALL_PATH_LEN)) == 0) {
        } else {
            send_response(sock, 404, "GET path " request->path " not valid",
                          19 + request->path_len);
        }
    } else {
        send_response(sock, 405, request->method " method not valid",
                      17 + request->method_len);
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
            send_response(sock, 400, "invalid request >:(", 19);
            return -1;
        }

        prevbuflen = buflen;
        buflen += rret;
        request->num_headers =
            sizeof(request->headers) / sizeof(request->headers[0]);
        pret = phr_parse_request(&(request->buffer), buflen, &(request->method),
                                 &(request->method_len), request->path,
                                 &(request->path_len),
                                 &(request->minor_version), request->headers,
                                 &(request->num_headers), prevbuflen);

        if (pret > 0) {
            return 0;
        } else if (pret == -1) {
            send_response(sock, 400, "invalid request >:(", 19);
            return -1;
        }

        if (pret == -2) {
            send_response(sock, 500, "something went wrong on our end :(", 34);
            exit(EXIT_FAILURE);
        }

        if (buflen == sizeof(request->buffer)) {
            send_response(sock, 400, "request too looooonnnggggg", 26);
            return -1;
        }
    }
}
