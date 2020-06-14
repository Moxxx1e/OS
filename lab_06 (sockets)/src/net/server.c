#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "info.h"

#define MAX_CLIENTS 10
int clients[MAX_CLIENTS] = { 0 };
#define SOCK_ERR -1
#define BIND_ERR -2
#define LISTEN_ERR -3
#define ACCEPT_ERR -4
#define SELECT_ERR -5
#define MAX_QUEUE_SIZE 3

int register_new_client(unsigned int fd)
{
    struct sockaddr_in client_addr;
    int addr_size = sizeof(client_addr);

    int new_sock = accept(fd, (struct sockaddr*)&client_addr, (socklen_t*)&addr_size);
    if (new_sock < 0) {
        printf("Error in accept.");
        return ACCEPT_ERR;
    }

    printf("\nNew client: \nfd = %d \nip = %s:%d\n", new_sock,
        inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] == 0) {
            clients[i] = new_sock;
            printf("Registered as client #%d\n", i);
            return 0;
        }
    }
}

void check_client(unsigned int fd, unsigned int client_id)
{
    char msg[MSG_LEN];
    memset(msg, 0, MSG_LEN);

    struct sockaddr_in client_addr;
    int addr_size = sizeof(client_addr);

    int recv_size = recv(fd, msg, MSG_LEN, 0);
    if (recv_size != 0) {
        msg[recv_size] = '\0';
        printf("Message from %d client: %s\n", client_id, msg);
    }
    else {
        getpeername(fd, (struct sockaddr*)&client_addr, (socklen_t*)&addr_size);
        printf("Client %d disconnected %s:%d \n", client_id, 
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        close(fd);
        clients[client_id] = 0;
    }
}

int main(void)
{
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Error in socket(). \n");
        return SOCK_ERR;
    }

    struct sockaddr_in client_addr;
    client_addr.sin_family = PF_INET;
    client_addr.sin_port = htons(SOCK_PORT);
    client_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*) &client_addr, sizeof(client_addr)) < 0) {
        printf("Error in bind.\n");
        return BIND_ERR;
    }

    printf("Server is running on the %d port.\n", SOCK_PORT);

    if (listen(sockfd, MAX_QUEUE_SIZE) < 0) {
        printf("Error in listen.\n");
        return LISTEN_ERR;
    }

    for (;;) {
        fd_set readfds;
        int max_fd;
        int active_clients_count;

        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        max_fd = sockfd;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            int fd = clients[i];

            if (fd > 0)
                FD_SET(fd, &readfds);

            max_fd = (fd > max_fd) ? (fd) : (max_fd);
        }

        active_clients_count = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if (active_clients_count < 0 && (errno != EINTR)) {
            printf("Error in select. \n");
            return SELECT_ERR;
        }

        if (FD_ISSET(sockfd, &readfds))
            register_new_client(sockfd);

        for (int i = 0; i < MAX_CLIENTS; i++) {
            int fd = clients[i];
            if ((fd > 0) && FD_ISSET(fd, &readfds))
                check_client(fd, i);
        }
    }

    return 0;
}