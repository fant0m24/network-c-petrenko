#include "caesar-server.h"

int main(int argc, char* argv[]) {
    atexit(free_socket);

    if (start_server(argc, argv)) {
        printf("Error starting server\n");
        return -1;
    }

    return 0;
}