#ifndef __HANDLER__
#define __HANDLER__

#define MIN(x, y) (x < y ? x : y)

typedef struct HTTPRequest {
    char buffer[4096];
    int minor_version;
    int num_headers;
    struct phr_header headers[100];
    int method_len;
    char *method;
    int path_len;
    char *path;
} HTTPRequest;

void handle_connection(int sock);

#endif

