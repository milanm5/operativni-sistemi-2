#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

void runAndSplit() {
    printf("Start \n");
    int pid = fork();
    if (pid == -1) {
        exit(-1);
    }
    if (pid > 0) {
        printf("Parent  ");
        printf("Parent \n ");
        wait(NULL);
        printf("Child died\n");
    } else {
        printf("Child ");   
        printf("Child \n");   
    }
}

int main() {
    runAndSplit();
}