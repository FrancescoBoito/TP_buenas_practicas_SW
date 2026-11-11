#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODOS 124
#define TAM_UNIDAD 32
#define TAM_LINEA 128

struct Nodo {
    int direccion;
    int medicion;
    char unidad[TAM_UNIDAD];
};

/* Lee una línea completa. Devuelve 0 si se termina la entrada. */
static int leerLinea(const char *mensaje, char *linea, int tamanio)
{
    for (;;) {
        printf("%s", mensaje);
        fflush(stdout);

        if (fgets(linea, tamanio, stdin) == NULL) {
            return 0;
        }

        char *salto = strchr(linea, '\n');
        if (salto != NULL) {
            *salto = '\0';
            return 1;
        }

        int caracter = getchar();
        if (caracter == '\n' || caracter == EOF) {
            return 1;
        }

        while (caracter != '\n' && caracter != EOF) {
            caracter = getchar();
        }
        puts("La entrada es demasiado larga.");
    }
}

/* Escribe el resultado por puntero solo si la línea contiene un entero válido. */
static int leerEntero(const char *mensaje, int minimo, int maximo, int *resultado)
{
    char linea[TAM_LINEA];
    while (leerLinea(mensaje, linea, sizeof linea)) {
        char *fin;
        errno = 0;
        long numero = strtol(linea, &fin, 10);

        if (fin != linea && errno != ERANGE && numero >= minimo &&
            numero <= maximo && fin[strspn(fin, " \t\r")] == '\0') {
            *resultado = (int)numero;
            return 1;
        }
        printf("Ingrese un entero entre %d y %d.\n", minimo, maximo);
    }
    return 0;
}

static int leerUnidad(const char *mensaje, char *unidad)
{
    char linea[TAM_LINEA];
    while (leerLinea(mensaje, linea, sizeof linea)) {
        if (linea[0] != '\0' && strlen(linea) < TAM_UNIDAD) {
            strcpy(unidad, linea);
            return 1;
        }
        printf("Ingrese una unidad de 1 a %d caracteres.\n", TAM_UNIDAD - 1);
    }
    return 0;
}

/* La dirección identifica a un único nodo dentro de la red. */
static int buscarDireccion(const struct Nodo *nodos, int cantidad, int direccion)
{
    for (int i = 0; i < cantidad; i++) {
        if (nodos[i].direccion == direccion) {
            return i;
        }
    }
    return -1;
}

static int cargarNodo(const struct Nodo *nodos, int cantidad, struct Nodo *nuevo)
{
    for (;;) {
        if (!leerEntero("Direccion (1 a 124): ", 1, MAX_NODOS, &nuevo->direccion)) {
            return 0;
        }
        if (buscarDireccion(nodos, cantidad, nuevo->direccion) == -1) {
            break;
        }
        puts("Esa direccion ya esta registrada.");
    }

    return leerEntero("Medicion: ", INT_MIN, INT_MAX, &nuevo->medicion) &&
           leerUnidad("Unidad (por ejemplo TEMP, HUMEDAD o PRESION): ", nuevo->unidad);
}

static void mostrarPorUnidad(const struct Nodo *nodos, int cantidad, const char *unidad)
{
    int coincidencias = 0;

    for (int i = 0; i < cantidad; i++) {
        if (strcmp(nodos[i].unidad, unidad) == 0) {
            printf("Direccion: %d | Medicion: %d | Unidad: %s\n",
                   nodos[i].direccion, nodos[i].medicion, nodos[i].unidad);
            coincidencias++;
        }
    }

    if (coincidencias == 0) {
        puts("No hay nodos con esa unidad.");
    }
}

int main(void)
{
    struct Nodo nodos[MAX_NODOS];
    int iniciales;
    int cantidad = 0;

    if (!leerEntero("Cantidad inicial de nodos (1 a 124): ", 1, MAX_NODOS, &iniciales)) {
        return EXIT_SUCCESS;
    }

    while (cantidad < iniciales) {
        printf("\nNodo %d de %d\n", cantidad + 1, iniciales);
        if (!cargarNodo(nodos, cantidad, &nodos[cantidad])) {
            return EXIT_SUCCESS;
        }
        cantidad++;
    }

    int opcion;
    do {
        puts("\n1. Nuevo nodo\n2. Mostrar nodos por unidad\n3. Salir");
        if (!leerEntero("Opcion: ", 1, 3, &opcion)) {
            break;
        }
        if (opcion == 1) {
            if (cantidad == MAX_NODOS) {
                puts("Se alcanzo el limite de 124 nodos.");
            } else if (cargarNodo(nodos, cantidad, &nodos[cantidad])) {
                cantidad++;
                puts("Nodo agregado.");
            } else {
                break;
            }
        } else if (opcion == 2) {
            char unidad[TAM_UNIDAD];
            if (!leerUnidad("Unidad a buscar: ", unidad)) {
                break;
            }
            mostrarPorUnidad(nodos, cantidad, unidad);
        }
    } while (opcion != 3);

    return EXIT_SUCCESS;
}
