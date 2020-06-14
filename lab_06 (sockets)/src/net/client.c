#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "info.h"

#define GETHOSTBYNAME_ERR -1
#define CONNECT_ERR -2
#define SEND_ERR -3
#define NUMBER_OF_MESSAGES 5

int main(void)
{
    srand(time(NULL));

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Error in socket().\n");
        return sockfd;
    }

    struct hostent* host = gethostbyname(SOCK_ADDR);
    if (!host) {
        perror("Error in gethostbyname(). \n");
        return GETHOSTBYNAME_ERR;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(SOCK_PORT);
    server_addr.sin_addr = *((struct in_addr*) host->h_addr_list[0]);

    if (connect(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        printf("Error in connect(). \n");
        return CONNECT_ERR;
    }

    char msg[MSG_LEN];
    for (int i = 0; i < NUMBER_OF_MESSAGES; i++) {
        memset(msg, 0, MSG_LEN);
        sprintf(msg, "%d message.", i);
        printf("%s", msg);

        if (send(sockfd, msg, strlen(msg), 0) < 0) {
            printf("Error in send(). \n");
            return SEND_ERR;
        }
        
        printf("Sended %d message\n", i);

        int wait_time = 1 + rand() % 3;
        sleep(wait_time);
    }

    printf("Client ended.\n");
    return 0;
}