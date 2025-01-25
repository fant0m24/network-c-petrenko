#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define RECEIVER_PORT 9555
#define SENDER_PORT 5559

void process_message(int receiver_socket);

char* decrypt_caesar(const char* text, int shift);