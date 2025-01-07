#ifndef CLIENT_H
#define CLIENT_H

#include "bank.h"
#include "bank.c"

static SOCKET client_connect(const char *address, SOCKADDR_IN *addr);
static void client_disconnect(SOCKET socket);

static int read_server(SOCKET socket, SOCKADDR_IN *addr, char *buffer);
static void write_server(SOCKET socket, SOCKADDR_IN *addr, char *buffer);

static void create_client(const char *name, char *address);

#endif