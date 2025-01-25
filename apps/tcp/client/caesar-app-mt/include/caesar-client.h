#pragma once

#include "stdio.h"
#include "string.h"
#include "net-utils.h"

#define DEFAULT_PORT 7703

int connect_to_server(const char* ip, short port);

int send_encrypted_message(SOCKET socket, const char* message);