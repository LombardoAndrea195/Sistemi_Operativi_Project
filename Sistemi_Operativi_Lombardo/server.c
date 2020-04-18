//
// Created by lombi on 16/11/18.
//

/*
 * Server:
 * 1)Creare una socket
 * 2) Assegnare un local address alla socket
 * 3) Settare la socket all’ascolto
 * 4) Iterativamente:
      a. Accettare una nuova connessione
      b. Inviare e ricevere dati
      c. Chiudere la connessione
 *
 *
 */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "functionServer.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define SIZE 1024
#define SERV_PORT 5195// compreso tra 0 e 65535
#define N_CONN 10

pthread_mutex_t lucchetto_file=PTHREAD_MUTEX_INITIALIZER;
int rc=0;
pthread_mutex_t lucchetto_thread=PTHREAD_MUTEX_INITIALIZER;
struct thread_s{
    pthread_t pid;
    char * buff;
    int socket_t;
    int operazione;//risultato strtol
    bool value;
    char* ID;  //variabile che immagazina ID nel caso in cui eseguo una disdetta per confermare che il posto sia giusto,esistente!
    char *disponibili;
    char *ptr; //elemento che contiene contenuti del buff mandati dal server per strtol  e per code della prenotazione
    int q;//variabile contatore
};
void* function_server(void * s){
    struct thread_s *t =malloc(sizeof(struct thread_s*));
    if(t==NULL){
        fprintf(stderr,"Error in malloc");
        exit(-1);
    }
    t=(struct thread_s *) s;


    int n_prenotazioni_carico_utente = 0;
    nodo *head = NULL;
    t->operazione=0;
    t->q = 0;
    t->value = false;
    t->disponibili = malloc_buff(SIZE);
    t->ptr=malloc_buff(SIZE);
    t->ID = malloc_buff(SIZE);
    memset(t->buff, 0,SIZE);

    if(recv(t->socket_t, t->buff,SIZE, 0)==0){
        if (close(t->socket_t) == -1) { /* chiude la connessione */
            perror("errore in close");
            exit(-1);
        }
        free(t);
        pthread_exit(0);
    }

    printf("server: il client ha scritto\n\t%s\n",t->buff);
    memset(t->buff, 0,strlen(t->buff));
    //COSTRUISCE SALA
    rc=pthread_mutex_lock(&lucchetto_file);
    if(rc!=0) {
        printf("pthread_mutex_lock failed %d - Quit\n", rc);
        exit(EXIT_FAILURE);
    }
    head = return_list_place(head);
    rc=pthread_mutex_unlock(&lucchetto_file);
    if(rc!=0) {
        printf("pthread_mutex_unlock failed %d - Quit\n", rc);
        exit(EXIT_FAILURE);
    }
    set_lista(head);
    //stampa_mappa(head);
    sprintf(t->buff,"%s:\n","La mappa dei posti della sala è la seguente");
    strcat(t->buff, mappa_dei_posti(head));
    //STAMPA IL VALORE
    //SPEDISCE L'INTERA SALA
    send(t->socket_t, t->buff, SIZE,0);
    /* ATTENZIONE: FUNZIONI USATE PER COSTRUIRE LA MAPPA DEI POSTI DISPONIBILI -  DA USARE ALL'AVVIO DEL SERVER LA PRIMA VOLTA
            disponibili=stampa_mappa_posti_disponibili(head);
            scrittura_database(disponibili);
    */
    t->disponibili=lettura_database();
    rc=pthread_mutex_lock(&lucchetto_file);
    if(rc!=0) {
        printf("pthread_mutex_lock failed %d - Quit\n", rc);
        exit(EXIT_FAILURE);
    }
    setta_disponibilita_in_lista(head,t->disponibili);
    rc=pthread_mutex_unlock(&lucchetto_file);
    if(rc!=0) {
        printf("pthread_mutex_unlock failed %d - Quit\n", rc);
        exit(EXIT_FAILURE);
    }
    while (1) {
        printf("socket di riferimento%d\n",t->socket_t);
        //preparazione delle variabili
        memset(t->buff, 0, SIZE);


        //LEGGE COMANDO DI INIZIALIZZAZIONE
        if(recv(t->socket_t, t->buff,SIZE, 0)==0){
            if (close(t->socket_t) == -1) { /* chiude la connessione */
                perror("errore in close");
                exit(-1);
            }
            free(t);
            pthread_exit(0);
        }
        printf("%s",t->buff);
        t->operazione = (int)strtol(t->buff,NULL,10);
        //Controlla che sia un numero
        if(!isint(*t->buff)){
            fprintf(stdout,"Osserva:\n il client non mi ha mandato un numero \n");
            memset(t->buff, 0, SIZE);
            t->operazione=4;
        }
        memset(t->buff, 0, SIZE);
        rc=pthread_mutex_lock(&lucchetto_thread);
        if(rc!=0) {
            printf("pthread_mutex_lock failed %d - Quit\n", rc);
            exit(EXIT_FAILURE);
        }
        switch ( t->operazione) {
            case 1:
                rc=pthread_mutex_lock(&lucchetto_file);
                if(rc!=0) {
                    printf("pthread_mutex_lock failed %d - Quit\n", rc);
                    exit(EXIT_FAILURE);
                }
                t->disponibili=lettura_database();
                rc=pthread_mutex_unlock(&lucchetto_file);
                if(rc!=0) {
                    printf("pthread_mutex_unlock failed %d - Quit\n", rc);
                    exit(EXIT_FAILURE);
                }
                // memset(disponibili, "\0",SIZE);
                //Writeline(conn_s, disponibili, strlen(buff));
                send(t->socket_t, t->disponibili, SIZE,0);
                memset(t->disponibili, '\0',SIZE);
                break;
            case 2:
                rc=pthread_mutex_lock(&lucchetto_file);
                if(rc!=0) {
                    printf("pthread_mutex_lock failed %d - Quit\n", rc);
                    exit(EXIT_FAILURE);
                }
                t->disponibili=lettura_database();
                rc=pthread_mutex_unlock(&lucchetto_file);
                if(rc!=0) {
                    printf("pthread_mutex_unlock failed %d - Quit\n", rc);
                    exit(EXIT_FAILURE);
                }
                memset(t->buff, 0, SIZE);
                strcpy(t->buff,"Quanti posti vuoi prenotare?:\n");
                send(t->socket_t,t->buff,SIZE,0);//Manda info per gestire azioni del client
                memset(t->buff, 0, SIZE);
                //riceve una stringa che rappresneta un numero di posti selezionati
                if(recv(t->socket_t, t->buff,SIZE, 0)==0){
                    if (close(t->socket_t) == -1) { /* chiude la connessione */
                            perror("errore in close");
                            exit(-1);
                     }
                        free(t);
                        pthread_exit(0);
                }


                memset(t->ptr, 0, SIZE);//puliamo anche ptr per sicurezza
               // n_prenotazioni_carico_utente =0;
                n_prenotazioni_carico_utente=(int) strtol(t->buff, &(t->ptr), 10);
                memset(t->buff, 0, SIZE);
                strcpy(t->buff,"Caro cliente,\ndammi una lettera per rappresentare la fila, da A-F, ed un numero,da 1-9, compresi, per la selezione del posto \n");
                strcat(t->buff,"(esempio A1 C9)\n");
                send(t->socket_t,t->buff,SIZE,0);//Manda info per gestire azioni del client
                for (t->q = 0; t->q<n_prenotazioni_carico_utente; (t->q)++) {

                    //riceve una stringa che rappresneta il posto
                    if(recv(t->socket_t, t->buff,SIZE, 0)==0){
                        if (close(t->socket_t) == -1) { /* chiude la connessione */
                            perror("errore in close");
                            exit(-1);
                        }
                        free(t);
                        pthread_exit(0);
                    }
                    printf("%s\n",t->buff);
                    strcpy(t->ID,t->buff);
                    rc=pthread_mutex_lock(&lucchetto_file);
                    if(rc!=0) {
                        printf("pthread_mutex_lock failed %d - Quit\n", rc);
                        exit(EXIT_FAILURE);
                    }
                    //strcpy(prenotazioni_id_code[q],buff);
                    t->value=setta_elemento(t->buff,head);

                    if(t->value==false){
                        memset(t->buff, 0, SIZE);
                        memset(t->ptr, 0, SIZE);
                        strcpy(t->buff,t->ID);
                        //ptr=random_code();
                        t->ptr=take_code(t->ID,head); //FORMA IL CODICE

                        //memset(buff, 0, SIZE);
                        strcpy(t->buff,t->ID);
                        strcat(t->buff,": ");
                        strcat(t->buff,t->ptr);
                        strcat(t->buff,"\n");
                        printf("%s",t->buff);

                        append_scrittura_database(t->buff);
                        memset(t->disponibili,0,SIZE);
                        t->disponibili=stampa_mappa_posti_disponibili(head);
                        scrittura_database(t->disponibili);
                    }
                    else{
                        //memset(buff, 0, SIZE);
                        if(controlla_buf(t->buff,head)){
                            strcpy(t->buff,"Il posto scelto non esiste!Riprova\n");
                            (t->q)--;
                        }
                        else {
                            strcpy(t->buff, "Il posto selezionato è occupato\n");
                            (t->q)--;
                        }
                    }
                    rc=pthread_mutex_unlock(&lucchetto_file);
                    if(rc!=0) {
                        printf("pthread_mutex_unlock failed %d - Quit\n", rc);
                        exit(EXIT_FAILURE);
                    }
                    send(t->socket_t,t->buff,SIZE,0);//Manda info per gestire azioni del client

                    memset(t->buff, 0, SIZE);
                    memset(t->disponibili, '\0',SIZE);

                }
                break;
            case 3:
                recv(t->socket_t, t->buff,SIZE, 0);
                rc=pthread_mutex_lock(&lucchetto_file);
                if(rc!=0) {
                    printf("pthread_mutex_lock failed %d - Quit\n", rc);
                    exit(EXIT_FAILURE);
                }
                t->buff=controllosuFile_code(t->buff,head);
                rc=pthread_mutex_unlock(&lucchetto_file);
                if(rc!=0) {
                    printf("pthread_mutex_unlock failed %d - Quit\n", rc);
                    exit(EXIT_FAILURE);
                }
                send(t->socket_t,t->buff,SIZE,0);
                memset(t->disponibili,0,SIZE);
                rc=pthread_mutex_lock(&lucchetto_file);
                if(rc!=0) {
                    printf("pthread_mutex_lock failed %d - Quit\n", rc);
                    exit(EXIT_FAILURE);
                }
                t->disponibili=stampa_mappa_posti_disponibili(head);
                scrittura_database(t->disponibili);//aggiorna il db delle prenotazioni

                rc=pthread_mutex_unlock(&lucchetto_file);
                if(rc!=0) {
                    printf("pthread_mutex_unlock failed %d - Quit\n", rc);
                    exit(EXIT_FAILURE);
                }
                memset(t->disponibili, '\0',SIZE);
                break;
            case 4:
                fprintf(stdout, "Il sistema si sta arrestando\n");
                if (close(t->socket_t) == -1) { /* chiude la connessione */
                    perror("errore in close");
                    exit(-1);
                }
                free(t);

                rc= pthread_mutex_unlock(&lucchetto_thread);
                if(rc!=0) {
                    printf("pthread_mutex_unlock failed %d - Quit\n", rc);
                    exit(EXIT_FAILURE);
                }
                pthread_exit(0);
            default:
                fflush(stdin);
                fprintf(stdout, "Comando non valido!\n");
                fprintf(stdout, "Inserisci un comando valido- del tipo <int> -compreso tra 1-4\n");
                break;
        }
        rc=pthread_mutex_unlock(&lucchetto_thread);
        if(rc!=0) {
            printf("pthread_mutex_unlock failed %d - Quit\n", rc);
            exit(EXIT_FAILURE);
        }




    }


}
int main(int argc, char *argv[ ]) {
    //variabili per connessione
    int listen_s, conn_s;
    int sin_size;
    struct sockaddr_in servaddr;
    struct sockaddr_in clientaddr;
    //variabili da inizializzare
    if ((listen_s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) { /* crea il socket */
        perror("errore in socket\n");
        exit(-1);
    }
    memset((void *) &servaddr, 0, sizeof(servaddr));//Setto tutti i byte a zero di tale struttura
    //bzero((char *) &serv_addr, sizeof(serv_addr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    /* il server accetta connessioni su una qualunque delle sue interfacce di rete */
    servaddr.sin_port = htons(SERV_PORT); /* numero di porta del server */
    /* assegna l'indirizzo al socket */
    if (bind(listen_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("errore in bind\n");
        exit(-1);
    }/* Server inizia ad ascoltare e puo prendere fino ad un max di connessioni N_CONN in attesa di completamento,
 *       quindi ancora non acccettate dal server */
    if (listen(listen_s, N_CONN) < 0) {
        perror("errore in listen\n");
        exit(-1);
    }
    fprintf(stdout, "Il server è stato avviato alla porta 5195 ed è in ascolto\n");

    sin_size = sizeof(struct sockaddr_in);
    while (1) {
        if ((conn_s = accept(listen_s, (struct sockaddr *) &clientaddr, &sin_size)) < 0) {
            perror("errore in accept");
            exit(-1);
        }

        struct thread_s *thread_info;
        thread_info = malloc(sizeof(struct thread_s));
        if (thread_info == NULL) {
            perror("error in malloc");
            exit(EXIT_FAILURE);
        }
        thread_info->socket_t = conn_s;
        thread_info->buff = malloc_buff(SIZE);
        if(pthread_create(&(thread_info->pid), NULL, function_server, thread_info)!=0){
            perror("Error in creazione del thread");
            exit(EXIT_FAILURE);
        }
    }
}




