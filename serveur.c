#include <stdlib.h>     /* Pour exit, EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/socket.h> /* Pour socket */
#include <sys/time.h>
#include <arpa/inet.h>  /* Pour sockaddr_in, IPPROTO_TCP */
#include <stdio.h>      /* Pour perror */
#include <unistd.h>     /* Pour close */
#include <string.h>     /* Pour memset */
#include <time.h>
#include <errno.h>
#include <ncurses.h>
#include <pthread.h>
#include <fcntl.h>

#include "fonctions.h"
#include "includes.h"
#include "message.h"


int main(int argc, char *argv[]) {

  grille_t *etang;
  grille_t *etangj1;
  grille_t *etangj2;
  struct sockaddr_in adresse;
  struct sockaddr p1, p2;
  struct timeval tour;
  /*size_t taille;*/
  requete_t requete;
  reponse_t reponse;
  socklen_t len = sizeof(struct sockaddr_in);
  socklen_t p_len = sizeof(struct sockaddr);
  int sockfdTCP, sockfdUDP, sock_one, sock_two, test, verif, maxFD;
  fd_set set;

  /*char *msg, *msgenvoi;
  msgenvoi = "";*/

  /* Vérification des arguments */
  if(argc != 2) {
    fprintf(stderr, "Usage : %s port\n", argv[0]);
    fprintf(stderr, "Où :");
    fprintf(stderr, "\n\tport : le numéro de port d'écoute du serveur\n");
    exit(EXIT_FAILURE);
  }
  etang = malloc(sizeof(grille_t));
  etangj1 = malloc(sizeof(grille_t));
  etangj2 = malloc(sizeof(grille_t));
  init_etang(etang);
  init_etang(etangj1);
  init_etang(etangj2);

  afficher_etang(etang);
  test = 0;
  verif = 0;
  /*
   *
   * PARTIE UDP
   *
   */
   /* Création de la socket */
   if((sockfdUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
     perror("Erreur lors de la création de la socket ");
     exit(EXIT_FAILURE);
   }

   /* Création de l'adresse du serveur */
   memset(&adresse, 0, sizeof(struct sockaddr_in));
   adresse.sin_family = AF_INET;
   adresse.sin_port = htons(atoi(argv[1]));
   adresse.sin_addr.s_addr = htonl(INADDR_ANY);

   /* Nommage de la socket */
   if(bind(sockfdUDP, (struct sockaddr*)&adresse, sizeof(struct sockaddr_in)) == -1) {
     perror("Erreur lors du nommage de la socket ");
     exit(EXIT_FAILURE);
   }

   if(getsockname(sockfdUDP,(struct sockaddr*)&adresse,&len)==-1){
     perror("Erreur getsockname");
     exit(EXIT_FAILURE);
   }

   /* Attente de la requête du joueur 1*/
   printf("Serveur en attente de la connexion du joueur 1.\n");
   if(recvfrom(sockfdUDP, &requete, sizeof(requete), 0, (struct sockaddr*)&p1, &p_len) == -1) {
     perror("Erreur lors de la réception du message ");
     exit(EXIT_FAILURE);
   }

   /* Attente de la requête du joueur 2*/
   printf("Serveur en attente de la connexion du joueur 2\n");
   if(recvfrom(sockfdUDP, &requete, sizeof(requete), 0, (struct sockaddr*)&p2, &p_len) == -1) {
     perror("Erreur lors de la réception du message ");
     exit(EXIT_FAILURE);
   }

  /*
  *
  * PARTIE TCP
  *
  */

  /* Création de la socket */
  if((sockfdTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
    perror("Erreur lors de la création de la socket ");
    exit(EXIT_FAILURE);
  }

  /* Création de l'adresse du serveur */
  memset(&adresse, 0, sizeof(struct sockaddr_in));
  adresse.sin_family = AF_INET;
  adresse.sin_addr.s_addr = htonl(INADDR_ANY);
  adresse.sin_port = htons(0);

  /* Nommage de la socket */
  if(bind(sockfdTCP, (struct sockaddr*)&adresse, sizeof(struct sockaddr_in)) == -1) {
    perror("createTCPServerSock : Erreur lors du nommage de la socket ");
    exit(EXIT_FAILURE);
  }

  if(getsockname(sockfdTCP,(struct sockaddr*)&adresse,&len)==-1){
    perror("Erreur getsockname");
    exit(EXIT_FAILURE);
  }

  memset(&reponse,0,sizeof(reponse));
  reponse.type = TYPE_CONNEXION_AUTHORIZED;
  reponse.port = ntohs(adresse.sin_port);
  printf("\nreponse.port : %d\n",reponse.port);
  /* 1ere connexion autorisée */
  reponse.idJOUEUR=1;
  if(sendto(sockfdUDP, &reponse, sizeof(reponse), 0, (struct sockaddr*)&p1, p_len) == -1) {
    perror("Erreur lors de la reponse 1 ");
    exit(EXIT_FAILURE);
  }
  /* 2ere connexion autorisée */
    reponse.idJOUEUR=2;
  if(sendto(sockfdUDP, &reponse, sizeof(reponse), 0, (struct sockaddr*)&p2, p_len) == -1) {
    perror("Erreur lors de la reponse 2 ");
    exit(EXIT_FAILURE);
  }
  /* Fermeture de la socket UDP*/
  if(close(sockfdUDP) == -1) {
    perror("Erreur lors de la fermeture de la socket ");
    exit(EXIT_FAILURE);
  }

  /* Mise en mode passif de la socket */
  if(listen(sockfdTCP, 1) == -1) {
    perror("Erreur lors de la mise en mode passif ");
    exit(EXIT_FAILURE);
  }

  /* Attente d'une connexion */
  if((sock_one = accept(sockfdTCP, NULL, NULL)) == -1) {
    perror("Erreur lors de la demande de connexion TCP du joueur 1");
    exit(EXIT_FAILURE);
  }
  printf("Joueur 1 connecté TCP !\n");
  if((sock_two = accept(sockfdTCP, NULL, NULL)) == -1) {
    perror("Erreur lors de la demande de connexion TCP du joueur 2");
    exit(EXIT_FAILURE);
  }
  printf("Joueur 2 connecté TCP !\n");

  if(sock_one > sock_two) maxFD = sock_one;
  else maxFD = sock_two;
  afficher_etang(etang);
  printf("\n\n");
  both_send(etang, sock_one, sock_two);

  while(sock_one > -1 || sock_two > -1){
    simulation_initialiser();
    printf("ETANG \n\n ------------------------\n");
    afficher_etang(etang);

     FD_ZERO(&set);
    FD_SET(sock_one, &set);
     FD_SET(sock_two, &set);
    generer_poisson(etang);
    tour.tv_sec = 1;
    tour.tv_usec = 0;



    if((verif = select(maxFD+1, &set, NULL, NULL, &tour)) == -1){
      if(errno != EINTR){
        perror("Serveur : erreur select");
        exit(EXIT_FAILURE);
      }
    }
    if(FD_ISSET(sock_one, &set)){
       if(read(sock_one, etangj1, sizeof(grille_t) )==-1) {
         if(errno != EINTR){
           perror("Serveur : erreur select");
           exit(EXIT_FAILURE);
         }
       }else
         both_send(etang, sock_one, sock_two);
       printf("SOCK_ONE verif serveur 183 = %d\n",verif);
    }
    if(FD_ISSET(sock_two, &set)){
      if(read(sock_two, etangj2, sizeof(grille_t))==-1){

          fusion_etang(etang,etangj1,etangj2);
          recuperation(etang);
        if(errno != EINTR){
          perror("Serveur : erreur select");
          exit(EXIT_FAILURE);
        }
      }else
        both_send(etang, sock_one, sock_two);
     printf("SOCK_TWO serveur 192 = %d\n",sock_one);
    }

      fusion_etang(etang,etangj1,etangj2);
      recuperation(etang);
    both_send(etang, sock_one, sock_two);
    printf("sockone serveur 201 = %d\n",sock_two);
    printf("while %d\n",test);

    test++;
  }

  /* Fermeture des sockets */
  if(close(sock_one) == -1) {
    perror("Erreur lors de la fermeture de la socket de communication ");
    exit(EXIT_FAILURE);
  }
  if(close(sockfdTCP) == -1) {
    perror("Erreur lors de la fermeture de la socket de connexion ");
    exit(EXIT_FAILURE);
  }

  /*début de la sim*/
  /*
  ();*/
  simulation_stopper();
  free(etang);
  free(etangj1);
  free(etangj2);
  printf("Serveur terminé.\n");

  return EXIT_SUCCESS;
}
