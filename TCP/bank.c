#include "bank.h"

static int read(SOCKET socket, char *buffer){
    int index = 0;
    if((index = recv(socket, buffer, BUF_SIZE - 1, 0)) < 0) {
        perror("recv()");
        exit(errno);
    }
    buffer[index] = 0;
    return index;
}
static void write(SOCKET socket, const char *buffer){
    if(send(socket, buffer, strlen(buffer), 0) < 0) {
        perror("send()");
        exit(errno);
    }
}

static void disconnect(SOCKET socket){
    closesocket(socket);
}

void get_time_now(char *buffer, size_t buffer_size)
{
    /* lire l'heure courante */
    time_t now = time (NULL);
    
    //char s_now[sizeof "JJ/MM/AAAA HH:MM:SS"];
    
    /* la convertir en heure locale */
    struct tm tm_now = *localtime (&now);
 
    /* Créer une chaine JJ/MM/AAAA HH:MM:SS */
    strftime (buffer, buffer_size, "%d/%m/%Y %H:%M:%S", &tm_now);
}