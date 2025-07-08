#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>

/* Primer pravljenja lanca procesa, svi ispisuju samo svoj broj.
   Ilustracija deljenja resursa u momentu kreiranja, primarno izlaznog
   bafera i njegovog stanja, posto nemamo \n posle ispisa "a ".
   
   Takodje je moguce u raznim pokretanjima da se izmesa redosled ispisa
   brojeva, posto su procesi nezavisni.
*/

void deliSe(int pocetno) {
    int n = pocetno;
    // da li treba da se napravi dete
    bool hoceDecu = true;

    printf("a ");    
    // svaki proces ce trebati da napravi jedno dete, osim poslednjeg
    while (n > 1 && hoceDecu) {
        int pid = fork();
        if (pid == -1) {
            exit(-1);
        }
        if (pid > 0) {
            // vise ne treba da pravi dete
            hoceDecu = false;
        } else {
            // prilagodimo n za novo dete
            n--;
        }
        // zapamtili smo odgovarajuce krajeve, mozemo ovo zatvoriti
    }
    printf("%d ",n);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Potreban je jedan argument - broj procesa\n");
        exit(1);
    }
    int n = atoi(argv[1]);
    if (n < 0) {
        printf("Ne mozemo imatei negativno procesa\n");
        exit(1);
    }
    if (n == 0) {
        printf("0\n");
        exit(0);
    }
    deliSe(n);
}