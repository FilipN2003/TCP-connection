#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8181

//env - telnet localhost 8181

#define check_error(cond, msg)\
do{\
    if(!(cond)){\
        perror(msg);\
        fprintf(stderr, "%s: %d\n", __FILE__, __LINE__);\
        exit(EXIT_FAILURE);\
    }\
}while(0)

int main(){
    socklen_t addrlen = sizeof(struct sockaddr);
    int s, c;
    struct sockaddr_in srv, cli;
    memset(&srv, 0, sizeof(struct sockaddr_in)); //Setting bytes to zero
    memset(&cli, 0, sizeof(struct sockaddr_in));

    s = socket(AF_INET, SOCK_STREAM, 0); // socket fd
    check_error(s != -1, "socket failed");
    srv.sin_addr.s_addr = 0; //Setting the structure up
    srv.sin_port = htons(PORT);
    srv.sin_family = AF_INET;

    check_error(bind(s, (struct sockaddr *)&srv, sizeof(srv)) == 0, "bind failed");
    int maxQueue = 5; // Maximum length of the queue of pending connections for sockfd
    check_error(listen(s, maxQueue) == 0, "bind failed");

    printf("WAITING FOR CONNECTION\n");

    c = accept(s, (struct sockaddr *)&srv, &addrlen); // accept return new socket fd for connecting client

    char localBuf[256];
    char *data = "This message is returned to the client as a response to his request\n";

    read(c, localBuf, 255);
    printf("Clients request: %s\n", localBuf);
    write(c, data, strlen(data));

    close(s);
    close(c);
    exit(EXIT_SUCCESS);
}