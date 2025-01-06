#include "bank.h"

static int read_socket(SOCKET _socket, char *buffer){
    int index = 0;
    if((index = recv(_socket, buffer, BUF_SIZE - 1, 0)) < 0) {
        perror("recv()");
        exit(errno);
    }
    buffer[index] = 0;
    return index;
}
static void write_socket(SOCKET _socket, const char *buffer){
    if(send(_socket, buffer, strlen(buffer), 0) < 0) {
        perror("send()");
        exit(errno);
    }
}

static void disconnect_socket(SOCKET _socket){
    closesocket(_socket);
}

void get_time_now(char *buffer)
{
    /* lire l'heure courante */
    time_t now = time (NULL);
    
    //char s_now[sizeof "JJ/MM/AAAA HH:MM:SS"];
    
    /* la convertir en heure locale */
    struct tm tm_now = *localtime (&now);
 
    /* Créer une chaine JJ/MM/AAAA HH:MM:SS */
    int buffer_size = 20;
    strftime (buffer, buffer_size, "%d/%m/%Y %H:%M:%S", &tm_now);
}