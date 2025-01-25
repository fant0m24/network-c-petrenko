#include "caesar-client.h"

int main() {
    const char* server_ip = "127.0.0.1";
    short port = DEFAULT_PORT;

    SOCKET client_socket = connect_to_server(server_ip, port);
    if (client_socket < 0) {
        return -1;
    }

    char message[] = "Khoor Zruog";
    printf("Sending encrypted message: %s\n", message);

    if (send_encrypted_message(client_socket, message)) {
        printf("Failed to send message\n");
    }

    closesocket(client_socket);
    return 0;
}