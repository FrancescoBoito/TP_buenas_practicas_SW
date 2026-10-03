#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define nodosMaximos 124


// definicion de la estructura temperatura y humedad
struct temperatura
{
    int direccion;
    int medicion;
    char unidad[10];
};

struct humedad
{
    int direccion;
    int medicion;
    char unidad[10];
};

// prototipos
void nuevoNodo(struct temperatura *temp, struct humedad *hum);
void ingresarDatos(struct temperatura *temp, struct humedad *hum);
int printScreen(struct temperatura *temp, struct humedad *hum);

// nuevo nodo
int main(void)
{
    struct temperatura temp;
    struct humedad hum;
    int opcion;
    printf("desea generar un nuevo nodo? (s/n): ");

    nuevoNodo(&temp, &hum);
    do {
        printf("\n=============================\n");
        printf("        NODOS Temp y Humedad\n");
        printf("=============================\n");

        printf("\n1. Nuevo Nodo\n");
        printf("2. Mostrar Nodos\n");
        printf("3. Cargar Mediciones\n");
        printf("4. Unidad a utilizar\n");
        printf("5. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
            case 1:
                nuevoNodo(&temp, &hum);
                break;
            case 2:
                printScreen(&temp, &hum);
                break;
            case 3:
                ingresarDatos(&temp, &hum);
                break;
            case 4:
                printf("Ingrese la unidad de temperatura (C/F): ");
                scanf("%9s", temp.unidad);
                printf("Ingrese la unidad de humedad (%%): ");
                scanf("%9s", hum.unidad);
                break;
            case 5:
                exit(0);
            default:
                printf("Opcion no valida\n");
                break;
        }
    } while (opcion != 5);

    return 0;
}

int printScreen(struct temperatura *temp, struct humedad *hum)
{
    printf("Temperatura: %d %s\n", temp=>medicion, temp=>unidad);
    printf("Humedad: %d %s\n", hum=>medicion, hum=>unidad);
    return 0;
}

