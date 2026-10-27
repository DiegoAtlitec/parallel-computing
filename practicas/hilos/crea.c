#include <stdio.h>

int main() {
    FILE *archivo;
    // Abrir archivo en modo escritura (crea o sobrescribe)
    archivo = fopen("matriz.txt", "w");

    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return 1;
    }

    for(int i =1 ; i <= 9; i ++){
    fprintf(archivo,"%d ",i );
    }

    fprintf(archivo,"\n");
    for(int i =1 ; i <= 17; i +=2){
    fprintf(archivo,"%d ",i );
    }

    fprintf(archivo,"\n");
    for(int i =1 ; i <= 9; i ++){
    fprintf(archivo,"%d ",i*2 );
    }

    // Cerrar el archivo para guardar los cambios
    fclose(archivo);

    printf("Escritura completada exitosamente.\n");

    return 0;
}