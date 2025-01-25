#include "caesar-server.h"
#include <stdlib.h>
#include <ctype.h>

SOCKET server_socket = -1;

void free_socket() {
    if (server_socket > 0) {
        closesocket(server_socket);
    }
}

char* decrypt_caesar(const char* encrypted, int shift) {
    int len = strlen(encrypted);
    char* decrypted = malloc(len + 1);
    if (!decrypted) return NULL;

    for (int i = 0; i < len; i++) {
        if (isalpha(encrypted[i])) {
            char base = islower(encrypted[i]) ? 'a' : 'A';
            decrypted[i] = (encrypted[i] - base - shift + 26) % 26 + base;
        }
        else {
            decrypted[i] = encrypted[i];
        }
    }
    decrypted[len] = '\0';
    return decrypted;
}

int process_connection() {
    SOCKET client_socket = -1;

    while (1) {
        struct sockaddr_in client_addr;
        int len = sizeof(client_addr);

        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &len);

        if (client_socket <= 0) {
            printf("Error accepting connection\n");
            continue;
        }

        printf("Connection from: %s\n", inet_ntoa(client_addr.sin_addr));

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        int ret = recv(client_socket, buffer, sizeof(buffer), 0);
        if (ret <= 0) {
            printf("Error receiving data\n");
            continue;
        }

        printf("Encrypted message received: %s\n", buffer);

        int shift = 3; // Example shift
        char* decrypted = decrypt_caesar(buffer, shift);
        if (!decrypted) {
            char* error_msg = "Decryption failed";
            send(client_socket, error_msg, strlen(error_msg), 0);
        }
        else {
            printf("Decrypted message: %s\n", decrypted);
            send(client_socket, decrypted, strlen(decrypted), 0);
            free(decrypted);
        }

        closesocket(client_socket);
    }

    return 0;
}

int start_server(int argc, char* argv[]) {
    short port = DEFAULT_PORT;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket <= 0) {
        printf("Cannot create socket\n");
        return -1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (struct sockaddr*)&address, sizeof(address))) {
        printf("Cannot bind socket to port %d\n", port);
        return -1;
    }

    if (listen(server_socket, DEFAULT_QUEUE)) {
        printf("Cannot listen on port %d\n", port);
        return -1;
    }

    printf("Server running on port %d\n", port);

    return process_connection();
}