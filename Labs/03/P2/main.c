
#include "csapp.h"

void echo(int connfd) {
    ssize_t n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while (1) {
        n = rio_readlineb(&rio, buf, MAXLINE);
        printf("server received %d bytes\n", (int) n);
        printf("Buf: %s", buf);
        if (n <= 0) {
            break;
        }
        if (strcmp(buf, "") == 0) {
            break;
        }
    }
    char *msg = (char *) "Hey bb <3\r\n";

    rio_writen(connfd, msg, strlen(msg));
    rio_readlineb(&rio, buf, MAXLINE);

}

int main(int argc, char **argv) {
    int listenfd, connfd;
    int port = 49861;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    listenfd = Open_listenfd(port);
    char client_hostname[MAXLINE], client_port[MAXLINE];

    struct timeval tv;
    tv.tv_sec = 1; // 1 second time out in reading.
    tv.tv_usec = 0;

    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
        getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
        printf("Connected to %s:%s\n", client_hostname, client_port);
        setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));

        echo(connfd);
        Close(connfd);
        sleep(1);
    }
}