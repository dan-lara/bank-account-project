#include "serveur.h"

static SOCKET server_connect(void)
{

    // Creation du socket pour IPv4 et TCP
    SOCKET _socket = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN sin = {0};

    printf("[+] Socket créé avec succès. INFO : %d\n", _socket);

    if (_socket == INVALID_SOCKET)
    {
        printf("[!] Erreur lors de la création d'un socket\n");
        perror("socket()");
        exit(errno);
    }

    // Livre reception de toutes les adresses IP
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;

    // bind() permet d'associer une adresse IP et un port à un socket
    if (bind(_socket, (SOCKADDR *)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        perror("bind()");
        exit(errno);
    }
    // listen() permet de mettre le serveur en mode écoute
    if (listen(_socket, MAX_CLIENTS) == SOCKET_ERROR)
    {
        perror("listen()");
        exit(errno);
    }
    return _socket;
}

static void server_disconnect(SOCKET _socket) { disconnect_socket(_socket); }

static int read_client(SOCKET _socket, char *buffer) { return read_socket(_socket, buffer); }
static void write_client(SOCKET _socket, const char *buffer) { write_socket(_socket, buffer); }

static void broadcast_message(Client *clients, Client origin, const char *buffer, int num_clients, int its_server)
{
    printf("[>] Broadcast : %s\n", buffer);
    char message[BUF_SIZE] = {0};
    if (!its_server)
    {
        strncpy(message, origin.name, BUF_SIZE - 1);
        strncat(message, " : ", sizeof(message - strlen(message) - 1));
    }
    strncat(message, buffer, sizeof(message - strlen(message) - 1));
    for (int i = 0; i < num_clients; i++)
        if (clients[i]._socket != origin._socket)
            write_client(clients[i]._socket, buffer);
}

static void remove_client(Client *clients, int to_remove, int *num_clients)
{
    // On décale les clients pour combler le trou
    memmove(clients + to_remove, clients + to_remove + 1, (*num_clients - to_remove - 1) * sizeof(SOCKET));
    (*num_clients)--;
}

static void clear_clients(Client *clients, int num_clients)
{
    for (int i = 0; i < num_clients; i++)
        disconnect_socket(clients[i]._socket);
}

static int compare_compte(Compte c1, Compte c2)
{
    return !strcmp(c1.id_client, c2.id_client) && !strcmp(c1.id_compte, c2.id_compte) && !strcmp(c1.password, c2.password);
}

void add_operation(Compte *comptes, int index, char *type_operation, int value)
{
    int num_op = comptes[index].num_operations;

    if (num_op < MAX_OPERATIONS)
    {
        strcpy(comptes[index].last_operations[num_op].type_operation, type_operation);

        get_time_now(comptes[index].last_operations[num_op].date_operation);

        comptes[index].last_operations[num_op].value_operation = value;

        comptes[index].num_operations++;
    }
    else
    {
        for (int j = 1; j < MAX_OPERATIONS; j++)
            // Decaler les opérations si le tableau est plein
            comptes[index].last_operations[j - 1] = comptes[index].last_operations[j];

        strncpy(comptes[index].last_operations[num_op - 1].type_operation, type_operation, 50);

        get_time_now(comptes[index].last_operations[num_op - 1].date_operation);

        comptes[index].last_operations[num_op - 1].value_operation = value;
    }
    return;
}

static void handle_command(Client *client, Compte *comptes, int num_comptes, const char *command)
{
    char op_request[10];
    Compte c_compte;
    int value;

    sscanf(command, "%s  %s  %s  %s  %d", op_request, c_compte.id_client, c_compte.id_compte, c_compte.password, &value);
    printf("[<] Commande %s reçue de : %s\n", op_request, client->name);
    int i;

    if (!strcmp(op_request, "AJOUT"))
    {
        for (int i = 0; i < num_comptes; i++)
            if (compare_compte(comptes[i], c_compte))
            {

                comptes[i].solde += value;

                add_operation(comptes, i, "AJOUT", value);
                get_time_now(comptes[i].last_operations[comptes[i].num_operations - 1].date_operation);

                write_client(client->_socket, "OK\n");
                printf("[+] AJOUT réussi d'une valeur de : %d.%d€, compte : %s\n", value / 100, value % 100, c_compte.id_compte);
                return;
            }
    }
    else if (!strcmp(op_request, "RETRAIT"))
    {
        for (int i = 0; i < num_comptes; i++)
            if (compare_compte(comptes[i], c_compte))
            {
                if (comptes[i].solde >= value)
                {

                    comptes[i].solde -= value;

                    add_operation(comptes, i, "RETRAIT", -value);

                    write_client(client->_socket, "OK\n");
                    printf("[+] RETRAIT réussi d'une valeur de : %d.%d€, compte : %s\n", value / 100, value % 100, c_compte.id_compte);
                }
                else
                {
                    write_client(client->_socket, "KO\n");
                    printf("[-] RETRAIT échoué : solde insuffisant\n");
                    printf("%s, vous avez %d.%d€, et souhaite retirer %d.%d€\n", c_compte.id_compte, comptes[i].solde / 100, comptes[i].solde % 100, value / 100, value % 100);
                }
                return;
            }
    }
    else if (!strcmp(op_request, "SOLDE"))
    {
        for (int i = 0; i < num_comptes; i++)
            if (compare_compte(comptes[i], c_compte))
            {

                char last_operation[50];

                if (comptes[i].num_operations > 0)
                    strcpy(last_operation, comptes[i].last_operations[comptes[i].num_operations - 1].date_operation);
                else
                    strcpy(last_operation, "Aucune somme n'a encore été versée");

                char response[BUF_SIZE];
                snprintf(response, BUF_SIZE, "RES_SOLDE | Solde : %d.%d€ \t| Dernière opération : %s\n", comptes[i].solde / 100, comptes[i].solde % 100, last_operation);
                write_client(client->_socket, response);
                printf("[+] SOLDE consulté : %d.%d€, compte : %s\n", comptes[i].solde / 100, comptes[i].solde % 100, c_compte.id_compte);
                return;
            }
    }
    else if (!strcmp(op_request, "OPERATIONS"))
    {
        for (int i = 0; i < num_comptes; i++)
            if (compare_compte(comptes[i], c_compte))
            {
                char operations[BUF_SIZE] = "RES_OPERATIONS\n";
                for (int j = 0; j < comptes[i].num_operations; j++)
                {
                    char operation[128];
                    snprintf(operation, sizeof(operation), "%2d. %s \t| %s \t| %d.%d€\n", j + 1, comptes[i].last_operations[j].type_operation, comptes[i].last_operations[j].date_operation, comptes[i].last_operations[j].value_operation / 100, comptes[i].last_operations[j].value_operation % 100);
                    strncat(operations, operation, BUF_SIZE - strlen(operations) - 1);
                }
                write_client(client->_socket, operations);
                printf("[+] OPERATIONS consultées : %d envoyées pour le compte : %s\n", c_compte.num_operations, c_compte.id_compte);
                return;
            }
    }

    write_client(client->_socket, "KO\n");
    printf("[-] Erreur sur l'authentication\n");
    return;
}

static void run_server(Compte *comptes, int num_comptes)
{
    printf("[i] Début du programme serveur\n");

    SOCKET _socket = server_connect();
    printf("[i] Serveur TCP en écoute sur le port %d...\n", PORT);

    char buffer[BUF_SIZE];
    Client clients[MAX_CLIENTS];

    int num_clients = 0;
    int max = _socket;

    fd_set rdfs;

    while (1)
    {
        FD_ZERO(&rdfs);
        FD_SET(STDIN_FILENO, &rdfs);
        FD_SET(_socket, &rdfs);
        
        for (int i = 0; i < num_clients; i++) {
            FD_SET(clients[i]._socket, &rdfs);
        }

        if (select(max + 1, &rdfs, NULL, NULL, NULL) == INVALID_SOCKET)
        {
            perror("select()");
            exit(errno);
        }

        if (FD_ISSET(STDIN_FILENO, &rdfs))
        {
            printf("[#] Arrêt du serveur via le clavier.\n");
            break;
        }
        else if (FD_ISSET(_socket, &rdfs))
        {
            SOCKADDR_IN csin = {0};
            socklen_t sinsize = sizeof(csin);

            SOCKET c_socket = accept(_socket, (SOCKADDR *)&csin, &sinsize);
            if (c_socket == SOCKET_ERROR)
            {
                perror("accept()");
                continue;
            }
               
            printf("[+] Nouveau client sur %s:%d\n", inet_ntoa(csin.sin_addr), ntohs(csin.sin_port));

            if (read_client(c_socket, buffer) == -1)
                continue;
            
            if (c_socket > max)
                max = c_socket;

            FD_SET(c_socket, &rdfs);

            Client c = {"", c_socket, csin};
            strncpy(c.name, buffer, BUF_SIZE - 1);
            clients[num_clients] = c;

            char message[BUF_SIZE];
            snprintf(message, BUF_SIZE, "%.50s,\n", c.name);
        
            char help_message[BUF_SIZE] =
                "bienvenue sur le serveur bancaire!\n Voici les commandes disponibles :\n"
                "- AJOUT      <id_client> <id_compte> <password> <somme> : Ajouter une somme au compte.\n"
                "- RETRAIT    <id_client> <id_compte> <password> <somme> : Retirer une somme du compte.\n"
                "- SOLDE      <id_client> <id_compte> <password> \t: Consulter le solde du compte.\n"
                "- OPERATIONS <id_client> <id_compte> <password> \t: Consulter les dernières opérations du compte.\n"
                "Veuillez respecter la syntaxe des commandes pour éviter toute erreur.\n";

            strncat(message, help_message, BUF_SIZE - strlen(message) - 1);
            write_client(c_socket, message);

            printf("[>] Message : %s s'est connecté \n", c.name);
            snprintf(message, BUF_SIZE, "%.50s s'est connecté.", c.name);
            broadcast_message(clients, c, message, num_clients, 1);
            num_clients++;
        }
        else
        {
            for (int j = 0; j < num_clients; j++)
            {
                if (FD_ISSET(clients[j]._socket, &rdfs))
                {
                    Client client = clients[j];
                    int c = read_client(clients[j]._socket, buffer);
                    if (c == 0)
                    {
                        printf("[-] Client déconnecté : %s\n", clients[j].name);
                        disconnect_socket(clients[j]._socket);
                        remove_client(clients, j, &num_clients);
                        strncpy(buffer, client.name, BUF_SIZE - 1);
                        strncat(buffer, " disconnected !", BUF_SIZE - strlen(buffer) - 1);
                        broadcast_message(clients, client, buffer, num_clients, 1);
                    }
                    else
                    {
                        handle_command(&clients[j], comptes, num_comptes, buffer);
                    }
                    break;
                }
            }
        }
    }
    clear_clients(clients, num_clients);
    server_disconnect(_socket);
    printf("[#] Fin du serveur\n");
}

int main(int argc, char **argv)
{
    int num_comptes = 5;
    Compte comptes[5] = {
        {"JeanDupont", "00112345", "mdpJean", 150000, 
            {{"AJOUT", "01/01/2023 10:00:00", 150000}}
            , 1
        },
        {"MarieCurie", "00223456", "mdpMarie", 200000, 
            {{"AJOUT", "02/02/2023 11:00:00", 200000}, {"RETRAIT", "03/03/2023 12:00:00", -50000}}
            , 2
        },
        {"PierreDurand", "00334567", "mdpPierre", 300000, 
            {{"AJOUT", "04/04/2023 13:00:00", 300000}, {"RETRAIT", "05/05/2023 14:00:00", -100000}, {"AJOUT", "06/06/2023 15:00:00", 50000}}
            , 3
        },
        {"LucieMartin", "00445678", "mdpLucie", 250000, 
            {{"AJOUT", "07/07/2023 16:00:00", 250000}, {"RETRAIT", "08/08/2023 17:00:00", -75000}, {"AJOUT", "09/09/2023 18:00:00", 100000}, {"RETRAIT", "10/10/2023 19:00:00", -50000}}
            , 4
        },
        {"DanielLara", "00556789", "mdpDaniel", 100000, 
            {{"AJOUT", "11/11/2023 20:00:00", 100000}, {"RETRAIT", "12/12/2023 21:00:00", -25000}, {"AJOUT", "13/01/2024 22:00:00", 50000}, {"RETRAIT", "14/02/2024 23:00:00", -10000}, {"AJOUT", "15/03/2024 00:00:00", 20000}}
            , 5
        }
    };
    run_server(comptes, num_comptes);
    return EXIT_SUCCESS;
}