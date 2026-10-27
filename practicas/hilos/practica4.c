#include<stdio.h>
#include<pthread.h>

#define NUM_FILAS 3
#define NUM_COLUMNAS 9

// Se define una estructura llamada DatosHilo que contiene información relevante para cada hilo.
typedef struct{
   int numero_hilo;
   int *resultado;
   int (*matriz)[NUM_COLUMNAS]; // matriz es un apuntador a una matriz de enteros con NUM_COLUMNAS columnas. Esto permite que cada hilo tenga acceso a la matriz completa y pueda operar sobre ella según sea necesario.
} DatosHilo;

void *Multiplicar_fila(void *argumentos) {
   DatosHilo *datos = (DatosHilo *)argumentos; // En esta línea, se realiza un casting del puntero de tipo void* a un puntero de tipo DatosHilo*.
   //Esto permite acceder a los miembros de la estructura DatosHilo a través del puntero datos.
   int numero_hilo = datos->numero_hilo; // Se accede al miembro numero_hilo de la estructura DatosHilo a través del puntero datos.
   int *resultado = datos->resultado; // Se accede al miembro resultado de la estructura DatosHilo a través del puntero datos.
   int (*matriz)[NUM_COLUMNAS] = datos->matriz; // Se accede al miembro matriz de la estructura DatosHilo a través del puntero datos.

   *(resultado) = 1; // Se inicializa el valor apuntado por resultado a 1. Esto es necesario para realizar la multiplicación correctamente, ya que cualquier número multiplicado por 1 permanece igual.

   for(int i = 0; i < NUM_COLUMNAS; i++)
      *(resultado) *= matriz[numero_hilo - 1][i]; // Se realiza la multiplicación de los elementos de la fila correspondiente de la matriz.

   pthread_exit(NULL); // Se finaliza la ejecución del hilo y se devuelve NULL como valo~r de retorno. Esto indica que el hilo ha terminado su trabajo y no tiene un valor específico para devolver.
}

int main (){
   //Primero abrimos el archivo de texto donde está la matriz
   FILE *archivo;
   archivo = fopen("matriz.txt", "r");
   if (archivo == NULL) {
      printf("Error al abrir el archivo\n");
      return 1;
   }

   // Inicializamos la matriz y leemos los datos del archivo
   int matriz[NUM_FILAS][NUM_COLUMNAS];
   for(int i = 0; i < NUM_FILAS; i++)
      for(int j = 0; j < NUM_COLUMNAS; j++)
         fscanf(archivo, "%d", &matriz[i][j]);
   fclose(archivo); // Cerramos el archivo después de leer los datos.

   // Inicializamos los resultados de cada hilo
   int resultado_hilo[NUM_FILAS] = {0}; // Se inicializa un arreglo de enteros llamado resultado_hilo con tamaño NUM_FILAS y se establece cada elemento en 0. Esto se hace para almacenar los resultados de la multiplicación de cada hilo.

   pthread_t id_hilo[NUM_FILAS]; // Se declara un arreglo de identificadores de hilos llamado id_hilo con tamaño NUM_FILAS. Cada elemento del arreglo representará un hilo creado en el programa.
   DatosHilo datos_hilo[NUM_FILAS];

  printf("Hola, soy el hilo principal y voy a crear 3 hilos...\n");
   for(int i = 0; i < NUM_FILAS; i++){
      datos_hilo[i].numero_hilo = i + 1;
      datos_hilo[i].resultado = &resultado_hilo[i];
      datos_hilo[i].matriz = matriz;
      pthread_create(&id_hilo[i], NULL, Multiplicar_fila, &datos_hilo[i]);
   }
   for(int i = 0; i < NUM_FILAS; i++)
      pthread_join(id_hilo[i], NULL);

   // También lo metemos un un txt de resultados
   FILE *archivo_resultados;
   archivo_resultados = fopen("resultados.txt", "w");
   if (archivo_resultados == NULL) {
      printf("Error al abrir el archivo de resultados\n");
      return 1;
   }
   for(int i = 0; i < NUM_FILAS; i++){
      printf("Hilo %d (%lu): %d\n", i + 1, (unsigned long)id_hilo[i], resultado_hilo[i]);
      fprintf(archivo_resultados, "Hilo %d (%lu): %d\n", i + 1, (unsigned long)id_hilo[i], resultado_hilo[i]);
   }

   fclose(archivo_resultados); // Cerramos el archivo de resultados después de escribir los datos.
   return 0;
}