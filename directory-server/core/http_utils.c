#include "http_utils.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

void send_response(int sock, int error_code, const char *msg, int msg_len) {
    char response[15 + msg_len];
    snprintf(response, 15 + msg_len, "HTTP/1.1 %d\r\n\r\n%s\r\n\r\n",
             error_code, msg);
    write(sock, response, strlen(response));
}
