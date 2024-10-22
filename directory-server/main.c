#include "main.h"

#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "handler.h"
#include "queue.h"

sem_t sem_pop;

void *check_requests(void *arg) {
    ServerInfo *info = (ServerInfo *)arg;
    int new_socket;
    while (1) {
        if ((new_socket =
                 accept(info->server_fd, info->address, info->addrlen)) < 0) {
            perror("error on accept");
            exit(EXIT_FAILURE);
        }
        append(new_socket);
    }
}

void *worker(void *) {
    int next_socket;
    while (1) {
        // TODO: second semaphore for POST requests, since
        // multiple nodes might want to change JSON at the
        // same time and cause race condition
        sem_wait(&sem_pop);
        next_socket = pop();
        sem_post(&sem_pop);
        if (next_socket != -1) {
            handle_connection(next_socket);
            close(next_socket);
        }
    }
}

int main(void) {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("could not open socket!");
        return 0;
    }

    memset((char *)&address, 0, addrlen);
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_fd, (struct sockaddr *)&address, addrlen) < 0) {
        perror("could not bind!");
        return 0;
    }

    if (listen(server_fd, 20) < 0) {
        perror("error on listen");
        exit(EXIT_FAILURE);
    }

    sem_init(&sem_pop, 0, 1);

    pthread_t threads[MAX_THREADS];
    int err;

    ServerInfo *info = malloc(sizeof(ServerInfo));
    info->server_fd = server_fd;
    info->address = (struct sockaddr *)&address;
    info->addrlen = (socklen_t *)&addrlen;
    err = pthread_create(threads, NULL, check_requests, info);
    if (err) {
        perror("thread errorrrrrr");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < MAX_THREADS; i++) {
        err = pthread_create(&threads[i], NULL, worker, NULL);
        if (err) {
            perror("thread errorrrrrr");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&sem_pop);
    free(info);

    return 0;
}
