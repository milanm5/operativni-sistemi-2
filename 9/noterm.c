# include <stdio.h>
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>

/*

Program koji broji koliko puta je pokusano
da se ubije, pa tek posle cetvrtog puta
se zavrsi.

 */

int pokusaji = 0;

void brojac(int i) {
    // recimo da nam nije bitno u ovom momentu
    // da proverimo koji smo signal dobili jer smo 
    // registrovali dole to sto nam je potrebno
    pokusaji++;
}

int main() {
    struct sigaction sa;
    sa.sa_handler = brojac;
    sa.sa_flags= 0;
    
    if (sigaction(SIGINT, &sa , NULL))
        perror("Problem postavljanja signala");
    // mozemo registrovati isti hendler za vise signala
    if (sigaction(SIGTERM, &sa , NULL))
        perror("Problem postavljanja signala");
    
    while (1) {
        printf("spavam; pokusaji dosada:%d\n",pokusaji);
        if (pokusaji > 4)
            exit(0);
        sleep(5);
    }
}