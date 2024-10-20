#ifndef __ENDPOINTS__
#define __ENDPOINTS__

/* POST REQUESTS */
#define RELAYUPDATE_PATH = "/tor/update/fp/xxxx"
#define RELAYUPDATE_PATH_LEN 15
#define NEWRELAY_PATH "/tor/register"
#define NEWRELAY_PATH_LEN 13

/* GET REQUESTS */
#define STATUSALL_PATH "/tor/status/all"
#define STATUSALL_PATH_LEN 15

#include "handler.h"

void relayupdate(int sock, HTTPRequest *request);
void newrelay(int sock, HTTPRequest *request);
void statusall(int sock, HTTPRequest *request);

#endif
