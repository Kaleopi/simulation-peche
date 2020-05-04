#include <stdlib.h>     /* Pour exit, EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/socket.h> /* Pour socket */
#include <arpa/inet.h>  /* Pour IPPROTO_TCP */
#include <stdio.h>      /* Pour perror */
#include <unistd.h>     /* Pour close */
#include <string.h>     /* Pour memset */

#include "includes.h"
#include "message.h"
#include "fonctions.h"
WINDOW *fen_box_sim, *fen_box_msg, *fen_box_outils, *fen_box_points ,*fen_sim, *fen_msg, *fen_outils, *fen_points;
int *tab;
int verif;
int main(int argc, char *argv[]) {
  struct sockaddr_in adresse;
  int item_actif;
  grille_t *etang;
  int sockfd, ch=0;
  requete_t requete;
  reponse_t reponse;
  int cpt=0;
  joueur_t *client;
  /* Vérification des arguments */
  if(argc != 3) {
    fprintf(stderr, "Usage : %s adresse portServeur\n", argv[0]);
    fprintf(stderr, "Où :\n");
    fprintf(stderr, "  adresse : adresse IPv4 du serveur\n");
    fprintf(stderr, "  portServeur    : numéro de port du serveur\n");
    /*fprintf(stderr, "  message        : le message à envoyer\n");*/
    exit(EXIT_FAILURE);
  }
  etang = malloc(sizeof(grille_t));
  tab = malloc(sizeof(int)*2);
  init_etang(etang);
  item_actif = HAMMECONS;
  verif = 0;
  client=malloc(sizeof(joueur_t));

  /*
  * PARTIE UDP
  */
  /* Création de la socket */
  if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    perror("Erreur lors de la création de la socket ");
    exit(EXIT_FAILURE);
  }

  /* Création de l'adresse du serveur */
  memset(&adresse, 0, sizeof(struct sockaddr_in));
  adresse.sin_family = AF_INET;
  adresse.sin_port = htons(atoi(argv[2]));
  if(inet_pton(AF_INET, argv[1], &adresse.sin_addr) != 1) {
    perror("Erreur lors de la conversion de l'adresse ");
    exit(EXIT_FAILURE);
  }

  /* Préparation du message */
  memset(&requete, 0, sizeof(requete_t));
  requete.type = TYPE_CONNEXION;

  /* Envoi du message */
  if(sendto(sockfd, &requete, sizeof(requete), 0, (struct sockaddr*)&adresse, sizeof(struct sockaddr_in)) == -1) {
    perror("Erreur lors de la connexion au serveur ");
    exit(EXIT_FAILURE);
  }
  /* Réception confirmation de connexion */
  if(recvfrom(sockfd, &reponse, sizeof(reponse), 0, NULL, NULL) == -1) {
    perror("Erreur lors de la réception du message ");
    exit(EXIT_FAILURE);
  }
  printf("reponse.type : %ld\n",reponse.type);
  printf("reponse.port : %d\n",reponse.port);
  client->id=reponse.idJOUEUR;
  client->points=0;
  client->poireaus=0;
  /* Fermeture de la socket */
  if(close(sockfd) == -1) {
    perror("Erreur lors de la fermeture de la socket ");
    exit(EXIT_FAILURE);
  }
  printf("connecté UDP\n");

  /*
  * PARTIE TCP
  */

  /* Création de la socket de connection */
  if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
    perror("Erreur lors de la création de la socket ");
    exit(EXIT_FAILURE);
  }

  /* Remplissage de la structure */
  memset(&adresse, 0, sizeof(struct sockaddr_in));
  adresse.sin_family = AF_INET;
  adresse.sin_port = htons(reponse.port);
  if(inet_pton(AF_INET, argv[1], &adresse.sin_addr.s_addr) != 1) {
    perror("Erreur lors de la conversion de l'adresse ");
    exit(EXIT_FAILURE);
  }

  /* Connexion au serveur */
  if(connect(sockfd, (struct sockaddr*)&adresse, sizeof(adresse)) == -1) {
    perror("Erreur lors de la connexion ");
    exit(EXIT_FAILURE);
  }
  printf("Connecté au serveur en TCP !\n");
  if(read(sockfd, etang, sizeof(grille_t))==-1){
    perror("Erreur lors de la réception de la grille");
  }

  ncurses_initialiser();
  fen_box_sim = creer_fenetre_box_sim();
  fen_box_points = creer_fenetre_box_points();
  fen_box_outils = creer_fenetre_box_outils();
  fen_box_msg = creer_fenetre_box_msg();
  fen_sim = creer_fenetre_sim();
  fen_points = creer_fenetre_points();
  fen_outils = creer_fenetre_outils();
  fen_msg = creer_fenetre_msg();
  mvprintw(LINES - 1, 0, "Tapez F2 pour quitter");
  mvprintw(LINES - 3, 0, "Fleche haut pour changer d'outils vers le haut");
    mvprintw(LINES - 4, 0, "Fleche bas pour changer d'outils vers le bas");
  wprintw(fen_msg,"initialisation client verif = %d\n",verif);
  refresh();
  wprintw(fen_msg,"after refresh");
  verif = read(sockfd, etang, sizeof(grille_t));
  if(client->id==1){
    item_actif=HAMMECONSJ1;
  }else{
    item_actif=HAMMECONSJ2;
  }
  if(verif ==-1){
    perror("Erreur lors de la réception de la grille");
  }
  timeout(250);
  /*wprintw(fen_points,"Client%d",client->id);*/
  wrefresh(fen_points);
  while(verif>-1 && ch!=KEY_F(2)&&client->points<15){
    /*wprintw(fen_msg,"ifverif>%d\n", verif);*/
    verif = read(sockfd, etang, sizeof(grille_t));
    wrefresh(fen_sim);
    /*wprintw(fen_msg, "J'AI RECU LE MSG DU SERVEUR %d\n", cpt);*/
    update_sim_client(fen_sim,etang,client);
    cpt++;
    if(verif>0){
      ch = getch();
      /*wprintw(fen_msg, "verif = %d\n", verif);*/
      wrefresh(fen_msg);
      wrefresh(fen_msg);
      wrefresh(fen_sim);
      update_sim_client(fen_sim,etang,client);

      switch (ch)

      {
        case KEY_MOUSE:
          lancerTruc(item_actif,fen_sim,fen_msg,tab,etang,sockfd,client);
          mvwprintw(fen_points, 0, 0, "Client%d",client->id);
          mvwprintw(fen_points, 1, 0, "Points : %d ",client->points);
          mvwprintw(fen_points, 2, 0, "Poireaus : %d",client->poireaus);
          wrefresh(fen_msg);
          wrefresh(fen_points);

         if(write(sockfd,etang,sizeof(grille_t))==-1){

         }
        verif = read(sockfd, etang, sizeof(grille_t));

        break;
        case KEY_DOWN:
        wprintw(fen_msg, "Switch Item down\n");
        wrefresh(fen_msg);

        item_actif=switchDown(item_actif,fen_outils);
        wrefresh(fen_outils);
        break;
        case KEY_UP:
        wprintw(fen_msg, "Switch item Up\n");
        wrefresh(fen_msg);
        item_actif=switchUp(item_actif,fen_outils);
        wrefresh(fen_outils);
        break;
      }

      refresh();
      verif=0;
    }
    wrefresh(fen_msg);
  }
   if(client->points==15){
     wprintw(fen_msg,"GG\n");

     wprintw(fen_msg,"YOU\n")
     ;wprintw(fen_msg,"WIN\n");
     wprintw(fen_msg,"GET \n");
     wprintw(fen_msg,"OUT \n");
     wprintw(fen_msg,"NOW \n");
      wprintw(fen_msg,"end\n");
     wrefresh(fen_msg);
     sleep(4);

  }

   wrefresh(fen_msg);

  simulation_stopper();
  delwin(fen_box_sim);
  delwin(fen_sim);
  delwin(fen_box_msg);
  delwin(fen_msg);
  delwin(fen_box_outils);
  delwin(fen_outils);
  delwin(fen_box_points);
  delwin(fen_points);
  ncurses_stopper();

  /* Fermeture de la socket TCP*/
  if(close(sockfd) == -1) {
    perror("Erreur lors de la fermeture de la socket ");
    exit(EXIT_FAILURE);
  }

  free(etang);
  free(tab);
  free(client);
  return EXIT_SUCCESS;
}
