# include <stdio.h>
# include <sys/types.h>
# include <dirent.h>
# include <errno.h>
# include <string.h>

/* Primer koriscenja poziva scandir za neko selektivno
  ispisivanje podataka. Zadatak kaze da je ponasanje
  definisano prvim parametrom, pa cemo prijaviti problem
  ako ga ne prepoznamo.
  
 */

// filter funkcije koje koristimo u scandir

int filter_a(const struct dirent* de) {
    if (de == NULL) {
        return 0;
    }
    if (de->d_name[0]=='a') 
        return 1;
    return 0;
}

int filter_N(const struct dirent* de) {
    if (de == NULL) {
        return 0;
    }
    if (de->d_name[0]=='N') 
        return 1;
    return 0;
}


// listanje jednog direktorijuma, sa nekim filterom
void list(char *ime, 
            int (*filter)(const struct dirent *), 
            int (*sort_comp)(const struct dirent**, const struct dirent **) ) {

    struct dirent **stavke;
    int broj;
    
    broj = scandir(ime, &stavke, filter, sort_comp);
    if (broj == -1) {
        perror("Neuspesno citanje:");
        return;
    }
    
    for (int i =0; i < broj; i++) {
        if (stavke[i]->d_type == DT_DIR)
            printf("d ");
        else
            printf("  ");
        printf("%ld\t %d \t %s",stavke[i]->d_ino, stavke[i]->d_type, stavke[i]->d_name);
        printf("\n");
    }

}

int main(int argn, char **args) {
    // neki primeri za pokretanje bez parametara, tj brze testiranje
    // list(".");
    // list("/etc/apt");
    
    // postavimo da je podrazumevano nista, pa cemo iz argumenata odrediti sta treba
    int (*odabrani)(const struct dirent*) = NULL;
    int (*sort)(const struct dirent**, const struct dirent**) = NULL;
    
    if (strcmp(args[1],"--sa")==0) {
        odabrani = filter_a;
    } else if (strcmp(args[1],"--sN")==0) {
        odabrani = filter_N;
    } else if (strcmp(args[1],"--sort")==0) {
        sort = alphasort;
    } else {
        printf("Nije dat dobar prvi arugment");
        return -1;
    }
    
    for (int i=2; i<argn; i++){
        printf("%s\n",args[i]);
        list(args[i], odabrani, sort);
    }
}