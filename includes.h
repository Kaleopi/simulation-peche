#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#include <ncurses.h>
#include <pthread.h>

#define NB_MSG 5
#define SIZE_MAX_MSG 256
#define MAX_PLAYERS 2
#define MAX 1024

#define NB_LIGNES_SIM		20				/* Dimensions des fenetres du programme */
#define NB_COL_SIM			40
#define NB_LIGNES_MSG		7
#define NB_COL_MSG			40
#define NB_LIGNES_OUTILS 6
#define NB_COL_OUTILS		40
#define NB_LIGNES_POINTS 3
#define NB_COL_POINTS 40

#define MAX_HAMMECONS 		1
#define MAX_POISSONS		10				/* Nombre maximum de poissons de la simulation */

#define VIDE				0				/* Identifiants des elements pouvant etre */
/* places sur la grille de simulation */
#define POISSON				1
#define HAMMECONS              2
#define PNEU                   3
#define DYNA                    4
#define FURTIF              6
#define REQUIN                  5

#define HAMMECONSJ1	        10
#define PNEUJ1              11
#define DYNAJ1              12
#define REQUINJ1 13
#define FURTIFJ1 14


#define HAMMECONSJ2         20
#define PNEUJ2              21
#define DYNAJ2              22
#define REQUINJ2 23
#define FURTIFJ2 24

#define COOL                1
#define FUITE               0

#define FURTIFON            1
#define FURTIFOFF           0

#define POISSONVAL1       100
#define POISSONVAL2       200
#define POISSONVAL3       300
#endif
