#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "hacking.h"

#define FILENAME "/var/notes"

// Declaraciones de funciones
int print_notes(int, int, char *);  // Función para imprimir notas.
int find_user_note(int, int);      // Buscar una nota para el usuario.
int search_note(char *, char *);   // Buscar una palabra clave en una nota.
void fatal(char *);                // Manejador de errores fatales

int main(int argc, char *argv[]) {
    int userid, printing = 1, fd;  // Descriptor de archivo y otras variables
    char searchstring[100];

    // Procesar argumentos de búsqueda
    if (argc > 1) 
        strcpy(searchstring, argv[1]);  // Copiar la cadena de búsqueda
    else 
        searchstring[0] = 0;  // Si no hay argumentos, la cadena está vacía.

    userid = getuid();  // Obtener el UID del usuario
    fd = open(FILENAME, O_RDONLY);  // Abrir el archivo en modo solo lectura
    if (fd == -1)
        fatal("in main() while opening file for reading");

    // Imprimir las notas
    while (printing)
        printing = print_notes(fd, userid, searchstring);

    printf("-------[ end of note data ]-------\n");
    close(fd);
}

// Función para imprimir notas de un usuario que coincidan con una cadena opcional
int print_notes(int fd, int uid, char *searchstring) {
    int note_length;
    char note_buffer[100];

    // Encontrar la nota para el UID especificado
    note_length = find_user_note(fd, uid);
    if (note_length == -1)  // Si se alcanza el final del archivo
        return 0;

    read(fd, note_buffer, note_length);  // Leer los datos de la nota
    note_buffer[note_length] = '\0';    // Terminar la cadena

    // Si la cadena de búsqueda coincide, imprimir la nota
    if (search_note(note_buffer, searchstring))
        printf("%s", note_buffer);

    return 1;  // Indicar que hay más notas
}

// Función para buscar la siguiente nota de un usuario específico
int find_user_note(int fd, int user_uid) {
    int note_uid = -1;
    unsigned char byte;
    int length;

    // Buscar una nota para el UID especificado
    while (note_uid != user_uid) {
        if (read(fd, &note_uid, 4) != 4)  // Leer el UID de la nota
            return -1;  // Si no se leen 4 bytes, fin del archivo

        if (read(fd, &byte, 1) != 1)  // Leer el separador de línea
            return -1;

        byte = length = 0;

        // Calcular la longitud de la nota
        while (byte != '\n') {
            if (read(fd, &byte, 1) != 1)  // Leer un byte
                return -1;
            length++;
        }
    }

    // Rewind del archivo por la longitud de la nota
    lseek(fd, length * -1, SEEK_CUR);
    printf("[DEBUG] found a %d byte note for user id %d\n", length, note_uid);
    return length;
}

// Función para buscar una palabra clave en una nota
int search_note(char *note, char *keyword) {
    int i, keyword_length, match = 0;

    keyword_length = strlen(keyword);
    if (keyword_length == 0)  // Si no hay cadena de búsqueda
        return 1;  // Siempre coincide

    // Iterar sobre los bytes de la nota
    for (i = 0; i < strlen(note); i++) {
        if (note[i] == keyword[match])  // Si el byte coincide
            match++;  // Verificar el siguiente byte
        else {
            if (note[i] == keyword[0])  // Si coincide el primer byte de la palabra clave
                match = 1;  // Reiniciar el conteo
            else
                match = 0;  // Reiniciar a cero
        }

        if (match == keyword_length)  // Si hay una coincidencia completa
            return 1;  // Devolver coincidencia
    }

    return 0;  // No hay coincidencia
}
