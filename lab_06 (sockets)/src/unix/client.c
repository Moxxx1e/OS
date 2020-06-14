#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "info.h"

int main()
{
    int sockfd = socket(PF_LOCAL, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("Error in socket(). \n");
        return -1;
    }

    struct sockaddr server_addr;
    server_addr.sa_family = PF_LOCAL;
    strcpy(server_addr.sa_data, SOCKET_NAME);

    char msg[MSG_LEN];
    sprintf(msg, "Message from %d process client", getpid());
    sendto(sockfd, msg, strlen(msg), 0, &server_addr, sizeof(server_addr));
    close(sockfd);

    return 0;
}