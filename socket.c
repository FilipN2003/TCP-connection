#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define check_error(cond, msg)\
do{\
    if(!(cond)){\
        perror(msg);\
        fprintf(stderr, "%s: %d\n", __FILE__, __LINE__);\
        exit(EXIT_FAILURE);\
    }\
}while(0)

#define IP "142.250.180.206" // IPv4 of www.youtube.com - host www.youtube.com
#define PORT 80 // Port number for HTTP

int main(){
    int s; //file descriptor
    struct sockaddr_in sock;

    s = socket(AF_INET, SOCK_STREAM, 0); //Creating new socket
    check_error(s != -1, "socket failed");

    sock.sin_addr.s_addr = inet_addr(IP);
    sock.sin_port = htons(PORT); //Host to network short
    sock.sin_family = AF_INET;

    check_error(connect(s, (struct sockaddr*)&sock, sizeof(struct sockaddr)) != -1, "connect failed");

    char *request = "HEAD / HTTP/1.0\r\n\r\n"; //Request that we are sending to the server
    char localBuf[256]; //Buffer for storing message received from the server

    write(s, request, strlen(request));
    read(s, localBuf, 255);
    close(s); //closing socket

    printf("%s\n", localBuf);
    exit(EXIT_SUCCESS);
}