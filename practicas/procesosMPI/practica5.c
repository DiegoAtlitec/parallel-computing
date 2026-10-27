// Programa básico de MPI para que cada NUCLEO haga una operación matemática distinta
// Yo tengo 10 NUCLEOS
#include<stdio.h>
#include<mpi.h>

/*
Se trabajará la siguiente matriz de 7 filas y 3 columnas, donde cada fila representa un núcleo y cada columna representa una operación matemática distinta:
1 2 3
4 5 6
7 8 9
9 6 3
-6 -9 -12
5 10 15
7 14 21

Las operaciones serán las siguientes:
1. a+b+c
2. a-b+c
3. a*b*c
4. a*(b+c)
5. a*b-b*c
6. c*(a+b)**2
7. a**2-b**2+c**3

*/

int main(int argc, char **argv){
    int identificador, total_nucleos;
	MPI_Status estatus;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&identificador);
    MPI_Comm_size(MPI_COMM_WORLD,&total_nucleos); // esta variable debería de ser 8

    // Inicializamos por defecto los valores abc como 0
    int a=0, b=0, c=0, resultado=0;
    // Inicializamos un vector linea
    int linea[3];

	if (identificador==0)
	{
        FILE *archivo_resultados;
        archivo_resultados = fopen("resultados.txt", "w");
        if (archivo_resultados == NULL) {
            printf("Error al abrir el archivo de resultados\n");
            return 1;
            MPI_Finalize();
        }
        // Este nodo padre mandará a los demás sus a, b, c.
        // Desde la siguiente matriz de 7 filas y 3 columnas.
        int matriz_og[7][3] = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9},
            {9, 6, 3},
            {-6, -9, -12},
            {5, 10, 15},
            {7, 14, 21}
        };

        for(int i=1; i<total_nucleos; i++){
            linea[0] = matriz_og[i-1][0];
            linea[1] = matriz_og[i-1][1];
            linea[2] = matriz_og[i-1][2];
            MPI_Send(linea,3,MPI_INT,i,100,MPI_COMM_WORLD);
        }

        for(int i=1; i<total_nucleos; i++){
            MPI_Recv(&resultado,1,MPI_INT,i,200,MPI_COMM_WORLD,&estatus);
            printf("%d\n", resultado);
            fprintf(archivo_resultados, "%d\n", resultado);
        }
        fclose(archivo_resultados);
	}
	else if (identificador==1)
	{
        MPI_Recv(linea,3,MPI_INT,0,100,MPI_COMM_WORLD,&estatus);
        a = linea[0];
        b = linea[1];
        c = linea[2];
        // Aqui se realiza la operación
        resultado = a+b+c;
        MPI_Send(&resultado,1,MPI_INT,0,200,MPI_COMM_WORLD);
	}
    else if (identificador==2)
    {
        MPI_Recv(linea,3,MPI_INT,0,100,MPI_COMM_WORLD,&estatus);
        a = linea[0];
        b = linea[1];
        c = linea[2];
        // Aqui se realiza la operación
        resultado = a-b+c;
        MPI_Send(&resultado,1,MPI_INT,0,200,MPI_COMM_WORLD);
    }
    else if (identificador==3)
    {
        MPI_Recv(linea,3,MPI_INT,0,100,MPI_COMM_WORLD,&estatus);
        a = linea[0];
        b = linea[1];
        c = linea[2];
        // Aqui se realiza la operación
        resultado = a*b*c;
        MPI_Send(&resultado,1,MPI_INT,0,200,MPI_COMM_WORLD);
    }
    else if (identificador==4)
    {
        MPI_Recv(linea,3,MPI_INT,0,100,MPI_COMM_WORLD,&estatus);
        a = linea[0];
        b = linea[1];
        c = linea[2];
        // Aqui se realiza la operación
        resultado = a*(b+c);
        MPI_Send(&resultado,1,MPI_INT,0,200,MPI_COMM_WORLD);
    }
    else if (identificador==5)
    {
        MPI_Recv(linea,3,MPI_INT,0,100,MPI_COMM_WORLD,&estatus);
        a = linea[0];
        b = linea[1];
        c = linea[2];
        // Aqui se realiza la operación
        resultado = (a*b) - (b*c);
        MPI_Send(&resultado,1,MPI_INT,0,200,MPI_COMM_WORLD);
    }
    else if (identificador==6)
    {
        MPI_Recv(linea,3,MPI_INT,0,100,MPI_COMM_WORLD,&estatus);
        a = linea[0];
        b = linea[1];
        c = linea[2];
        // Aqui se realiza la operación
        resultado = c * (a+b) * (a+b);
        MPI_Send(&resultado,1,MPI_INT,0,200,MPI_COMM_WORLD);
    }
    else // identificador == 7
    {
        MPI_Recv(linea,3,MPI_INT,0,100,MPI_COMM_WORLD,&estatus);
        a = linea[0];
        b = linea[1];
        c = linea[2];
        // Aqui se realiza la operación
        resultado = (a*a)-(b*b)+(c*c*c);
        MPI_Send(&resultado,1,MPI_INT,0,200,MPI_COMM_WORLD);
    }
    MPI_Finalize();
}