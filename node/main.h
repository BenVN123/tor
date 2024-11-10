#ifndef __MAIN__
#define __MAIN__

#define MAX_THREADS 4
#define PORT 8080

#include <sys/socket.h>

typedef struct ServerInfo {
    int server_fd;
    struct sockaddr *address;
    socklen_t *addrlen;
} ServerInfo;

void *check_requests(void *arg);
void *worker(void *);
int main(void);

#endif
