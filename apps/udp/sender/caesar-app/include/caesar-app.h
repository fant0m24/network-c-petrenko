#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define RECEIVER_PORT 9555
#define SENDER_PORT 5559
#define RECEIVER_IP "127.0.0.1"

void send_message(const char* message);