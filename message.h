#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <errno.h>
#include <pthread.h>
#include "includes.h"

#define TYPE_CONNEXION 0
#define TYPE_CONNEXION_AUTHORIZED 0
/*structure thread poisson*/
typedef struct{					/* Description d'une case sur la grille de simulation */
	int element;							/* Ce qui est present sur la case */
	pthread_t *poisson;						/* Identifiant du thread de la poisson presente sur la case */
	pthread_mutex_t mutex;
	pthread_cond_t cond;					/* Protection de la case */
}case_t;
/*structure du joueur*/
typedef struct{
	int id;
	int furtif;
	int points;
	int poireaus;
	int posxHAMMECON;
 	 int posyHAMMECON;
}joueur_t;
/*structure test d'update message serveur*/
typedef struct{
	int what;
	int posx;
	int posy;
	int type;
}update_t;
/*grille étazng*/
typedef struct{
	int grille[NB_LIGNES_SIM][NB_COL_SIM];
}grille_t;
/*structure des poissons*/
typedef struct{
	int id;
	int ETAT;
	int val;
	int stun;
	int posx;
	int posy;
	int posxFUIR;
	int posyFUIR;
}poisson_t;


/**
 * @struct requete_t
 * @field type type de la requête (connexion, etc)
 */
typedef struct{
  long type;
}requete_t;

/**
 * @struct response_t
 * @field type type de la réponse
 */
typedef struct{
  long type;
  int port;
  int idJOUEUR;
}reponse_t;

/*structure de stockage des positions et des infos poissons à passer au pthread create */
typedef struct{					/* Coordonnees d'une case sur la grille de simulation*/
	int y;
	int x;
	grille_t *etang;
	poisson_t *poisson;
}coord_t;

#endif /*_MESSAGE_H_*/
