#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "info.h"
#include "signal.h"
#include <string.h>

#define SOCKET_ERR -1
#define BIND_ERR -2
#define RECV_ERR -3

int sockfd;
int sigint_flag = 0;

void sigint_catch(int signum)
{
    close(sockfd);
    unlink(SOCKET_NAME);
    printf("\nCtrl + C catched; Socket closed\n");
    sigint_flag = 1;
}

int main()
{
    sockfd = socket(PF_LOCAL, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("Error in socket(). \n");
        return SOCKET_ERR;
    }

    struct sockaddr client_addr;
    client_addr.sa_family = PF_LOCAL;
    strcpy(client_addr.sa_data, SOCKET_NAME);

    if (bind(sockfd, &client_addr, sizeof(client_addr)) < 0) {
        close(sockfd);
        unlink(SOCKET_NAME);
        printf("Error in bind(). \n");
        return BIND_ERR;
    }

    printf("Server is running.");
    signal(SIGINT, sigint_catch);

    char msg[MSG_LEN];
    for(;;) {
        int msg_len = recv(sockfd, msg, sizeof(msg), 0);
        
        if (sigint_flag == 1) {
            return 0;
        }
        
        if (msg_len < 0) {
            close(sockfd);
            unlink(SOCKET_NAME);
            printf("Error in recv(). \n");
            return RECV_ERR;  
        }

        msg[msg_len] = 0;
        printf("\nMessage from client: %s", msg);
    }

    close(sockfd);
    unlink(SOCKET_NAME);
    printf("Socket closed."); 
    return 0;
}