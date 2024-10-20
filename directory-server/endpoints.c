#include "endpoints.h"

#include <assert.h>
#include <stdio.h>

#include "cJSON.h"
#include "handler.h"

cJSON *parse_relay_file(void);

void relayupdate(int sock, HTTPRequest *request) {}
void newrelay(int sock, HTTPRequest *request) {}

void statusall(int sock, HTTPRequest *request) {
    cJSON *json = parse_relay_file();

    char *json_string = cJSON_PrintUnformatted(json);
    send_response(sock, 200, json_string, strlen(json_string));

    cJSON_Free(json_string);
    cJSON_Delete(json);
}

cJSON *parse_relay_file(void) {
    FILE *fp = fopen("nodes.json", "r");
    assert(fp != NULL);

    char buffer[20000];
    int _ = fread(buffer, 1, sizeof(buffer), fp);
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
