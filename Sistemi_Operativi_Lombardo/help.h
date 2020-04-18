//
// Created by lombi on 05/01/19.
//

#ifndef SISTEMI_OPERATIVI_LOMBARDO_HELP_H
#define SISTEMI_OPERATIVI_LOMBARDO_HELP_H

#endif //SISTEMI_OPERATIVI_LOMBARDO_HELP_H

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
#define  SIZE 1024
//Struct fondamentale con tutte le info
struct biglietto{
    bool occupato;//bool per vedere se il posto è occupato o libero
    struct biglietto *next;// puntatore all'elemento successivo
    int n_posto; //numero posto
    char n_riga;//n riferito alla riga quindi A,B ecc
    char *code; //codice univoco di prenotazione
    char id[3]; //id del posto quindi A1,ecc
};
typedef struct biglietto nodo;
//Apre un file con i flag
int apertura_file(char *filepath,int flag)
{
    int fd;
    fd=open(filepath,flag,0666);
    if(fd==-1){
        perror("Errore nell'apertura del file");
        exit(EXIT_FAILURE);
    }
    return fd;

}
/*
 * Malloc del buffer di un char
 */
char * malloc_buff(int dim){
    char *buf;
    buf=malloc(sizeof(char)*dim);
    if(buf==NULL){
        perror("Errore in malloc");
    }
    return buf;
}
/*
 * tokenize di un buffer che mi permette di tokenizzare nel modo opportuno un elemento e ritorna un char ** !
 * Usato per la costruzione della lista collegata in quanto viene usato per partizionare le varie file e poi
 * i vari posti di ogni fila nella lista collegata
 */
char ** tokenize_elem(char *buffer,char *del){
    int c=1;
    char **array_stringhe=malloc((sizeof(char*)*SIZE));
    if(array_stringhe==NULL){
        fprintf(stderr,"errore in malloc\n");
        exit(EXIT_FAILURE);
    }

    char *token=malloc_buff(SIZE);
    memset(token,'\0',SIZE);
    memset(array_stringhe,'\0',SIZE);
    array_stringhe[0] = strtok (buffer,del);
    //fprintf(stdout,"%s\n",array_stringhe[0]);
    while(token!=NULL){
        token=strtok(NULL,del);
        if(token==NULL){
            break;
        }
        array_stringhe[c]=token;

        //fprintf(stdout,"%s\n",array_stringhe[c]);
        c++;

    }
    free(token);
    return array_stringhe;
}

/*
 * Mi serve per controllare se la prenotazione puo esser fatta nella maniera adeguata
 */

int isint ( char  x ) {
    if ( ! ( x >= '0' && x <= '9' ) )
        return 0;

    return 1;
}

/*
char ** Stampa_and_tokenize_elem(char *buffer,char *del){
    int c=1;
    char **array_stringhe=malloc_buff(SIZE);

    char *token=malloc_buff(SIZE);
    memset(token,'\0',SIZE);
    memset(array_stringhe,'\0',SIZE);
    array_stringhe[0] = strtok (buffer,del);
      fprintf(stdout,"%s\n",array_stringhe[0]);
    while(token!=NULL){
        token=strtok(NULL,del);
        if(token==NULL){
            break;
        }
        array_stringhe[c]=token;

        fprintf(stdout,"%s\n",array_stringhe[c]);
        c++;

    }
    free(token);
    return array_stringhe;
}
*/