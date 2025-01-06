#ifndef BANK_H
#define BANK_H
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#define MAX_CLIENTS     100
#define MAX_OPERATIONS  10
#define BUF_SIZE        1024
#define PORT            1234

typedef enum {
    OK,             // informe le client que la commande (AJOUT, RETRAIT) s'est bien déroulée
    KO,             // informe le client que la commande ne s'est pas bien déroulée
    RES_SOLDE,      // envoi au client le solde actuel du compte et la date de la dernière opération
    RES_OPERATIONS  // envoi au client les 10 dernières opérations du compte sous le format type_opération date_opération montant_operation
} ResponseCode;

typedef enum {
    AJOUT,          // ajouter une somme sur le compte
    RETRAIT,        // retirer une somme du compte
    SOLDE,          // obtenir le solde du compte
    OPERATIONS      // obtenir les 10 dernières opérations du compte
} OperationType;

typedef struct Operation_t {
    char type_operation[10];    //Type de l'operation (AJOUT, RETRAIT, SOLDE, OPERATIONS)
    char date_operation[20];    //Date et heure au format "DD/MM/YYYY HH:MM:SS"
    int value_operation;        //Montant de l'operation
}Operation;

typedef struct Compte_t {
    char id_client[BUF_SIZE];   //nom d'utilisateur
    char id_compte[10];         //numero de compte bancaire (XXX.XXX.XX)
    char password[BUF_SIZE];    //mot de passe
    int solde;                //solde du compte bancaire
    Operation last_operations[10]; //Tableau stockant les 10 dernieres operations
    int num_operations;          //Nombre d'operations effectuées
}Compte;

typedef struct Client_t {
    char name[BUF_SIZE];       //nom du client 
    SOCKET _socket;               //socket du client
    struct sockaddr_in addr;   //adresse du socket client
     
}Client;

static int read_socket(SOCKET _socket, char *buffer);
static void write_socket(SOCKET _socket, const char *buffer);

static void disconnect_socket(SOCKET _socket);

void get_time_now(char *buffer);

#endif