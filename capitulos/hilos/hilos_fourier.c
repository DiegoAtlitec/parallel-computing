#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

#define PI 3.14159265358979323846
#define PASO 0.1
#define A0_1 (pow(M_PI, 4) / 25) + 1
#define A0_2 0.0f
#define A0_3 -(pow(M_PI, 4) - 150.0f) * (1.0f / 30.0f)
#define A0_4 3.0f

double funcion_fourier(double x, int n, int caso)
{
    switch (caso) {
    case 0: {
        float AN = (2 * (((pow(M_PI, 4) + 5) * pow(n, 4) - 12 * pow(M_PI, 2) * pow(n, 2) + 24) * sin(M_PI * n))
                    + 4 * M_PI * n * (pow(M_PI, 2) * pow(n, 2) - 6) * cos(M_PI * n))
                   / (5 * M_PI * pow(n, 5));
        float BN = 0;
        return (AN * cos(n * x)) + (BN * sin(n * x));
    }
    case 1:
        return (-1.0f / 15.0f) * (1.0f / pow(n, 3) * (((5.0f * pow(M_PI * n, 2)) - (6.0f * pow(n, 2)) - 30.0f) * pow(-1, n)) * sin(n * x));
    case 2:
        return -(4.0f / 3.0f) * ((pow(M_PI * n, 2) - 6.0f) * pow(-1, n)) * (1.0f / pow(n, 4)) * cos(n * x);
    case 3:
        return (2.0f * pow(-1, n) * ((1.0f - pow(M_PI, 2)) * pow(n, 2) + 6.0f) * sin(n * x)) / pow(n, 3);
    default:
        return -1.0f;
    }
}

double obtener_a0(int opcion_funcion)
{
    switch (opcion_funcion)
    {
    case 0:
        return A0_1;
    case 1:
        return A0_2;
    case 2:
        return A0_3;
    case 3:
        return A0_4;
    default:
        return 0.0;
    }
}

typedef struct
{
    int opcion_funcion;
    int numero_armonicos;
    double x;
    double *resultado;
} DatosHilo;

void *calcular_suma_fourier(void *arg)
{
    DatosHilo *datos = (DatosHilo *)arg;
    double suma_armonicos = 0.0;

    for (int n = 1; n <= datos->numero_armonicos; n++)
        suma_armonicos += funcion_fourier(datos->x, n, datos->opcion_funcion);
    *datos->resultado = obtener_a0(datos->opcion_funcion) + suma_armonicos;

    free(datos);
    return NULL;
}

int calcular_hilos(double pi, double paso)
{
    return (int)ceil(2 * pi / paso) + 1; // Se suma 1 para incluir el valor de x = pi
}

int main()
{
    // Cantidad de armonicos a calcular
    int numero_armonicos;
    char entrada[128];
    while (1)
    {
        char *fin;
        long valor;

        printf("Ingrese un numero entero positivo de armonicos: ");
        if (fgets(entrada, sizeof(entrada), stdin) == NULL)
        {
            fprintf(stderr, "No se pudo leer la entrada.\n");
            return 1;
        }

        errno = 0;
        valor = strtol(entrada, &fin, 10);
        while (isspace((unsigned char)*fin))
            fin++;

        if (fin != entrada && *fin == '\0' && errno != ERANGE &&
            valor > 0 && valor <= INT_MAX)
        {
            numero_armonicos = (int)valor;
            break;
        }

        printf("Entrada invalida. Debe ingresar un entero positivo.\n");
    }

    struct timespec tiempo_inicio;
    struct timespec tiempo_fin;
    if (clock_gettime(CLOCK_MONOTONIC, &tiempo_inicio) != 0)
    {
        perror("Error al obtener el tiempo inicial");
        return 1;
    }

    int numero_hilos = calcular_hilos(PI, PASO);
    double resultados[4][numero_hilos];
    pthread_t hilos[numero_hilos];

    for (int opcion_funcion = 0; opcion_funcion < 4; opcion_funcion++)
    {
        for (int i = 0; i < numero_hilos; i++)
        {
            DatosHilo *datos_hilo = malloc(sizeof(DatosHilo));
            if (datos_hilo == NULL)
            {
                perror("Error al reservar memoria para datos del hilo");
                return 1;
            }

            datos_hilo->opcion_funcion = opcion_funcion;
            datos_hilo->numero_armonicos = numero_armonicos;
            datos_hilo->x = -PI + i * PASO;
            datos_hilo->resultado = &resultados[opcion_funcion][i];

            if (pthread_create(&hilos[i], NULL, calcular_suma_fourier, datos_hilo) != 0)
            {
                perror("Error al crear hilo");
                free(datos_hilo);
                return 1;
            }
        }

        for (int i = 0; i < numero_hilos; i++)
        {
            if (pthread_join(hilos[i], NULL) != 0)
            {
                perror("Error al esperar a que termine el hilo");
                return 1;
            }
        }
    }

    FILE *archivo = fopen("resultados_fourier.csv", "w");
    if (archivo == NULL)
    {
        perror("Error al crear el CSV");
        return 1;
    }

    fprintf(archivo, "x,ernesto,uriel,omar,diego\n");
    for (int i = 0; i < numero_hilos; i++)
    {
        double x = -PI + i * PASO;
        fprintf(archivo, "%lf,%lf,%lf,%lf,%lf\n", x,
                resultados[0][i], resultados[1][i],
                resultados[2][i], resultados[3][i]);
    }
    fclose(archivo);

    if (clock_gettime(CLOCK_MONOTONIC, &tiempo_fin) != 0)
    {
        perror("Error al obtener el tiempo final");
        return 1;
    }

    double tiempo_transcurrido = (tiempo_fin.tv_sec - tiempo_inicio.tv_sec) +
                                 (tiempo_fin.tv_nsec - tiempo_inicio.tv_nsec) / 1e9;
    printf("Tiempo transcurrido: %.6f segundos\n", tiempo_transcurrido);

    return 0;
}