#include "csapp.h"
#include "mongoose.h"

/**
 * Handles the connection event
 * @param event The event.
 * @param conn The connection
 * @return Supposed to return something? But NULL works just as fine.
 */
void *event_handler(enum mg_event event, struct mg_connection *conn) {
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    char cwd[1024];
    if (event == MG_NEW_REQUEST) {
        printf("Request method: %s\n", request_info->request_method);
        if (strcmp(request_info->request_method, "GET") == 0) {
            memset(cwd, 0, sizeof(cwd));
            char *filePath = request_info->uri;
            if (getcwd(cwd, sizeof(cwd)) == NULL) {
                fprintf(stderr, "Can not find current working directory??\n");
                return NULL;
            }
            strcat(cwd, filePath);

            FILE *file = fopen(cwd, "r");
            if (file == NULL) {
                return NULL;
            }
            // Get file size.
            fseek(file, 0, SEEK_END);
            long fsize = ftell(file);
            rewind(file);

            // Read all of contents into a String (char*)
            char *contents = malloc((size_t) (fsize + 1));
            fread(contents, (size_t) fsize, 1, file);
            fclose(file);

            // Send headers.
            mg_printf(conn,
                      "HTTP/1.1 200 OK\r\n"
                              "Cache: no-cache\r\n"
                              "Content-Type: %s\r\n"
                              "Content-Length: %ld\r\n"
                              "\r\n",
                      "text/plain",
                      fsize
            );
            // Write the data.
            mg_write(conn, contents, (size_t) fsize);
        }
    }
    return NULL;
}

int main(int argc, char **argv) {
    char *portString = malloc(6 * sizeof(char));
    if (portString == NULL) {
        fprintf(stderr, "No memory.");
        exit(EXIT_FAILURE);
    }

    int c;
    while ((c = getopt(argc, argv, "p:")) != -1) {
        switch (c) {
            case 'p': {
                int p = atoi(optarg);
                if (p < 0 || p > 65535) {
                    fprintf(stderr, "Invalid port entered.\n Defaulting to 8081.\n");
                    strcpy(portString, "8081");
                } else {
                    strcpy(portString, optarg);
                }
                break;
            }
            default:
                continue;
        }
    }

    const char *options[] = {
            "listening_ports", portString,
            "num_threads", "10",
            NULL
    };
    static struct mg_context *ctx;
    ctx = mg_start(&event_handler, NULL, options);
    if (ctx == NULL) {
        exit(EXIT_FAILURE);
    }
    printf("Server running, press enter to exit.\n");
    getchar();
    mg_stop(ctx);
    free(portString);
    return EXIT_SUCCESS;
}