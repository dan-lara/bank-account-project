#ifndef CLIENT_H
#define CLIENT_H

#include "bank.h"
#include "bank.c"

static SOCKET client_connect(const char *address);
static void client_disconnect(SOCKET socket);

static int read_server(SOCKET socket, char *buffer);
static void write_server(SOCKET socket, const char *buffer);

static void create_client(const char *name, const char *address);

#endif