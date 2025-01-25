#include "caesar-app.h"

void send_message(const char* message) {
    int sender_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (sender_socket < 0) {
        perror("Failed to create socket");
        exit(-1);
    }

    struct sockaddr_in receiver_addr;

    memset(&receiver_addr, 0, sizeof(receiver_addr));
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_port = htons(RECEIVER_PORT);
    inet_pton(AF_INET, RECEIVER_IP, &receiver_addr.sin_addr);

    struct sockaddr_in sender_addr;

    memset(&sender_addr, 0, sizeof(sender_addr));
    sender_addr.sin_family = AF_INET;
    sender_addr.sin_port = htons(SENDER_PORT);
    sender_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sender_socket, (struct sockaddr*)&sender_addr, sizeof(sender_addr)) < 0) {
        perror("Bind failed");
        close(sender_socket);
        exit(-1);
    }

    sendto(sender_socket, message, strlen(message), 0,
        (struct sockaddr*)&receiver_addr, sizeof(receiver_addr));

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    socklen_t receiver_addr_len = sizeof(receiver_addr);
    int bytes_received = recvfrom(
        sender_socket, buffer, BUFFER_SIZE, 0,
        (struct sockaddr*)&receiver_addr, &receiver_addr_len
    );

    if (bytes_received > 0) {
        printf("Receiver response: %s\n", buffer);
    }
    else {
        printf("Failed to receive response from receiver.\n");
    }

    close(sender_socket);
}