# ==========================================================
# Makefile para la compilación del Capítulo 5 (OpenMPI)
# Asignatura: Cómputo Paralelo - ESCOM IPN
# ==========================================================

CC = mpicc
CFLAGS = -Wall -Wextra -O3
TARGET = openmpi_fourier

all: $(TARGET)

$(TARGET): openmpi_fourier.c
	$(CC) $(CFLAGS) openmpi_fourier.c -o $(TARGET) -lm

clean:
	rm -f $(TARGET) resultados_openmpi.csv