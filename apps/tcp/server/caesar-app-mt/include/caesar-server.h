#pragma once

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "net-utils.h"
#include "pthread.h"

#define DEFAULT_PORT 7701
#define DEFAULT_QUEUE 20

extern SOCKET server_socket;

void free_socket();
int start_server(int argc, char* argv[]);
int init_server(short port, int queue_size);
void* handle_client(void* client_socket);
char* decrypt_caesar(const char* encrypted, int shift);