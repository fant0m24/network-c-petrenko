#include "caesar-server.h"

SOCKET server_socket = -1;

void free_socket() {
    if (server_socket > 0) {
        closesocket(server_socket);
    }
}

int start_server(int argc, char* argv[]) {
    short port = DEFAULT_PORT;
    int queue_size = DEFAULT_QUEUE;

    if (argc >= 3) {
        sscanf(argv[2], "%d", &port);
    }

    if (init_server(port, queue_size)) {
        printf("Cannot initialize server\n");
        return -1;
    }

    printf("Server running on port %d\n", port);

    while (1) {
        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);

        SOCKET client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            printf("Error accepting client connection\n");
            continue;
        }

        printf("Connection established from: %s\n", inet_ntoa(client_addr.sin_addr));

        pthread_t thread_id;
        SOCKET* client_sock_ptr = malloc(sizeof(SOCKET));
        *client_sock_ptr = client_socket;

        if (pthread_create(&thread_id, NULL, handle_client, (void*)client_sock_ptr) != 0) {
            printf("Error creating thread\n");
            free(client_sock_ptr);
            closesocket(client_socket);
        }

        pthread_detach(thread_id);
    }

    return 0;
}

int init_server(short port, int queue_size) {
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

    if (listen(server_socket, queue_size)) {
        printf("Cannot listen on port %d\n", port);
        return -1;
    }

    return 0;
}

void* handle_client(void* client_socket) {
    SOCKET socket = *(SOCKET*)client_socket;
    free(client_socket);

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int ret = recv(socket, buffer, sizeof(buffer), 0);
    if (ret <= 0) {
        printf("Error receiving data from client\n");
        closesocket(socket);
        return NULL;
    }

    printf("Encrypted message received: %s\n", buffer);

    char* decrypted = decrypt_caesar(buffer, 3);
    if (!decrypted) {
        char* error_msg = "Decryption failed";
        send(socket, error_msg, strlen(error_msg), 0);
    }
    else {
        send(socket, decrypted, strlen(decrypted), 0);
        free(decrypted);
    }

    closesocket(socket);
    return NULL;
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