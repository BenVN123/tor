#ifndef __ENDPOINTS__
#define __ENDPOINTS__

/* POST REQUESTS */
#define RELAYUPDATE_PATH = "/tor/update/fp/xxxx"
#define RELAYUPDATE_PATH_LEN 15

/* GET REQUESTS */
#define CONSENSUS_PATH "/tor/status-vote/current/consensus"
#define CONSENSUS_PATH_LEN 34 
#define MICRODESC_PATH "/tor/status-vote/current/consensus-microdesc"
#define MICRODESC_PATH_LEN 44
#define RELAYDESC_PATH "/tor/server/fp/xxxx"
#define RELAYDESC_PATH_LEN 15
#define STATUSALL_PATH "/tor/status/all"
#define STATUSALL_PATH_LEN 15
#define STATUSTOR_PATH "/tor/status"
#define STATUSTOR_PATH_LEN 11

#include "handler.h"

void relayupdate(int sock, HTTPRequest *request);
void consensus(int sock, HTTPRequest *request);
void microdesc(int sock, HTTPRequest *request);
void relaydesc(int sock, HTTPRequest *request);
void statusall(int sock, HTTPRequest *request);
void statustor(int sock, HTTPRequest *request);

#endif
