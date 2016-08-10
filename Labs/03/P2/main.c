
#include <regex.h>
#include "csapp.h"

regex_t regex;
int reti;
char msgbuf[100];
int SIZE_FILE = 10000;

/**
 * Handle the conection
 * @param connfd The connection file descriptor.
 */
void handle(int connfd) {
    ssize_t n;
    char buf[MAXLINE];
    rio_t rio;
    char *fileName = NULL;

    Rio_readinitb(&rio, connfd);
    while (1) {
        n = rio_readlineb(&rio, buf, MAXLINE);
        printf("server received %d bytes\n", (int) n);
        printf("Buf: %s", buf);
        regmatch_t pmatch[10];

        reti = regexec(&regex, buf, 10, pmatch, 0);
        if (!reti) {
            size_t len = (size_t) (pmatch[1].rm_eo - pmatch[1].rm_so);
            fileName = malloc(len * sizeof(char));
            memcpy(fileName, buf + pmatch[1].rm_so, len);
            fileName[len] = '\0';
        } else {
            //printf("NOT FOUND?!?!\n");
        }
        if (n <= 0) {
            break;
        }
        if (strcmp(buf, "") == 0) {
            break;
        }
    }
    char *msg = malloc(sizeof(char) * SIZE_FILE);
    memset(msg, 0, sizeof(char) * SIZE_FILE);

    if (fileName == NULL) {
        strcat(msg, "NO FILE SPECIFIED.\n");
    } else {
        printf("File name: %s\n", fileName);
        int x = 0;
        FILE *fileWanted = fopen(fileName, "r");
        if (fileWanted == NULL) {
            strcat(msg, "FILE NOT FOUND.\n");
        } else {
            char word[256];
            while (fgets(word, sizeof(word), fileWanted) != NULL) {
                rio_writen(connfd, word, strlen(word));
                rio_readlineb(&rio, buf, MAXLINE);
            }
            strcat(msg, "\n\n");
        }
    }
    rio_writen(connfd, msg, strlen(msg));
    rio_readlineb(&rio, buf, MAXLINE);
    free(fileName);
    free(msg);
}

int main(int argc, char **argv) {
    int port = 49861;

    reti = regcomp(&regex, ".* (.*) .*", REG_EXTENDED); // Compile regex to read the input.
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    int c;
    while ((c = getopt(argc, argv, "p:")) != -1) {
        switch (c) {
            case 'p': {
                int p = atoi(optarg);
                if (p < 0 || p > 65535) {
                    fprintf(stderr, "Invalid port entered.\n Defaulting to %d.", port);
                } else {
                    port = p;
                }
                break;
            }
            default:
                continue;
        }
    }

    int listenfd, connfd;
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

        handle(connfd);
        Close(connfd);
        sleep(1);
    }
}