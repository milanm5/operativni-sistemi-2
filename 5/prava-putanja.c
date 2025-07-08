#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* Primer ispisa prave putanje argumenta, ili trenutnog
   direktorijuma ako nije dat parametar */

int main(int argc, char **argv) {
    char *putanja;
    if (argc == 1) {
        // napravimo buff koji ce biti popunjen
        char buff[PATH_MAX];
        putanja = getcwd(buff,PATH_MAX);
        if (putanja == NULL) {
            perror("Problem sa odredjivanjem trenutne putanje");
            exit(EXIT_FAILURE);
        }
    } else {
        putanja = argv[1];
    }
    
    // ako prosledimo NULL, bice zauzeta nova memorija za nas
    // inace je moglo kao i kod getcwd iznad
    char *prava = realpath(putanja, NULL);
    
    if (prava != NULL) {
        printf("Prava putanja je: %s\n", prava);
        free(prava);
    } else {
        perror("Problem sa konverzijom putanje");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}