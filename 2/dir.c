# include <stdio.h>
# include <sys/types.h>
# include <dirent.h>
# include <errno.h>
# include <stdlib.h>
# include <string.h>

// ispis sadrzaja datih direktorijuma; ispisuje se nekoliko podatak vezano za njih

void list(char *name) {
    DIR *d = opendir(name);
    if (d == NULL) {
        int err = errno;
        printf("error while reading %s: %d %s\n", name,err,strerror(err));
        return;
    }
    struct dirent *ent;
    // set errno for later checks
    errno = 0;
    while ((ent = readdir(d)) != NULL) {
    
        if (ent->d_type == DT_DIR)
            printf("d ");
        else
            printf("  ");
        printf("%ld\t %d \t %s",ent->d_ino, ent->d_type, ent->d_name);
        printf("\n");
    }
    if (errno != 0) {
        printf("error while reading dir: %d %s\n", errno,strerror(errno));       
    }
    int ret = closedir(d);
    if (ret == -1) {
        printf("error closing dir stream: %d %s\n", errno,strerror(errno));           
    }

}

int main(int argn, char **args) {
    if (argn == 1) {
        printf("No arguments given; exiting;\n");
        exit(0);
    }
    for (int i=1; i<argn; i++){
        printf(args[i]);
        printf("\n");
        list(args[i]);
    }
}