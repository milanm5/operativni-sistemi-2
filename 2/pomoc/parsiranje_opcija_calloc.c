#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Options {
    int list_hidden;
    int recursive;
} options;

/*
 * Postavlja opcije i vraca indeks poslednjeg argumenta
 * koji pocinje sa "-". Ukoliko takvih argumenata nije
 * bilo, vraca se 0.
 */
int parseArguments(int argc, char **argv, options *o){

    int lastFlagIndex = 0;
    
    int i;
    for (i = 1; i < argc; i++){

        if (argv[i][0] != '-'){
            return lastFlagIndex;
        }
        
        lastFlagIndex += 1;

        if (strcmp(argv[i], "-R") == 0){
            o->recursive = 1;
        }
        else if (strcmp(argv[i], "-a") == 0){
            o->list_hidden = 1;
        }
    }
    
    return lastFlagIndex;

}

/*
 * U program se prosledjuju parametri 2 vrste:
 *   - opcije -- parametri koji odredjuju ponasanje
 *     programa. Pocinju sa karakterom '-'
 *   - pravi  -- parametri koji idu posle opcija.
 *     
 * Program obradjuje sve prosledjene parametre. Prvo se
 * ucitavaju "opcije", a cim pronadje prvi parametar 
 * koji ne pocinje sa karakterom '-', prelazi se u mod 
 * citanja "pravih" parametara.
 * 
 * Poziv je oblika:
 *   ./program [lista_opcija] [lista_pravih_parametara]
 *
 */
int main(int argc, char **argv){

    // Zauzimamo memoriju za opcije na heap-u pozivom funkcije calloc
    options *o = (options *) calloc(1, sizeof(options));

    int lastFlagIndex = parseArguments(argc, argv, o);

    if (o->list_hidden){
        printf("Korisnik zeli da se ispisuju sakriveni fajlovi!\n");
    }
    
    if (o->recursive){
        printf("Korisnik zeli da se ide rekurzivno kroz direktorijume!\n");
    }

    if (lastFlagIndex == 0){
        printf("Korisnik nije prosledio ni 1 argument koji pocinje sa '-'\n");
    }

    if (lastFlagIndex + 1 < argc) {
        printf("Poslednji argument sa '-' je na poziciji %d, "
            "a ukupno ima %d argumenata. Dakle, imamo jos %d"
            " \"pravih\" argumenata da parsiramo!\n", lastFlagIndex, argc, argc - (lastFlagIndex + 1));

        int firstTrueArgument = lastFlagIndex + 1;
        
        int i;
        for (i = firstTrueArgument; i < argc; i++){
            printf("Pravi argumenta na indeksu %d je: %s\n", i, argv[i]);
        }
        
            
    }

    // Obratiti paznju da se zauzeta memorija na kraju izvrsavanja
    // programa oslobodi.
    free(o);

}
