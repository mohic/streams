/*
        Maxime LECAT (mlecat)
        Christophe MOHIMONT (cmohimo)

        ce fichier contient les declarations pour l'utilisation de socket cote serveur
*/

#ifndef __SERVERSCK_H__
#define __SERVERSCK_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

typedef struct sockaddr_in SOCKADDR_IN; // définit une structure de socket de type TCP/IP
typedef struct sockaddr    SOCKADDR; // définit une structure de socket générique

int ouvrirSocket(unsigned short port);
int accepterClient(int socket);
int fermerSocket(int socket);
int envoyerMessage(int socket, char *message);
int recevoirMessage(int socket, char *message, int length);

#endif
