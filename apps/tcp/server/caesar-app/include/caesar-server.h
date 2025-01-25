#pragma once

#include "stdio.h"
#include "string.h"
#include "net-utils.h"

#define DEFAULT_PORT 7703
#define DEFAULT_QUEUE 5

extern SOCKET server_socket;

void free_socket();

int start_server(int argc, char* argv[]);

int process_connection();

char* decrypt_caesar(const char* encrypted, int shift);