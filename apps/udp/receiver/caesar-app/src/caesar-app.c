#include "caesar-app.h"

void process_message(int receiver_socket) {
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        int bytes_received = recvfrom(
            receiver_socket, buffer, BUFFER_SIZE, 0,
            (struct sockaddr*)&sender_addr, &sender_addr_len
        );

        if (bytes_received <= 0) {
            printf("Failed to receive data.\n");
            continue;
        }

        printf("Received encrypted message: %s\n", buffer);

        char* decrypted = decrypt_caesar(buffer, 3);
        char response[BUFFER_SIZE];

        if (decrypted) {
            snprintf(response, sizeof(response), "Decrypted: %s", decrypted);
        }
        else {
            snprintf(response, sizeof(response), "Decryption failed");
        }

        sendto(
            receiver_socket, response, strlen(response), 0,
            (struct sockaddr*)&sender_addr, sender_addr_len
        );

        printf("Sent response: %s\n", response);
    }
}

char* decrypt_caesar(const char* text, int shift) {
    static char result[BUFFER_SIZE];
    memset(result, 0, sizeof(result));

    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        if (isalpha(c)) {
            char offset = isupper(c) ? 'A' : 'a';
            result[i] = (c - offset - shift + 26) % 26 + offset;
        }
        else {
            result[i] = c;
        }
    }
    return result;
}