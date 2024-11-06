#ifndef __NODE_STATUS__
#define __NODE_STATUS__

#define DIRECTORY_SERVER_IP "1.2.3.4"
#define DIRECTORY_SERVER_PORT 1234

#define STATUSALL_PATH "/tor/status/all"
#define STATUSALL_PATH_LEN 15 

#define DA_PORT 8080

#include "picohttpparser.h"

typedef struct phr_header PHRHeader;
typedef struct HTTPResponse {
    char buffer[20000];
    int minor_version;
    int status;
    int num_headers;
    PHRHeader headers[100];
    char *msg;
    int msg_len;
} HTTPResponse;

void update_node_statuses(void);

#endif

