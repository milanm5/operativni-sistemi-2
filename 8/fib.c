#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
/*
 * Racunanje fibonacijevih brojeva koriscenjem teskih procesa.
 */


void racunanje(int n) {
    int rez = 0;
    const int pocetno = n;
    int brojDece = 0;
    const int potrebnoDece = 2;
    int izlaz;
    //imacemo vise ulaza, posto imamo vise dece
    int ulaz[brojDece];
    
    // 0 i 1 imaju trivijalne izlaze, pa umanjujemo n dotle
    
    while (n > 1 && brojDece < potrebnoDece) {
        int pfd[2];
        int piperes = pipe(pfd);
        if (piperes != 0)
            exit(1);
        
        int pid = fork();
        if (pid < 0)
            exit(1);

        if (pid != 0) {
            ulaz[brojDece] = dup(pfd[0]);
            brojDece++;
        } else {
            n = n - 1;
            // ako vec postoji jedno dete, umanjimo se dodatno
            n = n - brojDece;
            
            // treba da zatvorimo nasledjene ulaze
            // komplikovanije nego sto mora ovde, ali je opsta varijanta
            for (int i=0; i<brojDece-1; i++)
                close(ulaz[i]);
            
            brojDece = 0;
            
            // svi osim prvog nasledjuju i izlaz od roditelja
            if (n < pocetno)
                close(izlaz);
            
            izlaz = dup(pfd[1]);
        }
        
        // zatvorimo krajeve pajpova
        close(pfd[0]);
        close(pfd[1]);
    }
    
    if (n <= 1) {
        // trivijalno 0 je 0, 1 je 1
        rez = n;
    } else {
        int i, temp;
        rez = 0;
        // citamo rezultate od sve dece
        for (i=0; i<brojDece; i++){
            read(ulaz[i], &temp, sizeof(temp));
            rez += temp;
            close(ulaz[i]);
        }
    }
    
    if (n == pocetno) {
        printf("fib za %d je %d\n", n, rez);
    } else {
        write(izlaz, &rez, sizeof(rez));
        close(izlaz);    
    }
            
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Potreban je jedan argument - broj za faktorijel\n");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    int n = strtol(argv[1],NULL,10);
    if (errno != 0) {
        perror("Problem sa unetim brojem");
        exit(EXIT_FAILURE);
    }

    racunanje(n);
}