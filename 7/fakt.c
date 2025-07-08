#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>

void deliSe(int pocetno) {
    int n = pocetno;
    // da li treba da se napravi dete
    bool hoceDecu = true;
    // pamtimo krajeve pajpa na koje cemo pisati
    int izlaz, ulaz;    

    // svaki proces ce trebati da napravi jedno dete, osim poslednjeg
    while (n > 1 && hoceDecu) {
        int pfd[2];		
        int piperes = pipe(pfd);
        if (piperes != 0) {
            exit(1);
        }
        int pid = fork();
        if (pid == -1) {
            exit(-1);
        }
        if (pid > 0) {
            // zapamtimo kraj za citanje
            ulaz = dup(pfd[0]);
            // vise ne treba da pravi dete
            hoceDecu = false;
        } else {
            // ako nismo prvi proces, nasledili smo otvoreni izlaz
            if (n<pocetno)
                close(izlaz);
            // zapamtimo kraj za pisanje prema roditelju
            izlaz = dup(pfd[1]);
            // prilagodimo n za novo dete
            n--;
        }
        // zapamtili smo odgovarajuce krajeve, mozemo ovo zatvoriti
        close(pfd[0]);
        close(pfd[1]);
    }
    
    // nakon sto je formiran lanac radimo glavnu logiku
    printf("%d ",n);
    // uzecemo relativno velik tip broja da mozemo da racunamo veci faktorijel
    unsigned long rez = 1;
    // svi osim poslednjeg
    if (n > 1) {
        read(ulaz, &rez, sizeof(rez));
        close(ulaz);
        rez *= n;
    }
    // svi osim prvog
    if (n< pocetno) {
        write (izlaz, &rez, sizeof(rez));
        close(izlaz);
    }
    // samo prvi proces
    if (n == pocetno) {
        printf("rez: %lu\n", rez);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Potreban je jedan argument - broj za faktorijel\n");
        exit(1);
    }
    errno = 0;
    int n = strtol(argv[1],NULL,10);
    if (errno != 0) {
        perror("Problem sa unetim brojem");
        exit(1);
    }
    if (n < 0) {
        printf("Ne mozemo racunati faktorijel za negativne brojeve\n");
        exit(1);
    }
    if (n == 0) {
        printf("rez: 0\n");
        exit(0);
    }
    deliSe(n);
}