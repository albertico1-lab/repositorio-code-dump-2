#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h> // Para write y close
#include <sys/stat.h>

void usage(char *prog_name, char *filename) {
    printf("Usage: %s <data to add to %s>\n", prog_name, filename);
    exit(0);
}

void fatal(char *message); // Para errores fatales
void *ec_malloc(unsigned int size); // Malloc con verificación de errores

int main(int argc, char *argv[]) {
    int fd; // File descriptor
    char *buffer, *datafile;

    buffer = (char *)ec_malloc(100);
    datafile = (char *)ec_malloc(20);
    strcpy(datafile, "/tmp/notes");

    if (argc < 2) // Mostrar uso si faltan argumentos
        usage(argv[0], datafile);

    strncpy(buffer, argv[1], 99); // Copiar de manera segura
    buffer[99] = '\0'; // Asegurar terminador nulo
    strncat(buffer, "\n", 2); // Agregar salto de línea de manera segura

    printf("[DEBUG] buffer @ %p: \'%s\'\n", buffer, buffer);
    printf("[DEBUG] datafile @ %p: \'%s\'\n", datafile, datafile);

    // Abrir archivo
    fd = open(datafile, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd == -1)
        fatal("in main() while opening file");
    printf("[DEBUG] file descriptor is %d\n", fd);

    // Escribir datos
    if (write(fd, buffer, strlen(buffer)) == -1)
        fatal("in main() while writing buffer to file");

    // Cerrar archivo
    if (close(fd) == -1)
        fatal("in main() while closing file");

    printf("Note has been saved.\n");

    free(buffer);
    free(datafile);

    return 0;
}

void fatal(char *message) {
    char error_message[100];
    strcpy(error_message, "[!!] Fatal Error ");
    strncat(error_message, message, 83);
    perror(error_message);
    exit(-1);
}

void *ec_malloc(unsigned int size) {
    void *ptr = malloc(size);
    if (ptr == NULL)
        fatal("in ec_malloc() on memory allocation");
    return ptr;
}
