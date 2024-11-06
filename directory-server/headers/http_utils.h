#ifndef __HTTP_UTILS__
#define __HTTP_UTILS__

#include "picohttpparser.h"

typedef struct phr_header PHRHeader; // >:(
typedef struct HTTPRequest {
    char buffer[4096];
    int minor_version;
    int num_headers;
    PHRHeader headers[100];
    int method_len;
    char *method;
    int path_len;
    char *path;
} HTTPRequest;

void send_response(int sock, int error_code, const char *msg, int msg_len); 

#endif
