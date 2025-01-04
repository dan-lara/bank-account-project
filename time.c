#include <stdio.h>
#include <time.h>
 
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

int main(void)
{
    char s_now[sizeof "JJ/MM/AAAA HH:MM:SS"];
    get_time_now(s_now, sizeof s_now);
    printf("'%s'\n", s_now);
    return 0;
}