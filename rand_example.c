#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Incluir esta cabecera para usar time()

int main() {
    int i;
    printf("RAND_MAX is %u\n", RAND_MAX);
    srand(time(0)); // Inicializar el generador de números aleatorios con el tiempo actual
    
    printf("random values from 0 to RAND_MAX\n");
    for(i = 0; i < 8; i++) 
        printf("%d\n", rand()); // Generar números aleatorios

    printf("random values from 1 to 20\n");
    for(i = 0; i < 8; i++) 
        printf("%d\n", (rand() % 20) + 1); // Generar números aleatorios en el rango 1-20
}
