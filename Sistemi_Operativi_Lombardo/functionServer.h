//
// Created by lombi on 05/01/19.
//

#ifndef SISTEMI_OPERATIVI_LOMBARDO_FUNCTIONSERVER_C_H
#define SISTEMI_OPERATIVI_LOMBARDO_FUNCTIONSERVER_C_H

#endif //SISTEMI_OPERATIVI_LOMBARDO_FUNCTIONSERVER_C_H

// Created by lombi on 18/11/18.
//
#include <printf.h>
#include <stdio.h>
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


#define  SIZE 1024
/*
 * Mi permette di creare un codice randomico alfanumerico, di 14 elementi attraverso la funzione srand che deve includere time
 * alterna una lettera e un numero!
 */
char * random_code ()
{
    char *array;
    int i;
    int n_seat;
    srand (time(NULL));
    array=malloc_buff(SIZE);
    for (i=0; i<14;i++ ) {
        if(i%2==0){
            array[i] = (char) ((rand() % 26) + 65);
        }
        else if(i%2==1){
            n_seat=rand()%10;
            sprintf(&array[i],"%d", n_seat);

        }
    }
    return array;
}

/*
 * Legge l'intero file, che è il database di tutte le prenotazioni fatte e ritorna un buffer che contiene tutte le informazioni
 */

char * lettura_database_prenotazioni(){
    FILE* fd;
    char* disponibili;
    disponibili=malloc_buff(SIZE);
    errno=0;
    fd=fopen("database_prenotazioni.txt","r+");
    if(errno!=0){
        fprintf(stderr,"errore nell'apertura del file\n");
        exit(-1);
    }
    fread(disponibili,sizeof(char),SIZE,fd);
    if(fclose(fd)){
        fprintf(stderr,"errore nella chiusura\n");
        exit(-1);
    }
    return disponibili;
}
/*
 * Funzione che ritorna la grandezza del file dato input
 */
int value_size_file(char * path){
    FILE* fd;
    char* disponibili;
    int size_file;
    disponibili=malloc_buff(SIZE);
    errno=0;
    fd=fopen(path,"r+");
    if(errno!=0){
        fprintf(stderr,"errore nell'apertura del file\n");
        exit(-1);
    }
    fread(disponibili,sizeof(char),SIZE,fd);
    fseek(fd, 0, SEEK_END);
    size_file=(int)ftell(fd);
    rewind(fd);
    if(fclose(fd)){
        fprintf(stderr,"errore nella chiusura\n");
        exit(-1);
    }
    return size_file;
}
/*
 * Legge dal file un numero di byte pari a base, intero passato da input!
 *
 */
char* leggi_da_file(int base){
    FILE* fd;
    char* disponibili;
    disponibili=malloc_buff(SIZE);
    errno=0;
    fd=fopen("database_prenotazioni.txt","r+");
    if(errno!=0){
        fprintf(stderr,"errore nell'apertura del file\n");
        exit(-1);
    }
    fread(disponibili,sizeof(char),base,fd);
    if(fclose(fd)){
        fprintf(stderr,"errore nella chiusura\n");
        exit(-1);
    }
    return disponibili;
}
/*
 * Aggiorna il file responsabile del database delle prenotazioni
 */
void aggiorna_database(char* disponibili){
    FILE* fd;

    errno=0;
    fd=fopen("database_prenotazioni.txt","w+");
    if(errno!=0){
        fprintf(stderr,"errore nell'apertura del file\n");
        exit(-1);
    }
    fprintf(fd,"%s",disponibili);

    //fwrite(disponibili,sizeof(char),strlen(disponibili),fd);
    if(fclose(fd)){
        fprintf(stderr,"errore nella chiusura\n");
        exit(-1);
    }
    return;
}
/*
 * Aggiunge un buffer all'interno del file tramite un' append
 */
void append_scrittura_database(char* disponibili){
    FILE* fd;

    errno=0;
    fd=fopen("database_prenotazioni.txt","a+");
    if(errno!=0){
        fprintf(stderr,"errore nell'apertura del file\n");
        exit(-1);
    }fprintf(fd,"%s",disponibili);
    //fwrite(disponibili,sizeof(char),strlen(disponibili),fd);
    if(fclose(fd)){
        fprintf(stderr,"errore nella chiusura\n");
        exit(-1);
    }
    return;
}/*
 * Scrive nella mappa delle disponibilità aggiornandola
 */
void scrittura_database(char* disponibili){
    FILE* fd;

    errno=0;
    fd=fopen("mappa_sala_disponibilita.txt","w+");
    if(errno!=0){
        fprintf(stderr,"errore nell'apertura del file\n");
        exit(-1);
    }
    fprintf(fd,"%s",disponibili);
    fprintf(stdout,"scritto con successo\n");
    if(fclose(fd)){
        fprintf(stderr,"errore nella chiusura\n");
        exit(-1);
    }
    return;
}/*
 *
 * Legge la mappa del db
 */
char * lettura_database(){
    FILE* fd;
    char* disponibili;
    disponibili=malloc_buff(SIZE);
    errno=0;
    fd=fopen("mappa_sala_disponibilita.txt","r+");
    if(errno!=0){
        fprintf(stderr,"errore nell'apertura del file\n");
        exit(-1);
    }
    fread(disponibili,sizeof(char),SIZE,fd);
    if(fclose(fd)){
        fprintf(stderr,"errore nella chiusura\n");
        exit(-1);
    }
    return disponibili;
}
/*
 * operazione che inizializzerà un file di testo che manterrà le informazioni su
 * quali sono i posti della sala, posizione e mappa
 * verra' utilizzata di base per costruire una  lista di collegata di elementi
 */
void define_map(void){
    int fd;
    char *buf=malloc_buff(SIZE);
    fd=apertura_file("mappa_posti.txt",O_RDWR|O_CREAT|O_TRUNC);
    int len;
    strcpy(buf,"A1 A2 A3 A4 A5 A6 A7 A8 A9\n");
    strcat(buf,"B1 B2 B3 B4 B5 B6 B7 B8 B9\n");
    strcat(buf,"C1 C2 C3 C4 C5 C6 C7 C8 C9\n");
    strcat(buf,"D1 D2 D3 D4 D5 D6 D7 D8 D9\n");
    strcat(buf,"E1 E2 E3 E4 E5 E6 E7 E8 E9\n");
    strcat(buf,"F1 F2 F3 F4 F5 F6 F7 F8 F9\n");
    len=(int)strlen(buf);
    write(fd,buf,(size_t )len);

    close(fd);
    free(buf);
}
/*
 * Funzione che mi occorre per costruire la mia lista nella maniera adeguata e inizializzare la lista
 */

nodo* aggiungi_in_coda(nodo* head,nodo* new){
    if(head==NULL){
        return new;
    }
    else {
        nodo *p;
        p = malloc(sizeof(nodo));
        if (p == NULL) {
            fprintf(stderr, "Errore nella malloc\n");
            exit(EXIT_FAILURE);
        }
        for(p=head;p->next!=NULL;p=p->next);
        p->next=new;
        new->next=NULL;
        return head;
    }
}
/*
 * Funzione che definisce il file di mappa_posti e poi forma un char ** dove ogni singolo elemento punta ad un'area di memoria
 * che contiene la riga quindi il primo elemento contiene l'indirizzo di un'area di memoria pari a =A1 A2 A3 A4 A5 A6.. fino A9
 * il secondo elemento punta ad un'area di memoria che contiene la fila di B e cosi via dicendo, questa è la rappresentazione grafica
 * di quello che ho dove.
 *
 *
 *                   RITORNO:                             char**  array_row
 *
 *                                                     +-------------------------------+
 *                                                     | A1 A2 A3 A4 A5 A6 A7 A8 A9    |  array_row[0]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *                                                     +-------------------------------+
 *                                                     | B1 B2 B3 B4 B5 B6 B7 B8 B9    |  array_row[1]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *                                                     +-------------------------------+
 *                                                     | C1 C2 C3 C4 C5 C6 C7 C8 C9    |  array_row[2]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *                                                     +-------------------------------+
 *                                                     | D1 D2 D3 D4 D5 D6 D7 D8 D9    |  array_row[3]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *                                                     +-------------------------------+
 *                                                     | E1 E2 E3 E4 E5 E6 E7 E8 E9    |  array_row[4]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *                                                     +-------------------------------+
 *                                                     | F1 F2 F3 F4 F5 A6 F7 F8 F9    |  array_row[5]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *                                                     +-------------------------------+
 *                                                     |            NULL               |  array_row[6]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *
 */
char ** define_seat() {
    char *buffer_mappa;
    int fd;
    fd=open("mappa_posti.txt",O_CREAT|O_TRUNC|O_RDWR,0666);
    buffer_mappa=malloc_buff(SIZE);
    define_map();
    read(fd,buffer_mappa,SIZE);
    close(fd);
    char**array_row;
    array_row=malloc(sizeof(char*));
    if(array_row==NULL){
        fprintf(stderr,"error in malloc\n");
        exit(EXIT_FAILURE);
    }
    array_row=tokenize_elem(buffer_mappa,"\n");

    return array_row;
}

/*
 * Funzione che definisce il file di mappa_posti e poi forma un char ** dove ogni singolo elemento punta ad un'area di memoria
 * che contiene il singolo elemento esempio A1 ecc A2. Attraverso una lista collegata di posti mi creo una lista di struct biglietto
 * ridenominato come nodo con i seguenti parametri( occupato,*next,n_posto,n_riga,*code,id[3]) dove ad id assegno ogni elemento del
 * doppio puntatore in questo modo mi creo una lista collegata di nodi dove ognuno di questi è caratterizzato da un id con una fila e
 * una poltrona.
 * Se la scorre tutta impostandola A1 poi va al nodo successivo A2 e cosi via.
 * RITORNO: nodo della testa(head) di tale lista in modo che mi posso scorrere tutta la lista!
 *
 *
 *
 *
 *                                                     **    array_elem
 *
 *                                                     +-------------------------------+
 *                                                     |              A1               |  array_elem[0]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *                                                     +-------------------------------+
 *                                                     |              A2               |  array_elem[1]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *                                                     +-------------------------------+
 *                                                     |              A3               |  array_elem[2]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *                                                     +-------------------------------+
 *                                                     |              A4               |  array_elem[3]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *                                                     +-------------------------------+
 *                                                     |              A5               |  array_elem[4]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *                                                     +-------------------------------+
 *                                                     |              A6               |  array_elem[5]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *                                                     +-------------------------------+
 *                                                     |              A7               |  array_elem[6]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *                                                     +-------------------------------+
 *                                                     |              A8               |  array_elem[7]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *                                                     +-------------------------------+
 *                                                     |              B1               |  array_elem[8]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *                                                                    .
 *                                                                    .
 *                                                                    .
 *                                                                    .
 *                                                                    .
 *                                                     +-------------------------------+
 *                                                     |              F9               |  array_elem[LEN-2]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *                                                     +-------------------------------+
 *                                                     |             NULL              |  array_elem[LEN-1]
 *                                                     |                               |
 *                                                     +-------------------------------+
 *
 */
nodo * return_list_place(nodo* head ){
    char**array_row;
    int i,q;
    char**array_elem;
    array_elem=malloc(sizeof(char*));
    if(array_elem==NULL){
        fprintf(stderr,"error in malloc\n");
        exit(EXIT_FAILURE);
    }
    array_row=malloc(sizeof(char*));
    if(array_row==NULL){
        fprintf(stderr,"error in malloc\n");
        exit(EXIT_FAILURE);
    }
    array_row=define_seat();
    for (i=0;i<10;i++) {//10 perchè sono 10 elementi
        array_elem = tokenize_elem(*array_row, " ");
        for (q = 0; *array_elem != NULL; q++) {
            nodo*  new;
            new=malloc(sizeof(nodo));
            if(new==NULL){
                perror("error nella malloc\n");
                exit(EXIT_FAILURE);
            }
            if (array_elem[q] == NULL) {
                array_row++;
                break;
            }
            strcpy(new->id, array_elem[q]);
            head=aggiungi_in_coda(head,new);

        }

    }
    return head;
}
/*
 * Funzione che viene utilizzata per controllare il posto richiesto dal client se esistente, esempio in caso di A1
 * ha successo mentre in caso tu richiedi X9 ritorna 1 cioè fallisce!
 */
int controlla_buf(char * buf,nodo* head){
    nodo* p;
    if(head==NULL){
        errno=0;
        fprintf(stdout,"Questa sala non è stata costruita!\n");
        if(errno!=0){

            fprintf(stderr,"error\n");
            exit(1);
        }
        exit(0);
    }
    for(p=head;p!=NULL;p=p->next){


        if(strcmp(buf,p->id)==0) {

            return 0;
        }

    }
    return 1;
}
/*
 * Setta l'intera lista di nodi, elemento per elemento, settando il n_riga e n_posto
 */
void set_lista(nodo* head){

    nodo* p;
    if(head==NULL){
        errno=0;
        fprintf(stdout,"Non ci sono elementi quindi questo è l'unico elemento!\n");
        if(errno!=0){

            fprintf(stderr,"error\n");
            exit(1);
        }
        exit(0);
    }
    for(p=head;p!=NULL;p=p->next){

        char *c;
        c=malloc_buff(2);
        if(c==NULL){
            perror("Errore nella malloc\n");
            exit(EXIT_FAILURE);
        }
        sprintf(c,"%s",&p->id[1]);
        if(p->next==NULL){
            p->n_riga=p->id[0];
            p->n_posto=atoi(c);
            //p->code = random_code();

        }
        else{
            p->n_riga=p->id[0];
            p->n_posto=atoi(c);
            //p->code = random_code();
        }
        memset(c,'\0', sizeof(char)*2);
    }
}
/*Crea una mappa di posti  dove per input ha la testa del nodo, si scorre tutta la lista fin quando
 * Ritorno: un buffer che contiene tutta la mappa
 */
char * mappa_dei_posti(nodo *head){

    char *buffer=malloc(SIZE);
    nodo* p;
    if(head==NULL){
        errno=0;
        fprintf(stdout,"Non ci sono elementi quindi questo è l'unico elemento!\n");
        if(errno!=0){

            fprintf(stderr,"error\n");
            exit(1);
        }
        exit(0);
    }
    for(p=head;p!=NULL;p=p->next){
        if(p->n_posto==9 ){
            strcat(buffer,p->id);
            strcat(buffer,"\n");

        }
        else if(p->next==NULL){

            strcat(buffer,p->id);
        }
        else{
            strcat(buffer,p->id);
            strcat(buffer,"---");
        }
    }
    return buffer;
}

/*
 * Funzione usata per stamapre la mappa
 */


void stampa_mappa(nodo *head){
    //fprintf(stdout,"\n\nLa mappa dei posti della sala è la seguente:\n-----------------------------------------------------\n");

    nodo* p;
    if(head==NULL){
        errno=0;
        fprintf(stdout,"Non ci sono elementi quindi questo è l'unico elemento!\n");
        if(errno!=0){

            fprintf(stderr,"error\n");
            exit(1);
        }
        exit(0);
    }
    for(p=head;p!=NULL;p=p->next){
        if(p->n_posto==9 ){
            printf("%s\n",p->id);
        }
        else if(p->next==NULL){
            printf("%s",p->id);
        }
        else{
            printf("%s---",p->id);
        }
    }

}
/*
 * Funzione che si scandisce tutta la lista e va a cercare un nodo con l'id dato in input,
 * se lo trova pulisce il buffer del codice e setta il posto come libero
 */
void setta_posto_libero(nodo *head, char *id){
    nodo* p;
    p = malloc(sizeof(nodo));
    if (p == NULL) {
        fprintf(stderr, "Errore nella malloc\n");
        exit(EXIT_FAILURE);
    }
    for(p=head;p->next!=NULL;p=p->next){
        if(strcmp(p->id,id)==0){
            p->occupato=false;
            free(p->code);
            //memset(p->code,0,strlen(p->code));
        }
    }

}
/*
 * Funzione che mi permette di settare ogni elemento  della mia lista collegata
 * occupato o libero in funzione del contenuto del file mappa_sala_disponibilità.txt
 *
 */
void setta_disponibilita_in_lista(nodo *head, char *contenuto_file_disponibilita) {
    nodo *p;
    char *b_appoggio;
    b_appoggio = malloc_buff(7);//A1=L
    contenuto_file_disponibilita+=107;
    for (p = head; p != NULL; p = p->next){
        if (p->n_posto != 9){
            strncpy(b_appoggio, contenuto_file_disponibilita, 6);
            contenuto_file_disponibilita += 6;
        }
        else {
            strncpy(b_appoggio, contenuto_file_disponibilita,5);
            contenuto_file_disponibilita += 5;

        }
        if (strncmp(p->id, b_appoggio, 2) == 0) {
            if (b_appoggio[3] == 'L') {
                //In caso è occupato O
                p->occupato = false;
            }
            if (b_appoggio[3] == 'O') {
                //In caso è occupato O
                p->occupato = true;
            }
        }

        if (p->next == NULL) {
            //Ultimo elemento
            if (strncmp(p->id, b_appoggio, 2) == 0) {
                if (b_appoggio[3] == 'L') {
                    //In caso è occupato O
                    p->occupato = false;
                }
                if (b_appoggio[3] == 'O') {
                    //In caso è occupato O
                    p->occupato = true;
                }
            }
        }

        memset(b_appoggio, 0, 7);
    }


}


/*
 * Funzione usata per verificare se è occupato o libero un particolare posto, in caso lo devo occupare ora perchè precedentemente
 * è libero setto nel valore di quella struct il valore
 */
bool setta_elemento(char * posizione, nodo *head){
    nodo* p;
    for(p=head;p!=NULL;p=p->next){
        if(strcmp(p->id,posizione)==0 ){
            fprintf(stdout,"E'stato trovato l'elemento cercato:\t\t");
            if(p->occupato==true){
                //In caso è occupato O
                return true;
            }
            if(p->occupato==false){
                //In caso è libero L
                p->occupato=true;
                p->code=random_code();
                return false;
            }
        }
        if(p->next==NULL){
            //Ultimo elemento
            if(strcmp(p->id, posizione) == 0){
                printf("E'stato trovato l'elemento cercato");
                if(p->occupato==true){
                    //In caso è occupato O
                    return true;
                }
                if(p->occupato==false){
                    //In caso è libero L lo occupo
                    p->occupato=true;
                    p->code=random_code();
                    return false;
                }
            }
            else{
                fprintf(stdout,"L'elemento cercato non è stato trovato\n");
                return true;

            }
        }
    }

}
/*
 * Funzione usata per riprendere il codice di quel particolare nodo richiesto!
 */
char* take_code(char * posizione, nodo *head) {
    nodo *p;
    for (p = head; p != NULL; p = p->next) {
        if (strcmp(p->id, posizione) == 0) {
            //      printf("E'stato trovato l'elemento cercato");
            if (p->occupato == true) {
                //In caso è occupato O
                return p->code;
            }

        }
        else if (p->next == NULL) {
            //Ultimo elemento
            if (strcmp(p->id, posizione) == 0) {
                //        printf("E'stato trovato l'elemento cercato");
                if (p->occupato == true) {
                    //In caso è occupato O
                    return p->code;
                }
            } else {
                fprintf(stdout,"L'elemento cercato non è stato trovato o è ancora libero\n");

            }
        }
    }
}
/*
 * Stampa le disponibilità della mappa con la relativa leggenda,annessa!
 */
char* stampa_mappa_posti_disponibili(nodo *head){

    char *string_da_stampare;
    string_da_stampare=malloc_buff(SIZE);
    strcat(string_da_stampare,"\nLa mappa dei posti rispetto alle disponibilità della sala è la seguente:\n");
    strcat(string_da_stampare,"Leggenda: O=Occupato: L=Libero\n");

    nodo* p;
    if(head==NULL){
        errno=0;
        fprintf(stdout,"Non ci sono elementi quindi questo è l'unico elemento!\n");
        if(errno!=0){

            fprintf(stderr,"error\n");
            exit(1);
        }
        exit(0);
    }
    for(p=head;p!=NULL;p=p->next){
        if(p->n_posto==9 ){
            strcat(string_da_stampare,p->id);
            if(p->occupato==true){
                strcat(string_da_stampare,"=O");
            }
            if(p->occupato==false){
                strcat(string_da_stampare,"=L");
            }
            strcat(string_da_stampare,"\n");
        }
        else if(p->next==NULL){
            strcat(string_da_stampare,p->id);
            if(p->occupato==true){
                strcat(string_da_stampare,"=O");
            }
            if(p->occupato==false){
                strcat(string_da_stampare,"=L");
            }

        }
        else{
            strcat(string_da_stampare,p->id);
            if(p->occupato==true){
                strcat(string_da_stampare,"=O");
            }
            if(p->occupato==false){
                strcat(string_da_stampare,"=L");
            }
            strcat(string_da_stampare,"--");
        }
        //memset(string_da_stampare,'\0', sizeof(char)*5);
    }
    fprintf(stdout,"Sto mandando %s\n",string_da_stampare);
    return string_da_stampare;

}

/*
 * controllosuFile_code:-> In questa funzione vado a controllare se nel file database_prenotazioni.txt è presente un buffer,il cui contenuto coincide con il buffer 'input'
 * param1:char *input-> contiente le informazioni di (id: codice)
 * param2:nodo* head-> mi permette di scandirmi tutta la lista e fare un doppio controllo per aumentare la sicurezza
 *
 */

char * controllosuFile_code(char *input,nodo* head){
    char *contenuto_file;
    int q;
    int size_file_prev,size_file;
    char *prenotazioni;
    char *ritorno;
    ritorno=malloc_buff(SIZE);
    prenotazioni=malloc_buff(SIZE);
    size_file_prev=value_size_file("database_prenotazioni.txt");
    contenuto_file=malloc_buff(SIZE);
    contenuto_file=lettura_database_prenotazioni();
    //prenotazioni=tokenize_elem(contenuto_file,"\n");
    //memset(contenuto_file,0,SIZE);

    char *id = malloc_buff(4);
    strncpy(id,input,2);
    id[2]='\0';
    //Verifica se il 3 ,4 o il 1 elemento è un intero,in caso setta l'errore

    if(isint(&input[3])||isint(&input[0])||isint(&input[2])){
        strcpy(ritorno,"Errore nell'elemento scritto\n");
    }

    else if(controlla_buf(id,head)){
        strcpy(ritorno,"Errore nell'elemento scritto\n");
    }

    else if(strlen(input)!=18){
        strcpy(ritorno,"Errore nell'elemento scritto\n");
    }
    else {
        ritorno=strstr(contenuto_file,input);

        if(ritorno==NULL){
            free(ritorno);
            ritorno=malloc_buff(SIZE);
            strcpy(ritorno,"Errore nel codice riferito a quel posto!\n");
        }
        else{
            size_file=strlen(ritorno);
            contenuto_file=leggi_da_file(size_file_prev-size_file);
            fprintf(stdout,"%s\n",contenuto_file);
            //strcpy(prenotazioni,contenuto_file);
            if(size_file!=19) {
                ritorno += 19;


                fprintf(stdout,"%s\n",contenuto_file);
                strcat(contenuto_file, ritorno);
                //memset(contenuto_file, 0, strlen(contenuto_file));
            }
            aggiorna_database(contenuto_file);
            setta_posto_libero(head, id);

            memset(ritorno,0,SIZE);
            //aggiornare il file
            strcpy(ritorno,"Cancellata prenotazione con successo!\n");
        }


    }
    return ritorno;

}