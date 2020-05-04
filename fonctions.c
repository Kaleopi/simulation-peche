#include <stdlib.h>		/* Pour exit, EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/socket.h> /* Pour socket */
#include <arpa/inet.h>	/* Pour sockaddr_in, IPPROTO_TCP */
#include <stdio.h>		/* Pour perror */
#include <unistd.h>		/* Pour close */
#include <string.h>		/* Pour memset */
#include <time.h>
#include <sys/time.h>
#include <errno.h>

#include "message.h"
#include "includes.h"
#include "fonctions.h"

pthread_t *threads_poissons[MAX_POISSONS+2]; /* Identifants des threads des poissons de la simulation*/
WINDOW *fen_sim;						   /* Fenetre de simulation partagee par les poissons*/
WINDOW *fen_msg;						   /* Fenetre de messages partagee par les poissons*/
case_t grille[NB_LIGNES_SIM][NB_COL_SIM];  /* Grille de simulation */
int verif=0;
int nb_poissons=0;
int timer=0;
int temptimerx=0;
int temptimery=0;

void both_send(grille_t *etang, int sock_one, int sock_two)
{

	one_send(etang, sock_one);
	one_send(etang, sock_two);
}

void one_send(grille_t *etang, int sockfd)
{
	if (write(sockfd, etang, sizeof(grille_t)) == -1)
	{
		perror("Erreur lors de l'envoi de la grille");
	}
}

void init_etang(grille_t *etang)
{
	int i, j;
	for (i = 0; i < NB_LIGNES_SIM; i++)
	{
		for (j = 0; j < NB_COL_SIM; j++)
		{
			etang->grille[i][j] = 0;
		}
	}
}

void update_sim(WINDOW *w, grille_t *etang){
	int i,j;
	for(i=0 ; i<NB_LIGNES_SIM ; i++){
		for(j=0 ; j<NB_COL_SIM ; j++){
			switch(etang->grille[i][j]){
				case VIDE:
					wattron(w, COLOR_PAIR(4));
					mvwprintw(w, i, j, " ", 4);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(4));
					break;
				case POISSON:
					wattron(w, COLOR_PAIR(1));
					mvwprintw(w, i, j, " ", 1);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(1));
				break;
				case POISSONVAL1:
					wattron(w, COLOR_PAIR(1));
					mvwprintw(w, i, j, "1", 1);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(1));
				break;
				case POISSONVAL2:
					wattron(w, COLOR_PAIR(1));
					mvwprintw(w, i, j, "2", 1);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(1));
				break;
				case POISSONVAL3:
					wattron(w, COLOR_PAIR(1));
					mvwprintw(w, i, j, "3", 1);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(1));
				break;
				case HAMMECONS:
					wattron(w, COLOR_PAIR(4));
					mvwprintw(w, i, j, "*", 4);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(4));
				break;
				case HAMMECONSJ1:
					wattron(w, COLOR_PAIR(6));
					mvwprintw(w, i, j, "*", 6);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(6));
				break;
				case HAMMECONSJ2:
					wattron(w, COLOR_PAIR(7));
					mvwprintw(w, i, j, "*", 7);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(7));
				break;
				case PNEUJ1:
					wattron(w, COLOR_PAIR(2));
					mvwprintw(w, i, j, " ", 2);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(2));
				break;
				case DYNAJ1:
					wattron(w, COLOR_PAIR(3));
					mvwprintw(w, i, j, " ", 3);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(3));
				case PNEUJ2:
					wattron(w, COLOR_PAIR(2));
					mvwprintw(w, i, j, " ", 2);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(2));
				break;
				case DYNAJ2:
					wattron(w, COLOR_PAIR(3));
					mvwprintw(w, i, j, " ", 3);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(3));
				break;
				default:
					wattron(w, COLOR_PAIR(4));
					mvwprintw(w, i, j, " ", 4);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(4));
				break;
			}
			refresh();
		}
	}
}

void update_sim_client(WINDOW *w, grille_t *etang,joueur_t *client){
	int i,j;
	if(client->id==1){
		for(i=0 ; i<NB_LIGNES_SIM ; i++){
		for(j=0 ; j<NB_COL_SIM ; j++){
			switch(etang->grille[i][j]){
				case VIDE:
					wattron(w, COLOR_PAIR(4));
					mvwprintw(w, i, j, " ", 4);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(4));
					break;
				case POISSON:
					wattron(w, COLOR_PAIR(1));
					mvwprintw(w, i, j, " ", 1);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(1));
				break;
				case POISSONVAL1:
					wattron(w, COLOR_PAIR(1));
					mvwprintw(w, i, j, "1", 1);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(1));
				break;
				case POISSONVAL2:
					wattron(w, COLOR_PAIR(1));
					mvwprintw(w, i, j, "2", 1);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(1));
				break;
				case POISSONVAL3:
					wattron(w, COLOR_PAIR(1));
					mvwprintw(w, i, j, "3", 1);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(1));
				break;
				case HAMMECONS:
					wattron(w, COLOR_PAIR(4));
					mvwprintw(w, i, j, "*", 4);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(4));
				break;
				case HAMMECONSJ1:
					wattron(w, COLOR_PAIR(6));
					mvwprintw(w, i, j, "*", 6);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(6));
				break;
				case PNEUJ1:
					wattron(w, COLOR_PAIR(2));
					mvwprintw(w, i, j, " ", 2);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(2));
				break;
				case DYNAJ1:
					wattron(w, COLOR_PAIR(3));
					mvwprintw(w, i, j, " ", 3);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(3));
				break;
				default:
					wattron(w, COLOR_PAIR(4));
					mvwprintw(w, i, j, " ", 4);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(4));
					break;
			}
			}
		}
	}
	if(client->id==2){
		for(i=0 ; i<NB_LIGNES_SIM ; i++){
		for(j=0 ; j<NB_COL_SIM ; j++){
			switch(etang->grille[i][j]){
				case VIDE:
					wattron(w, COLOR_PAIR(4));
					mvwprintw(w, i, j, " ", 4);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(4));
					break;
				case POISSON:
					wattron(w, COLOR_PAIR(1));
					mvwprintw(w, i, j, " ", 1);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(1));
				break;
				case POISSONVAL1:
					wattron(w, COLOR_PAIR(1));
					mvwprintw(w, i, j, "1", 1);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(1));
				break;
				case POISSONVAL2:
					wattron(w, COLOR_PAIR(1));
					mvwprintw(w, i, j, "2", 1);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(1));
				break;
				case POISSONVAL3:
					wattron(w, COLOR_PAIR(1));
					mvwprintw(w, i, j, "3", 1);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(1));
				break;
				case HAMMECONS:
					wattron(w, COLOR_PAIR(4));
					mvwprintw(w, i, j, "*", 4);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(4));
				break;
				case HAMMECONSJ2:
					wattron(w, COLOR_PAIR(6));
					mvwprintw(w, i, j, "*", 6);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(6));
				break;
				case PNEUJ2:
					wattron(w, COLOR_PAIR(2));
					mvwprintw(w, i, j, " ", 2);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(2));
				break;
				case DYNAJ2:
					wattron(w, COLOR_PAIR(3));
					mvwprintw(w, i, j, " ", 3);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(3));
				break;
				 default:
					wattron(w, COLOR_PAIR(4));
					mvwprintw(w, i, j, " ", 4);
					wrefresh(w);
					wattroff(w, COLOR_PAIR(4));
					break;
			}
			}
		}
	}

	refresh();


}

void afficher_etang(grille_t *etang)
{
	int i, j;
	for (i = 0; i < NB_LIGNES_SIM; i++)
	{
		for (j = 0; j < NB_COL_SIM; j++)
		{
			printf("%d", etang->grille[i][j]);
		}
		printf("\n");
	}
}

void init_sim(WINDOW *w, grille_t *etang)
{
	int i, j, val;
	for (i = 0; i < NB_LIGNES_SIM; i++)
	{
		for (j = 0; j < NB_COL_SIM; j++)
		{
			val = etang->grille[i][j];
			if (val == 0)
			{
				val = 4;
			}
			wattron(w, COLOR_PAIR(val));
			mvwprintw(w, i, j, " ", val);
			wrefresh(w);
			wattroff(w, COLOR_PAIR(val));
		}
	}
}

void ncurses_initialiser()
{
	initscr();						  /* Demarre le mode ncurses */
	cbreak();						  /* Pour les saisies clavier (desac. mise en buffer) */
	noecho();						  /* Desactive l'affichage des caracteres saisis */
	keypad(stdscr, TRUE);			  /* Active les touches specifiques */
	refresh();						  /* Met a jour l'affichage */
	curs_set(FALSE);				  /* Masque le curseur */
	mousemask(BUTTON1_CLICKED, NULL); /* Active le clic gauche de la souris*/
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_YELLOW); /*poisson*/
	init_pair(2, COLOR_WHITE, COLOR_BLACK);/*peuneu*/
	init_pair(3, COLOR_WHITE, COLOR_RED);/*dyna*/
	init_pair(4, COLOR_WHITE, COLOR_BLUE);	 /*eau*/
	init_pair(5, COLOR_WHITE, COLOR_GREEN);/*requin*/
	init_pair(6,COLOR_WHITE,COLOR_BLUE); /*hammeconqs J1*/
	init_pair(7,COLOR_BLACK,COLOR_BLUE); /*hammecon J2*/
	wbkgd(stdscr, COLOR_PAIR(2));
	refresh();
}

void ncurses_stopper()
{
	endwin();
}

void simulation_initialiser()
{
	int i, j;

	for (i = 0; i < MAX_POISSONS; i++) /* Au depart il n'y a aucune poisson dans la simulation */
		threads_poissons[i] = NULL;

	for (i = 0; i < NB_LIGNES_SIM; i++)
	{ /* Initialisation des cases de la simulation */
		for (j = 0; j < NB_COL_SIM; j++)
		{
			grille[i][j].element = VIDE;
			grille[i][j].poisson = NULL;
			pthread_mutex_init(&grille[i][j].mutex, NULL);
			pthread_cond_init(&grille[i][j].cond, NULL);
		}
	}
}

void simulation_stopper()
{
	int i;

	for (i = 0; i < MAX_POISSONS; i++)
	{
		if (threads_poissons[i] != NULL)
		{
			pthread_cancel(*threads_poissons[i]);
			threads_poissons[i] = NULL;
		}
	}
}

WINDOW *creer_fenetre_box_sim()
{
	/*Creation de la fenetre de contour de la fenetre de simulation */

	WINDOW *fen;

	fen = newwin(NB_LIGNES_SIM + 2, NB_COL_SIM + 2, 0, 0);
	box(fen, 0, 0);
	mvwprintw(fen, 0, (NB_COL_SIM + 2) / 2 - 5, "SIMULATION");
	wrefresh(fen);

	return fen;
}

WINDOW *creer_fenetre_sim()
{
	/* Creation de la fenetre de simulation dans la fenetre de contour */
	/* La simulation est affichee dans cette fenetre */

	WINDOW *fen;

	fen = newwin(NB_LIGNES_SIM, NB_COL_SIM, 1, 1);
	wrefresh(fen);
	return fen;
}

WINDOW *creer_fenetre_box_msg()
{
	/* Creation de la fenetre de contour de la fenetre de messages */

	WINDOW *fen;

	fen = newwin(NB_LIGNES_MSG + 2, NB_COL_MSG + 2, NB_LIGNES_POINTS+NB_LIGNES_OUTILS+4, NB_COL_SIM + 2);
	box(fen, 0, 0);
	mvwprintw(fen, 0, (NB_COL_MSG + 2) / 2 - 4, "DEBUG");
	wrefresh(fen);

	return fen;
}

WINDOW *creer_fenetre_msg()
{
	/* Creation de la fenetre de messages dans la fenetre de contour */
	/* Les messages indicatifs des evenements de la simulation et de l'interface */
	/* utilisateur sont affiches dans cete fenetre */

	WINDOW *fen;
	fen = newwin(NB_LIGNES_MSG, NB_COL_MSG, NB_LIGNES_POINTS+NB_LIGNES_OUTILS+5, NB_COL_SIM + 3);
	scrollok(fen, TRUE);

	return fen;
}

WINDOW *creer_fenetre_box_outils()
{
	/* Fenetre de contour de la fenetre des outils */

	WINDOW *fen;

	fen = newwin(NB_LIGNES_OUTILS + 2, NB_COL_OUTILS + 2, NB_LIGNES_POINTS + 2, NB_COL_SIM + 2);
	box(fen, 0, 0);
	mvwprintw(fen, 0, (NB_COL_OUTILS + 2) / 2 - 3, "OUTILS");
	wrefresh(fen);

	return fen;
}

WINDOW *creer_fenetre_outils()
{
	/* Creation de la fenetre des outils a l'interieur de la fenetre de contour */
	/* Les outils dans cette fenetre sont clickables, l'outil actif etant indique par un X */

	WINDOW *fen;

	fen = newwin(NB_LIGNES_OUTILS, NB_COL_OUTILS, NB_LIGNES_POINTS + 3, NB_COL_SIM + 3);
	mvwprintw(fen, 0, 1, " : Hammecon\n");
	mvwprintw(fen, 1, 1, " : Pneu (150)\n");
	mvwprintw(fen, 2, 1, " : Dynamite (200)\n");
	mvwprintw(fen, 3, 1, " : Requin (300)\n");
	mvwprintw(fen, 4, 1, " : Furtif (500)\n");
	mvwprintw(fen, 0, 0, "X");
	wrefresh(fen);

	return fen;
}

WINDOW *creer_fenetre_box_points()
{
	/* Fenetre de contour de la fenetre des outils */

	WINDOW *fen;

	fen = newwin(NB_LIGNES_POINTS + 2, NB_COL_POINTS + 2, 0, NB_COL_SIM + 2);
	box(fen, 0, 0);
	mvwprintw(fen, 0, (NB_COL_OUTILS + 2) / 2 - 3, "POINTS");
	wrefresh(fen);

	return fen;
}

WINDOW *creer_fenetre_points()
{
	/* Creation de la fenetre des outils a l'interieur de la fenetre de contour */
	/* Les outils dans cette fenetre sont clickables, l'outil actif etant indique par un X */

	WINDOW *fen;

	fen = newwin(NB_LIGNES_POINTS, NB_COL_POINTS, 1, NB_COL_SIM + 3);
	mvwprintw(fen, 0, 0, "Points : x");
	mvwprintw(fen, 1, 0, "Poireaus : x");
	wrefresh(fen);

	return fen;
}
/*test si un poisson se trouve à côté*/
int poisson_near(coord_t*coord){
	
	if(coord->etang->grille[coord->y-1][coord->x]==HAMMECONSJ1 ){
	
		verif= 1;
	}
	if(coord->etang->grille[coord->y][coord->x-1]==HAMMECONSJ1 ){
		
		verif =1;
	}
	if(coord->etang->grille[coord->y+1][coord->x]==HAMMECONSJ1){
	
		verif =1;
	}
	if(coord->etang->grille[coord->y][coord->x+1]==HAMMECONSJ1 ){
	
		verif=1;
	}
	if(coord->etang->grille[coord->y-1][coord->x]==HAMMECONSJ2 ){
	
		verif= 1;
	}
	if(coord->etang->grille[coord->y][coord->x-1]==HAMMECONSJ2 ){
	
		verif =1;
	}
	if(coord->etang->grille[coord->y+1][coord->x]==HAMMECONSJ2){
	
		verif =1;
	}
	if(coord->etang->grille[coord->y][coord->x+1]==HAMMECONSJ2 ){
	
		verif=1;
	}
	return verif;
	
		
	
	printf("!!!!!!!!!!!!!!!!!!");
}
/*thread*/
	/*routine des poissons*/
void *routine_poisson(void *arg)
{
	int pos;
	coord_t *coord = (coord_t *)arg;
	struct timeval now;
  	struct timespec timeout;
	int retcode;
	int cpt;
	int directionx;
	int directiony;
	int direction;
	
	/*int j;*/

	srand(time(NULL));
	

	gettimeofday(&now,NULL);
	timeout.tv_sec=now.tv_sec + 3;
	timeout.tv_nsec=now.tv_usec *3000;
	retcode=0;
	cpt=0;
	if(timer==3||timer==2||timer==1){
		while(timer>0){
			timer--;
			directionx=coord->x-temptimerx;
			directiony=coord->y-temptimery;
			direction=directionx+directiony;
			
			if(direction<0){
				if (coord->y + 1 < NB_LIGNES_SIM)
				{
					if (grille[coord->y + 1][coord->x].element == VIDE)
					{
						coord->etang->grille[coord->y + 1][coord->x]= coord->poisson->val;
						coord->etang->grille[coord->y ][coord->x]= VIDE;
						grille[coord->y + 1][coord->x].element = POISSON;
						grille[coord->y][coord->x].element = VIDE;
						coord->y++;
					}

				}
			}else{
				if (coord->y -1 < NB_LIGNES_SIM)
				{
					if (grille[coord->y -1 ][coord->x].element == VIDE)
					{
						coord->etang->grille[coord->y - 1][coord->x]= coord->poisson->val;
						coord->etang->grille[coord->y ][coord->x]= VIDE;
						grille[coord->y - 1][coord->x].element = POISSON;
						grille[coord->y][coord->x].element = VIDE;
						coord->y--;
					}

				}
			}

		}
	}else{
		while (1)
		{

			pthread_mutex_lock(&grille[coord->y][coord->x].mutex);
			while(poisson_near(coord)==1 ){
				coord->etang->grille[coord->y ][coord->x]= coord->poisson->val;
				update_sim(fen_sim,coord->etang);
					retcode=pthread_cond_timedwait(&grille[coord->y][coord->x].cond,&grille[coord->y][coord->x].mutex,&timeout);

				
				cpt++;
				if(cpt==3){
					verif=0;
					cpt=0;
				
					
				}
		
			
			
			}
			pthread_mutex_unlock(&grille[coord->y][coord->x].mutex);

			if(retcode==ETIMEDOUT){
				
				/*printf("timeout  %d\n",retcode);*/

			}
			/*printf("rekt %d",retcode);*/
			
			pos = rand() % 4;

			switch (pos)
			{
			case 0:
				if (coord->y + 1 < NB_LIGNES_SIM)
				{
					if (grille[coord->y + 1][coord->x].element == VIDE)
					{
						coord->etang->grille[coord->y + 1][coord->x]= coord->poisson->val;
						coord->etang->grille[coord->y ][coord->x]= VIDE;
						grille[coord->y + 1][coord->x].element = POISSON;
						grille[coord->y][coord->x].element = VIDE;
						coord->y++;
					}

				}
				break;
			case 1:
				if (coord->x + 1 < NB_COL_SIM)
				{
					if (grille[coord->y][coord->x + 1].element == VIDE)
					{
							coord->etang->grille[coord->y][coord->x+1]= coord->poisson->val;
						coord->etang->grille[coord->y ][coord->x]= VIDE;
						grille[coord->y][coord->x + 1].element = POISSON;
						grille[coord->y][coord->x].element = VIDE;
						coord->x++;
					}

				}
				break;
			case 2:
				if (coord->y - 1 > 0)
				{
					if (grille[coord->y - 1][coord->x].element == VIDE)
					{
							coord->etang->grille[coord->y -1 ][coord->x]= coord->poisson->val;
						coord->etang->grille[coord->y ][coord->x]= VIDE;
						grille[coord->y - 1][coord->x].element = POISSON;
						grille[coord->y][coord->x].element = VIDE;
						coord->y--;
					}

				}
				break;
			case 3:
				if (coord->x - 1 > 0)
				{
					if (grille[coord->y][coord->x - 1].element == VIDE)
					{
						coord->etang->grille[coord->y ][coord->x-1]= coord->poisson->val;
						coord->etang->grille[coord->y ][coord->x]= VIDE;
						grille[coord->y][coord->x - 1].element = POISSON;
						grille[coord->y][coord->x].element = VIDE;
						coord->x--;
					}
				}
				break;
			}
		sleep(1);
		wrefresh(fen_sim);
		printf("GRILLLE \n\n ------------------------\n");
    	afficher_grille();	
		}
	}
	pthread_mutex_unlock(&grille[coord->y][coord->x].mutex);
	update_sim(fen_sim,coord->etang);
	free(coord);
	return NULL;
}
/*recupere la grille*/
void recuperation(grille_t *etang){
	int i,j;
	for(i=0;i<NB_LIGNES_SIM;i++){
		for( j=0;j<NB_COL_SIM;j++){
			if(grille[i][j].element==VIDE){
				if(etang->grille[i][j]>99){
				etang->grille[i][j]=grille[i][j].element;
				}
			}

		}
	}
}


/*recupere l'etang*/
void recuperation_grille(grille_t *etang){
	int i,j;
	for(i=0;i<NB_LIGNES_SIM;i++){
		for( j=0;j<NB_COL_SIM;j++){
			grille[i][j].element=etang->grille[i][j];
		}
	}
}
/*crée les stats des poissons*/
void creer_poisson(int id, int posx, int posy,poisson_t *poisson,int random,int nb_poissons)
{



				if (random < 20)
				{
					poisson[nb_poissons].val = 300;
				}
				if (random > 20&& random <40 )
				{
					poisson[nb_poissons].val = 200;
				}
				if(random>40&&random<100)
				{
					poisson[nb_poissons].val = 100;
				}

	poisson[nb_poissons].id = nb_poissons;
	poisson[nb_poissons].posx = posx;
	poisson[nb_poissons].posy = posy;


}


/*thread*/
	/*creer les thread etles poissons*/
void generer_poisson(grille_t *etang)
{
	int randomx, randomy;
	int randomP;

	poisson_t *poisson;
	coord_t *coord;
	poisson = (poisson_t *)malloc(sizeof(poisson_t)*MAX_POISSONS);

		while (nb_poissons < MAX_POISSONS)
		{
			randomx = rand() % NB_COL_SIM/1.5+1;
			randomy = rand() % NB_LIGNES_SIM/1.5+1;
			srand(time(NULL)+randomx*randomy*randomx*randomx);
			if (etang->grille[randomy][randomx] ==0)

				{

				nb_poissons++;
				randomP = rand() % 100;



				creer_poisson(nb_poissons,randomx,randomy,poisson, randomP,nb_poissons);
				threads_poissons[nb_poissons] = (pthread_t *)malloc(sizeof(pthread_t));
				grille[randomy][randomx].element = POISSON;



				etang->grille[randomy][randomx]=poisson->val;
				grille[randomy][randomx].poisson = threads_poissons[nb_poissons];
				coord = (coord_t *)malloc(sizeof(coord_t));
				coord->y = randomy;
				coord->x = randomx;
				coord->etang=etang;

				coord->poisson=&poisson[nb_poissons];
				pthread_create(threads_poissons[nb_poissons], NULL, routine_poisson, (void *)coord);
				/* wprintw(fen_msg, "Ajout d'une poisson a la position %d %d\n", randomy - 1, randomx - 1);*/

				pthread_cond_broadcast(&grille[randomy][randomx].cond);
				pthread_mutex_unlock(&grille[randomy][randomx].mutex);


				/*wrefresh(fen_sim);
				wprintw(fen_msg, "%d %d\n", randomy, randomx);*/

				}
			}
			sleep(0.75);

}
/*change d'items*/
int switchUp(int item_actif,WINDOW *fen){
	switch(item_actif){
		case HAMMECONSJ1:
			mvwprintw(fen, 0, 0, " ");
			mvwprintw(fen, 4, 0, "X");
			return FURTIFJ1;
		break;
		case PNEUJ1:
			mvwprintw(fen, 1, 0, " ");
			mvwprintw(fen, 0, 0, "X");
			return HAMMECONSJ1;
		break;
		case DYNAJ1:
			mvwprintw(fen, 2, 0, " ");
			mvwprintw(fen, 1, 0, "X");
			return PNEUJ1;
		break;
		case FURTIFJ1:
			mvwprintw(fen, 4, 0, " ");
			mvwprintw(fen, 3, 0, "X");
			return REQUINJ1;
		break;
		case REQUINJ1:
			mvwprintw(fen, 3, 0, " ");
			mvwprintw(fen, 2, 0, "X");
			return DYNAJ1;


		case HAMMECONSJ2:
			mvwprintw(fen, 0, 0, " ");
			mvwprintw(fen, 4, 0, "X");
			return FURTIFJ2;
		break;
		case PNEUJ2:
			mvwprintw(fen, 1, 0, " ");
			mvwprintw(fen, 0, 0, "X");
			return HAMMECONSJ2;
		break;
		case DYNAJ2:
			mvwprintw(fen, 2, 0, " ");
			mvwprintw(fen, 1, 0, "X");
			return PNEUJ2;
		break;
		case FURTIFJ2:
			mvwprintw(fen, 4, 0, " ");
			mvwprintw(fen, 3, 0, "X");
			return REQUINJ2;
		break;
		case REQUINJ2:
			mvwprintw(fen, 3, 0, " ");
			mvwprintw(fen, 2, 0, "X");
			return DYNAJ2;
	}
	return item_actif;
}
/*change d'items*/
int  switchDown(int item_actif,WINDOW *fen){
		switch(item_actif){

		case HAMMECONSJ1:
			mvwprintw(fen, 0, 0, " ");
			mvwprintw(fen, 1, 0, "X");
			return PNEUJ1;
		break;
		case PNEUJ1:
			mvwprintw(fen, 1, 0, " ");
			mvwprintw(fen, 2, 0, "X");
			return DYNAJ1;
		break;
		case DYNAJ1:
			mvwprintw(fen, 2, 0, " ");
			mvwprintw(fen, 3, 0, "X");
			return REQUINJ1;
		break;
		case REQUINJ1:
			mvwprintw(fen, 3, 0, " ");
			mvwprintw(fen, 4, 0, "X");
			return FURTIFJ1;
		break;
		case FURTIFJ1:
			mvwprintw(fen, 4, 0, " ");
			mvwprintw(fen, 0, 0, "X");
			return HAMMECONSJ1;
			break;

		case HAMMECONSJ2:
			mvwprintw(fen, 0, 0, " ");
			mvwprintw(fen, 1, 0, "X");
			return PNEUJ2;
		break;
		case PNEUJ2:
			mvwprintw(fen, 1, 0, " ");
			mvwprintw(fen, 2, 0, "X");
			return DYNAJ2;
		break;
		case DYNAJ2:
			mvwprintw(fen, 2, 0, " ");
			mvwprintw(fen, 3, 0, "X");
			return REQUINJ2;
		break;
		case REQUINJ2:
			mvwprintw(fen, 3, 0, " ");
			mvwprintw(fen, 4, 0, "X");
			return FURTIFJ2;
		break;
		case FURTIFJ2:
			mvwprintw(fen, 4, 0, " ");
			mvwprintw(fen, 0, 0, "X");
			return HAMMECONSJ2;
			break;
	}
	return item_actif;
}
/*fusionne les étang cote serveur */
void fusion_etang(grille_t *etang,grille_t* etangj1,grille_t *etangj2){
int i=0,j=0;
	for(i=0;i<20;i++){
		for (j=0;j<40;j++){
			if(etang->grille[i][j]<99){
				if (etangj1->grille[i][j]<99){
					etang->grille[i][j]=etangj2->grille[i][j];

				}
				if (etangj2->grille[i][j]==0){
					etang->grille[i][j]=etangj1->grille[i][j];

				}
			}
		}
	}
}
/*fuite*/
void fuite(poisson_t *poisson){

}
/*peche*/
void peche(grille_t *etang,joueur_t * client){
	int poireaustemp;
	int pointtemp;
	poireaustemp=client->poireaus;
	pointtemp=client->points;
	if(etang->grille[client->posyHAMMECON+1][client->posxHAMMECON]>99){
		switch(etang->grille[client->posyHAMMECON+1][client->posxHAMMECON]){
			case 100:
				pointtemp+= 1;
			break;
			case 200:
				pointtemp += 2;
			break;
			case 300:
				pointtemp += 3;
			break;
		}
	
		poireaustemp+= client->poireaus+etang->grille[client->posyHAMMECON+1][client->posxHAMMECON];
		if(grille[client->posyHAMMECON+1][client->posxHAMMECON].element==POISSON){
			if(pthread_cancel(*grille[client->posyHAMMECON+1][client->posxHAMMECON].poisson)){
			
			}
		}
	}
	if(etang->grille[client->posyHAMMECON][client->posxHAMMECON+1]>99){
		switch(etang->grille[client->posyHAMMECON][client->posxHAMMECON+1]){
			case 100:
				pointtemp+= 1;
			break;
			case 200:
				pointtemp += 2;
			break;
			case 300:
				pointtemp += 3;
			break;
		}
	
		poireaustemp+= client->poireaus+etang->grille[client->posyHAMMECON][client->posxHAMMECON+1];
		if(grille[client->posyHAMMECON][client->posxHAMMECON+1].element==POISSON){
			if(pthread_cancel(*grille[client->posyHAMMECON][client->posxHAMMECON+1].poisson)){
			
			}
		}
	}

	if(etang->grille[client->posyHAMMECON-1][client->posxHAMMECON]>99){
		switch(etang->grille[client->posyHAMMECON-1][client->posxHAMMECON]){
			case 100:
				pointtemp+= 1;
			break;
			case 200:
				pointtemp += 2;
			break;
			case 300:
				pointtemp += 3;
			break;
		}
	
		poireaustemp+= client->poireaus+etang->grille[client->posyHAMMECON-1][client->posxHAMMECON];
		if(grille[client->posyHAMMECON-1][client->posxHAMMECON].element==POISSON){
			if(pthread_cancel(*grille[client->posyHAMMECON-1][client->posxHAMMECON].poisson)){
			
			}
		}
	}

	if(etang->grille[client->posyHAMMECON][client->posxHAMMECON-1]>99){
		switch(etang->grille[client->posyHAMMECON][client->posxHAMMECON-1]){
			case 100:
				pointtemp+= 1;
			break;
			case 200:
				pointtemp += 2;
			break;
			case 300:
				pointtemp += 3;
			break;
		}
	
		poireaustemp+= client->poireaus+etang->grille[client->posyHAMMECON][client->posxHAMMECON-1];
		if(grille[client->posyHAMMECON][client->posxHAMMECON-1].element==POISSON){
			if(pthread_cancel(*grille[client->posyHAMMECON][client->posxHAMMECON-1].poisson)){
			
			}
		}
	}
	client->poireaus=poireaustemp;
	client->points=pointtemp;
	poireaustemp=0;
	pointtemp=0;
}
void pecheDYNA(grille_t *etang,joueur_t *client,int posy ,int posx){
	int poireaustemp;
	int pointtemp;
	poireaustemp=client->poireaus;
	pointtemp=client->points;
	if(etang->grille[posy+1][posx]>99){
		switch(etang->grille[posy+1][posx]){
			case 100:
				pointtemp+= 1;
			break;
			case 200:
				pointtemp += 2;
			break;
			case 300:
				pointtemp += 3;
			break;
		}
	
		poireaustemp+= client->poireaus+etang->grille[posy+1][posx];
		if(grille[posy+1][posx].element==POISSON){
			if(pthread_cancel(*grille[posy+1][posx].poisson)){
			
			}
		}
	}
	if(etang->grille[posy-1][posx]>99){


		switch(etang->grille[posy-1][posx]){
			case 100:
				pointtemp+= 1;
			break;
			case 200:
				pointtemp += 2;
			break;
			case 300:
				pointtemp += 3;
			break;
			break;
		}
		poireaustemp+=  client->poireaus+etang->grille[posy-1][posx];
		if(grille[posy][posx-1].element==POISSON){
			if(pthread_cancel(*grille[posy][posx-1].poisson)){
			
			}
		}
	}
	if(etang->grille[posy][posx+1]>99){

		switch(etang->grille[posy][posx+1]){
			case 100:
				pointtemp+= 1;
			break;
			case 200:
				pointtemp += 2;
			break;
			case 300:
				pointtemp += 3;
			break;
			
		}
		poireaustemp+=  client->poireaus+etang->grille[posy][posx+1];

			if(grille[posy][posx+1].element==POISSON){
				if(pthread_cancel(*grille[posy][posx+1].poisson)){
			
				}
			}
	}
	if(etang->grille[posy][posx-1]>99){

		switch(etang->grille[posy][posx-1]){
			case 100:
				pointtemp+= 1;
			break;
			case 200:
				pointtemp += 2;
			break;
			case 300:
				pointtemp += 3;
			break;
		}
		poireaustemp+= client->poireaus+etang->grille[posy][posx-1];

		if(grille[posy][posx-1].element==POISSON){
			if(pthread_cancel(*grille[posy][posx-1].poisson)){
			
			}
		}

	}
	client->points=pointtemp;
	poireaustemp=0;
	pointtemp=0;

}


void afficher_grille(){
	int i;
	int j;
	for(i=0;i<NB_LIGNES_SIM;i++){
		for(j=0;j<NB_COL_SIM;j++){
			printf("%d",grille[i][j].element);
		}
		printf("\n");
	}
}
void suppr_hammecon(joueur_t *client,grille_t *etang){
	
	etang->grille[client->posyHAMMECON][client->posyHAMMECON]=0;
	
	etang->grille[client->posyHAMMECON][client->posyHAMMECON]=0;
	
}
/*lance un items*/
void lancerTruc(int item_actif,WINDOW *fen_sim,WINDOW *fen_msg,int* tab, grille_t* etang,int sockfd,joueur_t * client){
	MEVENT event;
	int i;
	coord_t *coord;
	int nb_hammeconj1=tab[0];
	int nb_hammeconj2=tab[1];
	int tempx = client->posxHAMMECON, tempy = client->posyHAMMECON;
	int tempx2 = client->posxHAMMECON, tempy2 = client->posyHAMMECON;



	if (getmouse(&event) == OK)
			{
				wprintw(fen_msg, "Clic a la position %d %d de l'ecran\n", event.y, event.x);
				wrefresh(fen_msg);

				if (event.y > 0 && event.y < NB_LIGNES_SIM + 1 && event.x > 0 && event.x < NB_COL_SIM + 1)
				{
					switch (item_actif)
					{
					case HAMMECONSJ1:

						if (nb_hammeconj1 < MAX_HAMMECONS)
						{
							if (etang->grille[event.y - 1][event.x - 1] == VIDE)
							{
								etang->grille[event.y - 1][event.x - 1] = HAMMECONSJ1;
								wattron(fen_sim, COLOR_PAIR(4));
								mvwprintw(fen_sim, event.y - 1, event.x - 1, "*");
								wattroff(fen_sim, COLOR_PAIR(4));
								tempx = event.x - 1;
								tempy = event.y - 1;
								nb_hammeconj1++;
								wprintw(fen_msg, "Ajout d'un Hammecon \n");
								if(client->furtif==0){
								timer=3;
								}
							}
							if (etang->grille[event.y - 1][event.x - 1] == PNEUJ2)
							{
								wprintw(fen_msg, "PRANKED!!!!!!!\n");
								wprintw(fen_msg, "have a nice 5 second sleep\n");
								sleep(5);
							}
						}
						else
						{
							

							etang->grille[tempy][tempx] = VIDE;
							wattron(fen_sim, COLOR_PAIR(4));
							mvwprintw(fen_sim, tempy, tempx, " ");
							wattroff(fen_sim, COLOR_PAIR(4));
							peche(etang,client);
							tempx = event.x - 1;
							tempy = event.y - 1;
							nb_hammeconj1 = 0;
							suppr_hammecon(client,etang);
							
							wprintw(fen_msg, "retrait\n");
							sleep(1);

						}
						wrefresh(fen_sim);
						wrefresh(fen_msg);
						/*pthread_mutex_unlock(&grille[event.y - 1][event.x - 1].element ,mutex);*/

					break;
					case HAMMECONSJ2:

						if (nb_hammeconj2 < MAX_HAMMECONS)
						{
							if (etang->grille[event.y - 1][event.x - 1] == VIDE)
							{
								etang->grille[event.y - 1][event.x - 1] = HAMMECONSJ2;
								wattron(fen_sim, COLOR_PAIR(4));
								mvwprintw(fen_sim, event.y - 1, event.x - 1, "*");
								wattroff(fen_sim, COLOR_PAIR(4));
								tempx2 = event.x - 1;
								tempy2 = event.y - 1;
								nb_hammeconj2++;
								wprintw(fen_msg, "Ajout d'un Hammecon \n");
								if(client->furtif==0){
								timer=3;
								}
							
							}
							if (etang->grille[event.y - 1][event.x - 1] == PNEUJ1)
							{
									wprintw(fen_msg, "PRANKED!!!!!!!\n");
										wprintw(fen_msg, "have a nice 5 second sleep\n");
								sleep(5);
							}
						}
						else
						{
							etang->grille[tempy2][tempx2] = VIDE;
							wattron(fen_sim, COLOR_PAIR(4));
							mvwprintw(fen_sim, tempy2, tempx2, " ");
							wattroff(fen_sim, COLOR_PAIR(4));
							tempx2 = event.x - 1;
							tempy2 = event.y - 1;
							nb_hammeconj2 = 0;
							suppr_hammecon(client,etang);

							peche(etang,client);
							wprintw(fen_msg, "retrait\n");
							sleep(1);
						}

						wrefresh(fen_sim);
						wrefresh(fen_msg);
						/*pthread_mutex_unlock(&grille[event.y - 1][event.x - 1].element ,mutex);*/

					break;

				case PNEUJ1:
					if(client->poireaus>=150){

					
							if ((etang->grille[event.y - 1][event.x - 1] == VIDE)
								&& (etang->grille[event.y  ][event.x - 1] == VIDE)
								&&(etang->grille[event.y +1][event.x - 1] == VIDE) )
							{
								etang->grille[event.y - 1][event.x - 1] = PNEUJ1;
								etang->grille[event.y ][event.x - 1] = PNEUJ1;
								etang->grille[event.y +1][event.x - 1] = PNEUJ1;
								wattron(fen_sim, COLOR_PAIR(2));
								mvwprintw(fen_sim, event.y - 1, event.x - 1, " ");
								mvwprintw(fen_sim, event.y , event.x - 1, " ");
								mvwprintw(fen_sim, event.y +1 , event.x - 1, " ");
								wattroff(fen_sim, COLOR_PAIR(2));
								wprintw(fen_msg, "Ajout d'un pneu \n");
							}
							client->poireaus-=150;
					}else{
						wprintw(fen_msg, "NO MONEY\n");
					}
					break;
				case PNEUJ2:
					if(client->poireaus>=150){

							if ((etang->grille[event.y - 1][event.x - 1] == VIDE)
								&& (etang->grille[event.y  ][event.x - 1] == VIDE)
								&&(etang->grille[event.y +1][event.x - 1] == VIDE) )
							{
								etang->grille[event.y - 1][event.x - 1] = PNEUJ2;
								etang->grille[event.y ][event.x - 1] = PNEUJ2;
								etang->grille[event.y +1][event.x - 1] = PNEUJ2;
								wattron(fen_sim, COLOR_PAIR(2));
								mvwprintw(fen_sim, event.y - 1, event.x - 1, " ");
								mvwprintw(fen_sim, event.y , event.x - 1, " ");
								mvwprintw(fen_sim, event.y +1 , event.x - 1, " ");
								wattroff(fen_sim, COLOR_PAIR(2));
								wprintw(fen_msg, "Ajout d'un pneu \n");
							
							}
							client->poireaus-=100;
					}else{
						wprintw(fen_msg, "NO MONEY\n");
					}
					break;
				case DYNAJ1:
					if(client->poireaus>=200) {
								pecheDYNA(etang,client,event.y-2,event.x-2);
								pecheDYNA(etang,client,event.y-2,event.x);
								pecheDYNA(etang,client,event.y,event.x);
								pecheDYNA(etang,client,event.y,event.x-2);
								
								etang->grille[event.y-3 ][event.x - 3] = VIDE;
								etang->grille[event.y - 2][event.x - 3] = VIDE;
								etang->grille[event.y -1][event.x - 3] = VIDE;
								etang->grille[event.y ][event.x - 3] = VIDE;
								etang->grille[event.y +1][event.x - 3]=VIDE;

								etang->grille[event.y-3 ][event.x - 2] = VIDE;
								etang->grille[event.y - 2][event.x - 2] = VIDE;
								etang->grille[event.y -1][event.x - 2] = VIDE;
								etang->grille[event.y ][event.x - 2] = VIDE;
								etang->grille[event.y +1][event.x - 2] = VIDE;

								etang->grille[event.y-3 ][event.x - 1] = VIDE;
								etang->grille[event.y - 2][event.x - 1] = VIDE;
								etang->grille[event.y -1][event.x - 1] = VIDE;
								etang->grille[event.y ][event.x - 1] = VIDE;
								etang->grille[event.y +1][event.x - 1]=VIDE;

								etang->grille[event.y-3 ][event.x] = VIDE;
								etang->grille[event.y - 2][event.x] = VIDE;
								etang->grille[event.y -1][event.x] = VIDE;
								etang->grille[event.y ][event.x] = VIDE;
								etang->grille[event.y +1][event.x ]=VIDE;

								etang->grille[event.y-3 ][event.x+1] = VIDE;
								etang->grille[event.y - 2][event.x+1] = VIDE;
								etang->grille[event.y -1][event.x+1] = VIDE;
								etang->grille[event.y ][event.x+1] = VIDE;
								etang->grille[event.y +1][event.x+1 ]=VIDE;



								wattron(fen_sim, COLOR_PAIR(3));
								mvwprintw(fen_sim, event.y -3, event.x - 3, " ");
								mvwprintw(fen_sim, event.y - 2, event.x - 3, " ");
								mvwprintw(fen_sim, event.y -1, event.x -3, " ");
								mvwprintw(fen_sim, event.y , event.x - 3, " ");
								mvwprintw(fen_sim, event.y + 1, event.x - 3, " ");

								mvwprintw(fen_sim, event.y -3, event.x - 2, " ");
								mvwprintw(fen_sim, event.y - 2, event.x - 2, " ");
								mvwprintw(fen_sim, event.y -1, event.x -2, " ");
								mvwprintw(fen_sim, event.y , event.x - 2, " ");
								mvwprintw(fen_sim, event.y + 1, event.x - 2, " ");

								mvwprintw(fen_sim, event.y -3, event.x - 1, " ");
								mvwprintw(fen_sim, event.y - 2, event.x - 1, " ");
								mvwprintw(fen_sim, event.y -1, event.x -1, " ");
								mvwprintw(fen_sim, event.y , event.x - 1, " ");
								mvwprintw(fen_sim, event.y + 1, event.x - 1, " ");

								mvwprintw(fen_sim, event.y -3, event.x , " ");
								mvwprintw(fen_sim, event.y - 2, event.x , " ");
								mvwprintw(fen_sim, event.y -1, event.x , " ");
								mvwprintw(fen_sim, event.y , event.x , " ");
								mvwprintw(fen_sim, event.y + 1, event.x , " ");

								mvwprintw(fen_sim, event.y -3, event.x +1, " ");
								mvwprintw(fen_sim, event.y - 2, event.x +1, " ");
								mvwprintw(fen_sim, event.y -1, event.x +1, " ");
								mvwprintw(fen_sim, event.y , event.x +1, " ");
								mvwprintw(fen_sim, event.y + 1, event.x +1, " ");
								wattroff(fen_sim, COLOR_PAIR(3));
								client->poireaus-=200;
					}else{
									wprintw(fen_msg, "NO MONEY\n");
					}
				case DYNAJ2:	
					if(client->poireaus>=200) {
								pecheDYNA(etang,client,event.y-2,event.x-2);
								pecheDYNA(etang,client,event.y-2,event.x);
								pecheDYNA(etang,client,event.y,event.x);
								pecheDYNA(etang,client,event.y,event.x-2);
								etang->grille[event.y-3 ][event.x - 3] = VIDE;
								etang->grille[event.y - 2][event.x - 3] = VIDE;
								etang->grille[event.y -1][event.x - 3] = VIDE;
								etang->grille[event.y ][event.x - 3] = VIDE;
								etang->grille[event.y +1][event.x - 3]=VIDE;

								etang->grille[event.y-3 ][event.x - 2] = VIDE;
								etang->grille[event.y - 2][event.x - 2] = VIDE;
								etang->grille[event.y -1][event.x - 2] = VIDE;
								etang->grille[event.y ][event.x - 2] = VIDE;
								etang->grille[event.y +1][event.x - 2] = VIDE;

								etang->grille[event.y-3 ][event.x - 1] = VIDE;
								etang->grille[event.y - 2][event.x - 1] = VIDE;
								etang->grille[event.y -1][event.x - 1] = VIDE;
								etang->grille[event.y ][event.x - 1] = VIDE;
								etang->grille[event.y +1][event.x - 1]=VIDE;

								etang->grille[event.y-3 ][event.x] = VIDE;
								etang->grille[event.y - 2][event.x] = VIDE;
								etang->grille[event.y -1][event.x] = VIDE;
								etang->grille[event.y ][event.x] = VIDE;
								etang->grille[event.y +1][event.x ]=VIDE;

								etang->grille[event.y-3 ][event.x+1] = VIDE;
								etang->grille[event.y - 2][event.x+1] = VIDE;
								etang->grille[event.y -1][event.x+1] = VIDE;
								etang->grille[event.y ][event.x+1] = VIDE;
								etang->grille[event.y +1][event.x+1 ]=VIDE;



								wattron(fen_sim, COLOR_PAIR(3));
								mvwprintw(fen_sim, event.y -3, event.x - 3, " ");
								mvwprintw(fen_sim, event.y - 2, event.x - 3, " ");
								mvwprintw(fen_sim, event.y -1, event.x -3, " ");
								mvwprintw(fen_sim, event.y , event.x - 3, " ");
								mvwprintw(fen_sim, event.y + 1, event.x - 3, " ");

								mvwprintw(fen_sim, event.y -3, event.x - 2, " ");
								mvwprintw(fen_sim, event.y - 2, event.x - 2, " ");
								mvwprintw(fen_sim, event.y -1, event.x -2, " ");
								mvwprintw(fen_sim, event.y , event.x - 2, " ");
								mvwprintw(fen_sim, event.y + 1, event.x - 2, " ");

								mvwprintw(fen_sim, event.y -3, event.x - 1, " ");
								mvwprintw(fen_sim, event.y - 2, event.x - 1, " ");
								mvwprintw(fen_sim, event.y -1, event.x -1, " ");
								mvwprintw(fen_sim, event.y , event.x - 1, " ");
								mvwprintw(fen_sim, event.y + 1, event.x - 1, " ");

								mvwprintw(fen_sim, event.y -3, event.x , " ");
								mvwprintw(fen_sim, event.y - 2, event.x , " ");
								mvwprintw(fen_sim, event.y -1, event.x , " ");
								mvwprintw(fen_sim, event.y , event.x , " ");
								mvwprintw(fen_sim, event.y + 1, event.x , " ");

								mvwprintw(fen_sim, event.y -3, event.x +1, " ");
								mvwprintw(fen_sim, event.y - 2, event.x +1, " ");
								mvwprintw(fen_sim, event.y -1, event.x +1, " ");
								mvwprintw(fen_sim, event.y , event.x +1, " ");
								mvwprintw(fen_sim, event.y + 1, event.x +1, " ");
								wattroff(fen_sim, COLOR_PAIR(3));


								wrefresh(fen_sim);
								sleep(1);




								wprintw(fen_msg, "EXPLOSION \n");
								wattron(fen_sim, COLOR_PAIR(4));
								mvwprintw(fen_sim, event.y -3, event.x - 3, " ");
								mvwprintw(fen_sim, event.y - 2, event.x - 3, " ");
								mvwprintw(fen_sim, event.y -1, event.x -3, " ");
								mvwprintw(fen_sim, event.y , event.x - 3, " ");
								mvwprintw(fen_sim, event.y + 1, event.x - 3, " ");

								mvwprintw(fen_sim, event.y -3, event.x - 2, " ");
								mvwprintw(fen_sim, event.y - 2, event.x - 2, " ");
								mvwprintw(fen_sim, event.y -1, event.x -2, " ");
								mvwprintw(fen_sim, event.y , event.x - 2, " ");
								mvwprintw(fen_sim, event.y + 1, event.x - 2, " ");

								mvwprintw(fen_sim, event.y -3, event.x - 1, " ");
								mvwprintw(fen_sim, event.y - 2, event.x - 1, " ");
								mvwprintw(fen_sim, event.y -1, event.x -1, " ");
								mvwprintw(fen_sim, event.y , event.x - 1, " ");
								mvwprintw(fen_sim, event.y + 1, event.x - 1, " ");

								mvwprintw(fen_sim, event.y -3, event.x , " ");
								mvwprintw(fen_sim, event.y - 2, event.x , " ");
								mvwprintw(fen_sim, event.y -1, event.x , " ");
								mvwprintw(fen_sim, event.y , event.x , " ");
								mvwprintw(fen_sim, event.y + 1, event.x , " ");

								mvwprintw(fen_sim, event.y -3, event.x +1, " ");
								mvwprintw(fen_sim, event.y - 2, event.x +1, " ");
								mvwprintw(fen_sim, event.y -1, event.x +1, " ");
								mvwprintw(fen_sim, event.y , event.x +1, " ");
								mvwprintw(fen_sim, event.y + 1, event.x +1, " ");
								wattroff(fen_sim, COLOR_PAIR(4));
								client->poireaus-=200;
					}else{
									wprintw(fen_msg, "NO MONEY\n");
					}
					case REQUINJ1:
						if(client->poireaus>=300) {
							if (grille[event.y - 1][event.x - 1].element == VIDE) {
									i = MAX_POISSONS;
									while (i < MAX_POISSONS+2 && threads_poissons[i] != NULL)
										i++;
									if (i < MAX_POISSONS+2) {

										threads_poissons[i] = (pthread_t *) malloc(sizeof(pthread_t));
										grille[event.y - 1][event.x - 1].element = POISSON;
										grille[event.y - 1][event.x - 1].poisson = threads_poissons[i];
										coord = (coord_t *) malloc(sizeof(coord_t));
										coord->y = event.y - 1;
										coord->x = event.x - 1;
										pthread_create(threads_poissons[i], NULL, routine_poisson, (void *) coord);
										mvwprintw(fen_sim, event.y - 1, event.x - 1, "@");
										wprintw(fen_msg, "Ajout d'une poisson a la position %d %d\n", event.y - 1, event.x - 1);
									}
									else {
										wprintw(fen_msg, "Nombre maximum de poissons atteint\n");
									}
								}	
								client->poireaus-=300;
							}else{
									wprintw(fen_msg, "NO MONEY\n");
								
					}
					
					break;
					case REQUINJ2:
							if(client->poireaus>=300) {
								if (grille[event.y - 1][event.x - 1].element == VIDE) {
									i = MAX_POISSONS;
									while (i < MAX_POISSONS+2&& threads_poissons[i] != NULL)
										i++;
									if (i <MAX_POISSONS+2) {
										threads_poissons[i] = (pthread_t *) malloc(sizeof(pthread_t));
										grille[event.y - 1][event.x - 1].element = POISSON;
										grille[event.y - 1][event.x - 1].poisson = threads_poissons[i];
										coord = (coord_t *) malloc(sizeof(coord_t));
										coord->y = event.y - 1;
										coord->x = event.x - 1;
										pthread_create(threads_poissons[i], NULL, routine_poisson, (void *) coord);
										mvwprintw(fen_sim, event.y - 1, event.x - 1, "@");
										wprintw(fen_msg, "Ajout d'une poisson a la position %d %d\n", event.y - 1, event.x - 1);
										client->poireaus-=300;
									}
									else {
										wprintw(fen_msg, "Nombre maximum de poissons atteint\n");
									}
								}	
							}else{
									wprintw(fen_msg, "NO MONEY\n");
					}
					break;
					case FURTIFJ1:
							if(client->poireaus>=500) {
								client->furtif=1;
								client->poireaus-=500;
									wprintw(fen_msg, "FURTIF ON\n");
							}else{
									wprintw(fen_msg, "NO MONEY\n");
					}
					break;
					case FURTIFJ2:
							if(client->poireaus>=500) {
								client->furtif=1;
								client->poireaus-=500;
								wprintw(fen_msg, "FURTIF ON\n");
							}else{
									wprintw(fen_msg, "NO MONEY\n");
					}
					break;
				}
			}
			}
			update_sim(fen_sim,etang);
			if(client->id==1){
				client->posxHAMMECON=tempx;
				client->posyHAMMECON=tempy;
				tab[0]=nb_hammeconj1;
			}else{
				client->posxHAMMECON=tempx2;
				client->posyHAMMECON=tempy2;
				tab[1]=nb_hammeconj2;
			}
		
			/*return tab;*/
}
