#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"

// Callback para procesar cada campo de una línea CSV
void campo_callback(void* datos, size_t tamanio, void* usuario) {
    // Imprimimos el campo
    printf("%.*s ", (int)tamanio, (const char*)datos);
}

// Callback para indicar el fin de una fila
void fila_callback(int c, void* usuario) {
    printf("\n");
}

int main() {
    // Declaramos el puntero para el archivo
    FILE* file;

    // Especificamos la ruta absoluta del archivo
    const char* filepath = "C:\\Users\\Jon\\Desktop\\PBLtracker.txt";

    // Abrimos el archivo en modo lectura ("r")
    file = fopen(filepath, "r");

    // Verificamos si se abrió correctamente
    if (file == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", filepath);
        printf("Asegúrate de que el archivo exista y la ruta sea correcta.\n");
        return 1;
    }

    // Inicializamos el analizador CSV
    struct csv_parser parser;
    if (csv_init(&parser, 0) != 0) {
        printf("Error: No se pudo inicializar el analizador CSV\n");
        fclose(file);
        return 1;
    }

    printf("Contenido de PBLtracker.txt:\n");
    printf("-----------------------------\n");

    // Creamos un buffer para almacenar los datos leídos
    char buffer[1024];
    size_t bytes_leidos;

    // Leemos el archivo en fragmentos
    while ((bytes_leidos = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (csv_parse(&parser, buffer, bytes_leidos, campo_callback, fila_callback, NULL) != bytes_leidos) {
            printf("Error: Fallo al analizar el archivo CSV: %s\n", csv_strerror(csv_error(&parser)));
            csv_free(&parser);
            fclose(file);
            return 1;
        }
    }

    // Verificamos si ocurrió un error durante la lectura
    if (ferror(file)) {
        printf("Error: Fallo al leer el archivo\n");
    }

    // Finalizamos el análisis CSV
    csv_fini(&parser, campo_callback, fila_callback, NULL);
    csv_free(&parser);

    // Cerramos el archivo
    fclose(file);

    return 0;
}
