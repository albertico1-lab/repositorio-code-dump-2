// hacking.h
#include <stdio.h>
#include <stdlib.h> // Para malloc y exit
#include <unistd.h> // Para funciones relacionadas con el sistema, como getuid, read, close, lseek

void fatal(char *message) {
    char error_message[100];
    snprintf(error_message, sizeof(error_message), "[!!] Fatal Error: %s", message);
    perror(error_message);
    exit(-1);
}

void *ec_malloc(unsigned int size) {
    void *ptr;
    ptr = malloc(size);
    if (ptr == NULL)
        fatal("in ec_malloc() on memory allocation");
    return ptr;
}

