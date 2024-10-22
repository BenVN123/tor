#include "endpoints.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "http_utils.h"

cJSON *parse_relay_file(int sock);

void relayupdate(int sock, HTTPRequest *request) {}
void newrelay(int sock, HTTPRequest *request) {}

void statusall(int sock, HTTPRequest *request) {
    cJSON *json = parse_relay_file(sock);

    char *json_string = cJSON_PrintUnformatted(json);
    send_response(sock, 200, json_string, strlen(json_string));

    cJSON_free(json_string);
    cJSON_Delete(json);

    printf("node json succcessfully sent :D\n");
}

cJSON *parse_relay_file(int sock) {
    FILE *fp = fopen("nodes.json", "r");
    assert(fp != NULL);

    char buffer[20000];
    fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            send_response(sock, 500, "something went wrong on our end :(", 34);
            exit(EXIT_FAILURE);
        }
    }

    return json;
}
