#include "client.h"

static SOCKET client_connect(const char *address, SOCKADDR_IN *addr){ 
    //Creation du socket pour IPv4 et TCP
    SOCKET _socket = socket(AF_INET, SOCK_DGRAM, 0);

    printf("[+] Socket créé. INFO : %d\n", _socket);
    if(_socket == INVALID_SOCKET){
        printf("[!] Erreur lors de la création d'un socket\n");
        perror("socket()");
        exit(errno);
    }

    struct hostent *hostinfo;    
    hostinfo = gethostbyname(address);
    if (hostinfo == NULL) {
        fprintf (stderr, "Unknown host %s.\n", address);
        exit(EXIT_FAILURE);
    }

    addr->sin_addr = *(IN_ADDR *) hostinfo->h_addr_list[0];
    addr->sin_port = htons(PORT); //1234
    addr->sin_family = AF_INET; //IPv4

    printf("[i] Socket du serveur %s:%d recuperé\n", address, PORT);

    return _socket;
}


static void client_disconnect(SOCKET socket){ disconnect_socket(socket); }

static int read_server(SOCKET socket, SOCKADDR_IN *addr, char *buffer){ return read_socket(socket, addr, buffer); }

static void write_server(SOCKET socket, SOCKADDR_IN *addr, char *buffer){ write_socket(socket, addr, buffer); }

static void read_input(char *buffer) {
    fgets(buffer, BUF_SIZE - 1, stdin);
    char *p = NULL;
    if ((p = strchr(buffer, '\n')) != NULL)
        *p = 0;
    else
        buffer[BUF_SIZE - 1] = 0;
}

static void create_client(const char *address, char *name){
    SOCKADDR_IN addr = {0};
    SOCKET _socket = client_connect(address, &addr);
    char buffer[BUF_SIZE];

    fd_set rdfs;

    write_server(_socket, &addr, name);

    while (1){
        FD_ZERO(&rdfs);
        FD_SET(STDIN_FILENO, &rdfs);
        FD_SET(_socket, &rdfs);

        if (select(_socket + 1, &rdfs, NULL, NULL, NULL) == -1){
            perror("select()");
            exit(errno);
        }

        if (FD_ISSET(STDIN_FILENO, &rdfs)){
            read_input(buffer);
            write_server(_socket, &addr, buffer);
        }
        else if (FD_ISSET(_socket, &rdfs)){
            int n = read_server(_socket, &addr, buffer);
            if (!n){
                printf("[#] Server disconnected !\n");
                break;
            }
            printf("[<] %s\n", buffer);
        }
    }
    client_disconnect(_socket);
    printf("[#] Déconnexion du client\n");
}



int main(int argc, char **argv){
    if (argc != 3 && argc != 1){
        printf("[i] Usage: %s <address> <name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    create_client(argv[1], argv[2]);

    return EXIT_SUCCESS;
}