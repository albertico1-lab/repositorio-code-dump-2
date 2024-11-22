#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h> // Para getuid, write y close

void usage(char *prog_name, char *filename) {
    printf("Usage: %s <data to add to %s>\n", prog_name, filename);
    exit(0);
}

void fatal(char *message); // Para errores fatales
void *ec_malloc(unsigned int size); // Malloc con verificación de errores

int main(int argc, char *argv[]) {
    int userid, fd; // File descriptor
    char *buffer, *datafile;

    buffer = (char *)ec_malloc(100);
    datafile = (char *)ec_malloc(20);
    strncpy(datafile, "/var/notes", 19); // Copia segura del nombre del archivo
    datafile[19] = '\0'; // Asegurar el terminador nulo

    if (argc < 2) // Mostrar uso si faltan argumentos
        usage(argv[0], datafile);

    strncpy(buffer, argv[1], 99); // Copia segura de la entrada del usuario
    buffer[99] = '\0'; // Asegurar el terminador nulo

    printf("[DEBUG] buffer @ %p: '%s'\n", buffer, buffer);
    printf("[DEBUG] datafile @ %p: '%s'\n", datafile, datafile);

    // Abrir el archivo
    fd = open(datafile, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd == -1)
        fatal("in main() while opening file");
    printf("[DEBUG] file descriptor is %d\n", fd);

    userid = getuid(); // Obtener el UID real del usuario

    // Escribir datos en el archivo
    if (write(fd, &userid, sizeof(userid)) == -1) // Escribir el UID
        fatal("in main() while writing userid to file");

    if (write(fd, "\n", 1) == -1) // Escribir un salto de línea
        fatal("in main() while writing newline to file");

    if (write(fd, buffer, strlen(buffer)) == -1) // Escribir la nota
        fatal("in main() while writing buffer to file");

    if (write(fd, "\n", 1) == -1) // Terminar con un salto de línea
        fatal("in main() while writing newline to file");

    // Cerrar el archivo
    if (close(fd) == -1)
        fatal("in main() while closing file");

    printf("Note has been saved.\n");

    free(buffer);
    free(datafile);

    return 0;
}

// Función para manejar errores fatales
void fatal(char *message) {
    char error_message[100];
    strncpy(error_message, "[!!] Fatal Error ", 99);
    strncat(error_message, message, 83);
    perror(error_message);
    exit(-1);
}

// Función para malloc con verificación de errores
void *ec_malloc(unsigned int size) {
    void *ptr = malloc(size);
    if (ptr == NULL)
        fatal("in ec_malloc() on memory allocation");
    return ptr;
}
