#include "caesar-app.h"

int main() {
    int receiver_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (receiver_socket < 0) {
        perror("Failed to create socket");
        return -1;
    }

    struct sockaddr_in receiver_addr;
    memset(&receiver_addr, 0, sizeof(receiver_addr));
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_port = htons(RECEIVER_PORT);
    receiver_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(receiver_socket, (struct sockaddr*)&receiver_addr, sizeof(receiver_addr)) < 0) {
        perror("Bind failed");
        close(receiver_socket);
        return -1;
    }

    printf("Receiver is running on port %d...\n", RECEIVER_PORT);

    process_message(receiver_socket);

    close(receiver_socket);
    return 0;
}