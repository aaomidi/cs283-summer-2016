
#include "csapp.h"

int main(int argc, char **argv) {
    int port = 80;
    char *host = NULL;
    char *file = NULL;
    int c;
    while ((c = getopt(argc, argv, "p:h:f:")) != -1) {
        switch (c) {
            case 'h': {
                host = malloc(strlen(optarg) * sizeof(char));
                memset(host, 0, strlen(optarg) * sizeof(char));
                strcpy(host, optarg);
                break;
            }
            case 'p': {
                int p = atoi(optarg);
                if (p < 0 || p > 65565) {
                    fprintf(stderr, "Invalid port entered.\n Defaulting to 80.");
                } else {
                    port = p;
                }
                break;
            }
            case 'f': {
                file = malloc(strlen(optarg) * sizeof(char));
                memset(file, 0, strlen(optarg) * sizeof(char));
                strcpy(file, optarg);
                break;
            }
            case '?':
                if (isprint(optopt)) {
                    fprintf(stderr, "Unknown option '-%c'.\n", optopt);
                } else {
                    fprintf(stderr, "Unknown option char`\\x%x'.\n", optopt);
                }

                break;
            default:
                continue;
        }
    }
    if (host == NULL) {
        fprintf(stderr, "Please enter the host.\n ./executable -h www.example.com -f index.html -p 80");
        return 1;
    } else if (file == NULL) {
        fprintf(stderr, "Please enter the file.\n ./executable -h www.example.com -f index.html -p 80");
        return 1;
    }

    int clientfd = Open_clientfd(host, port);
    char *m = "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n";
    char *message = malloc(sizeof(char) * strlen(m) + strlen(file) + strlen(host) + 5);
    sprintf(message, m, file, host);

    rio_t rio;
    char buf[MAXLINE];
    Rio_readinitb(&rio, clientfd);
    Rio_writen(clientfd, message, strlen(message));

    int i = 0;
    for (i = 0; i < 100000; i++) {
        Rio_readlineb(&rio, buf, MAXLINE);

        if (strtol(buf, NULL, 16) != '\0') {
            continue;
        }

        if (strcmp(buf, "0\r\n") == 0) {
            break;
        }

        Fputs(buf,stdout);
    }
    free(host);
    free(file);
    return 0;
}