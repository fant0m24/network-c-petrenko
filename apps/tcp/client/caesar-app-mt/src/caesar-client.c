#include "caesar-client.h"

int connect_to_server(const char* ip, short port) {
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket <= 0) {
        printf("Cannot create socket\n");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Cannot connect to server\n");
        return -1;
    }

    return client_socket;
}

int send_encrypted_message(SOCKET socket, const char* message) {
    if (send(socket, message, strlen(message), 0) <= 0) {
        printf("Error sending message\n");
        return -1;
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int ret = recv(socket, buffer, sizeof(buffer), 0);
    if (ret <= 0) {
        printf("Error receiving response\n");
        return -1;
    }

    printf("Response from server: %s\n", buffer);

    return 0;
}