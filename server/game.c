#include "game.h"

int traiterMessage(int sckClient, char *message, game *g, int joueur) {
	if (strlen(message) <= 0)
		return -1;

	switch(message[0]) {
		case '1':
			printf("Nom du joueur: %s", message + 2);
			g->nom[joueur] = message +2;
			break;
		default:
			printf("Message inconnu: %s\n", message);
	}

	return 0;
}
