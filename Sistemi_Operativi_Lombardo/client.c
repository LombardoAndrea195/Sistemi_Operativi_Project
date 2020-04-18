#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <zconf.h>
#include "help.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define fflush(stdin) while (getchar()!='\n')
#define SERV_PORT 5195 //Numero di porta scelto

#define SIZE 1024
#define TIMER 25
#ifdef FLUSH
#define fflush(stdin) while(getchar() != '\n')
#endif
/*
 * Funzione usata per settare quanti posti quel client vuole prenotare:
 * Aspetta uno stdin per il numero di posti e con una sprintf lo immagazina in un buff per mandarlo al server,
 * ritorna il numero di elementi
 */

int selezione_posti(int sock_fd) {
    char *input_2;
    int q;
    input_2 = malloc_buff(SIZE);
    LABEL1:
    if(fscanf(stdin,"%d",&q)==0){
        //In caso di errore pulisco lo stdin altrimenti il valore rimane all'interno,stampo tipo di errore e ricomincio da capo
        fflush(stdin);
        fprintf(stdout, "Comando non valido!\n");
        fprintf(stdout, "Inserisci un comando valido- del tipo <int> -compreso tra 1-4\n");
        goto LABEL1;
    }
    sprintf(input_2,"%d",q);
    send(sock_fd,input_2,4,0);//manda il numero di quanti posti sta selezionando
    memset(input_2,0,SIZE);
    free(input_2);
    return q;
}


/*
 *
 *   Client:
 *   1. Creare una socket
 *   2. Connettersi al server
 *   3. Inviare e ricevere dati
 *   4. Chiudere la connessione
 */

void client_inattivo(int sig){
    if(sig==SIGALRM){
        puts("Attenzione: a causa di inattività del sistema sto per chiudere la connessione");
        exit(0);

    }
}

void Gestione_Segnale(int sig){
    if(sig==SIGINT){
        puts("Attenzione:sto per chiudere la connessione con il server perchè ho ricevuto un segnale di SIGINT");
        exit(0);

    }
}
int main(int argc, char *argv[]) {
    if(argc!=2){
        perror("Error in numero di elementi passati");
    }
    signal(SIGINT,Gestione_Segnale);
    signal(SIGALRM,client_inattivo);
    //Inizializzazione variabili
    fprintf(stdout,"Progetto Sistema Operativi !\n-----------------------------------------------------------\n");
    fprintf(stdout,"Sistema di prenotazione posti per una sala cinematografica!\n-----------------------------------------------------------\n");
    fprintf(stdout,"Lato CLIENT:\n");
    int q,i;//variabili contatore
    int opzione_scelta=0; //variabile che mi permette di capire che scelta devo fare
    char* prenotazioni_with_code;
    int sock_fd;
    char*buffer_invio;
    struct sockaddr_in addr;
    char *buffer_inricezione;


    if ((sock_fd = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP)) < 0) { /*   crea il socket
                                                                         AF_INET perche via remoto
                                                                         SOCK_STREAM perchè tramite TCP
                                                                         per maggior sicurezza, come tipo di protocollo di comunicazione
                                                                    */
        perror("errore in socket\n");
        exit(-1);
    }
    memset((void *)&addr, 0, sizeof(addr)); /* azzera servaddr */
    addr.sin_family = AF_INET; /* assegna il tipo di indirizzo */
    addr.sin_port = htons(SERV_PORT); /* assegna la porta del server inizializzata come macro */
    if (inet_pton(AF_INET,argv[1], &(addr.sin_addr)) <= 0) {  // inet_pton serve per convertire in una struttura di indirizzi network(IP)
        perror("errore in inet_pton\n");
        exit(-1);
    }
    /* stabilisce la connessione con il server */
    if (connect(sock_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("errore in connect\n");
        exit(-1);
    }
    //ALLOCAZIONE_BUFFER
    buffer_invio=malloc_buff(SIZE);
    buffer_inricezione=malloc_buff(SIZE);
    prenotazioni_with_code=malloc_buff(SIZE);


    memset(buffer_invio,0,SIZE);
    fprintf(stdout,"Manda un saluto al server e attendi una risposta per iniziare la comunicazione:\n");
    /*
     * Prendo da stdin e recapito in buffer_invio quel che leggo che deve essere al piu un num di byte pari a SIZE
     * e sostituisco l'ultimo valore del buffer attraverso un terminatore di stringa invece che '\n'
     */
    alarm(TIMER);
    if(fgets(buffer_invio,SIZE,stdin)==NULL){
        perror("Errore nell'fgets\n");
        exit(EXIT_FAILURE);
    }

    alarm(0);
    int len=(int)strlen(buffer_invio);
    if(buffer_invio[len-1]=='\n'){
        buffer_invio[len-1]='\0';
    }
    send(sock_fd,buffer_invio,SIZE,0); //funzione per  mandare l'intero buffer
    memset(buffer_inricezione,0,SIZE);
    recv(sock_fd, buffer_inricezione,SIZE, 0);
    fprintf(stdout,"%s\n", buffer_inricezione);
    STEP1:
    memset(buffer_invio,0,SIZE);
    fprintf(stdout,"\n-----------------------------------------------------------\nScegli tra le operazioni disponibili:\n-----------------------------------------------------------\n");
    fprintf(stdout,"1-Vedere disponibilità nella mappa\n-----------------------------------------------------------\n");
    fprintf(stdout,"2-Inviare i posti che si intende prenotare\n-----------------------------------------------------------\n");
    fprintf(stdout,"3-Disdire una prenotazione per cui si possiede il codice\n-----------------------------------------------------------\n");
    fprintf(stdout,"4-Quit\n-----------------------------------------------------------\n");
    fprintf(stdout,"\n Inserisci un numero che rappresenta un'operazione sopra indicata: \t");

    alarm(TIMER);
    if(fgets(buffer_invio,SIZE,stdin)==NULL){
        perror("Errore nell'fgets\n");
        exit(EXIT_FAILURE);
    }
    alarm(0);
    len=(int)strlen(buffer_invio);
    if(buffer_invio[len-1]=='\n'){
        buffer_invio[len-1]='\0';
    }
    opzione_scelta=(int)strtol(buffer_invio,NULL,10);
    /*if(fscanf(stdin, "%d", &opzione_scelta)==0){
        //In caso di errore pulisco lo stdin altrimenti il valore rimane all'interno,stampo tipo di errore e ricomincio da capo
        fflush(stdin);
        fprintf(stdout, "Comando non valido!\n");
        fprintf(stdout, "Inserisci un comando valido- del tipo <int> -compreso tra 1-4\n");
        goto STEP1;
    }*/
    if(!isint(*buffer_invio)){
        fprintf(stdout,"OSSERVA\n Non hai inserito un numero come da manuale\n");
        fprintf(stdout,"Premi INVIO PER CONTINUARE!\n");
        alarm(TIMER);
        fflush(stdin);
        alarm(0);
        goto STEP1;
    }
    memset(buffer_inricezione,0,SIZE);
    sprintf(buffer_invio,"%d\n",opzione_scelta);//setto qualsiasi sia il valore nel buffer_invio e poi lo spedisco

    send(sock_fd,buffer_invio,SIZE,0);//manda informazione(operzione_scelta) al server e si mette in ricezione di un buff
    switch (opzione_scelta) {
        case 1:
            //Stampa le mappa della sala,tenendo conto delle disponibilità e indisponibilità dei posti
            memset(buffer_inricezione,0,SIZE);
            recv(sock_fd, buffer_inricezione,SIZE, 0);//ricevo e stampo le disponibilità
            fprintf(stdout,"%s\n", buffer_inricezione);
            break;
            /*
             * Inviare al server l'elenco dei posti che si intende prenotare (ciascun posto da prenotare
             * viene ancora identificato tramite numero di fila e numero di poltrona).
             */
        case 2:
            memset(buffer_inricezione,0,SIZE);
            recv(sock_fd, buffer_inricezione,SIZE, 0);//riceve dal server :("Quanti posti vuoi prenotare?:\n")
            fprintf(stdout,"%s",buffer_inricezione);
            alarm(TIMER);
            q = selezione_posti(sock_fd);
            alarm(0);
            i=0;
            memset(buffer_invio,0,SIZE);
            recv(sock_fd, buffer_invio,SIZE, 0);//ricevo un qualcosa del tipo:Ok ora verifico se nel database sono gia occupati i posti
            fprintf(stdout,"%s\n",buffer_invio);
            while(i<q){ //ripete tale struttura tante volte quante richieste in selezione_posti
                memset(buffer_invio,0,SIZE);
                memset(buffer_inricezione,0,SIZE);
                if(strcmp(buffer_invio,"")==0){
                    strcpy(buffer_invio,"Inserisci il nuovo posto:");
                    printf("%s\t",buffer_invio);
                }
                memset(buffer_invio,0,SIZE);
                STEP2:
                alarm(TIMER);
                if(fscanf(stdin, "%s",buffer_invio)==0){
                    //In caso di errore pulisco lo stdin altrimenti il valore rimane all'interno,stampo tipo di errore e ricomincio da capo
                    fflush(stdin);
                    fprintf(stdout, "Comando non valido!\n");
                    fprintf(stdout, "Inserisci un posto ad esempio A1-B2-C4, lettere disponibili [A-F] , mentre i posti [1-9] \n");
                    goto STEP2;
                }
                alarm(0);
                send(sock_fd,buffer_invio,SIZE,0);//manda il posto
                recv(sock_fd, buffer_inricezione,SIZE, 0);//riceve codice di prenotazione
                if(strcmp(buffer_inricezione,"Il posto selezionato è occupato\n")==0){
                    fprintf(stdout,"%s\nSi fa riferimento al posto numero %s\n",buffer_inricezione,buffer_invio);
                    i--;
                }
                else if(strcmp(buffer_inricezione,"Il posto scelto non esiste!Riprova\n")==0){
                    fprintf(stdout,"%s\nSi fa riferimento al posto numero %s\n",buffer_inricezione,buffer_invio);
                    i--;
                }
                else{
                    fprintf(stdout,"ATTENZIONE:Conservare il codice in caso di disdetta\n");
                    fprintf(stdout,"%s\n",buffer_inricezione);
                    strcpy(prenotazioni_with_code,buffer_inricezione);
                    strcat(prenotazioni_with_code," ");

                }
                memset(buffer_inricezione,0,SIZE);
                i++;
            }
            fflush(stdin);
            break;
        case 3:

            memset(buffer_invio,0,SIZE);
            fprintf(stdout,"Inserisci il valore di un posto e di un codice nella seguente forma:\n (esempio:\tA1: Z7M0H7L3U7P2P2)\n");
        STEP3:
            alarm(TIMER);
            if(fscanf(stdin," %[^\n]",buffer_invio)==0){
                fflush(stdin);
                fprintf(stdout, "Comando non valido!\n");
                goto STEP3;
            }
            alarm(0);

            printf("Il codice che sto mandando al server è il seguente:\t%s\n",buffer_invio);
            send(sock_fd,buffer_invio,SIZE,0);
            memset(buffer_inricezione,0,SIZE);
            recv(sock_fd, buffer_inricezione,SIZE, 0);//riceve codice di prenotazione
            if(strcmp(buffer_inricezione,"Cancellata prenotazione con successo!\n")==0){
                printf("%s",buffer_inricezione);
            }
            else if(strcmp(buffer_inricezione,"Errore nell'elemento scritto\n")==0){
                printf("%s",buffer_inricezione);
            }
            else if(strcmp(buffer_inricezione,"Errore nel codice riferito a quel posto!\n")==0){
                printf("%s",buffer_inricezione);
            }
            fflush(stdin);
            break;

        case 4:
            fprintf(stdout, "Il sistema si sta arrestando\n");
            if (close(sock_fd) == -1) { /* chiude la connessione */
                perror("errore in close\n");
                exit(-1);
            }
            exit(0);
        default:

            fprintf(stdout, "Comando non valido!\n");
            fprintf(stdout, "Inserisci un comando valido- del tipo <int> -compreso tra 1-4\n");
            break;
    }
    goto STEP1;
    return 0;
}