#ifndef SERVEUR_H
#define SERVEUR_H

#include "bank.h"
#include "bank.c"

#define CHECK_ORIGIN 0

static SOCKET server_connect(void);
static void server_disconnect(SOCKET _socket);

static int read_client(SOCKET _socket, SOCKADDR_IN *addr, char *buffer);
static void write_client(SOCKET _socket, SOCKADDR_IN *addr, char *buffer);
static void broadcast_message(Client *clients, Client origin, char *buffer, int num_clients, int its_server);

static void remove_client(Client *clients, int to_remove, int *num_clients);
static void clear_clients(Client *clients, int num_clients);

static void handle_command(Client *client, Compte *comptes, int num_comptes, const char *command, int check_origin);
void add_operation(Compte *comptes, int index, char *type_operation, int value);
static int compare_compte(Compte c1, Compte c2);

static void run_server(Compte *comptes, int num_comptes);

#endif