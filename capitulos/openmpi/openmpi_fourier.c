/**
 * @file openmpi_fourier.c
 * @brief Programa en lenguaje C para el cálculo paralelo de la Serie de Fourier usando OpenMPI.
 * @author Equipo 6BV1 - Cómputo Paralelo
 * @date 2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define PI 3.14159265358979323846
#define PASO 0.05
#define NUM_ARMONICOS 100

int main(int argc, char **argv) {
    int rank, size;
    
    // Inicialización del entorno MPI
    MPI_Init(&argc, &argv);
    
    // Obtención del rango del proceso actual
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Obtención del número total de procesos en ejecución
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Definición del dominio temporal / espacial (-PI a PI)
    int total_puntos = (int)ceil((2 * PI) / PASO) + 1;
    double *punto_reducido = (double *)malloc(total_puntos * sizeof(double));
    double *local_resultados = (double *)malloc(total_puntos * sizeof(double));
    
    if (punto_reducido == NULL || local_resultados == NULL) {
        fprintf(stderr, "Error al reservar memoria en el proceso %d\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    // Inicialización del arreglo local
    for (int i = 0; i < total_puntos; i++) {
        local_resultados[i] = 0.0;
    }
    
    // Cálculo distribuido de los armónicos de la serie de Fourier (Descomposición por rangos)
    for (int i = 0; i < total_puntos; i++) {
        double x = -PI + (i * PASO);
        double suma_parcial = 0.0;
        
        // Asignación estática de armónicos (Round-Robin) entre los procesos activos
        for (int n = rank + 1; n <= NUM_ARMONICOS; n += size) {
            // Coeficientes y evaluación armónica de la función algebraica propuesta
            double numerador = 2.0 * pow(-1, n) * ((1.0 - pow(PI, 2)) * pow(n, 2) + 6.0) * sin(n * x);
            double denominador = pow(n, 3);
            suma_parcial += (numerador / denominador);
        }
        local_resultados[i] = suma_parcial;
    }
    
    // Reducción colectiva: Suma de todas las contribuciones locales hacia el nodo maestro (rank 0)
    MPI_Reduce(local_resultados, punto_reducido, total_puntos, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    // Tareas exclusivas del Nodo Maestro (rank 0)
    if (rank == 0) {
        double A0 = 3.0; // Coeficiente de corriente directa de la función analizada
        FILE *csv = fopen("resultados_openmpi.csv", "w");
        if (csv == NULL) {
            perror("Error al crear el archivo CSV de resultados");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        
        fprintf(csv, "x,f_openmpi\n");
        for (int i = 0; i < total_puntos; i++) {
            double x = -PI + (i * PASO);
            double f_x = A0 + punto_reducido[i];
            fprintf(csv, "%lf,%lf\n", x, f_x);
        }
        fclose(csv);
        printf("[OpenMPI] Cálculo paralelo completado exitosamente con %d procesos.\n", size);
        printf("[OpenMPI] Archivo 'resultados_openmpi.csv' generado y guardado correctamente.\n");
    }
    
    // Liberación de recursos de memoria y finalización del entorno MPI
    free(local_resultados);
    free(punto_reducido);
    MPI_Finalize();
    
    return 0;
}