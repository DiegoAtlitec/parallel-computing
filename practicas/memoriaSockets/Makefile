# Variables
CC = gcc
CFLAGS = -Wall
TARGET_SERVER = Servidor_D
SRC_SERVER = Servidor_Datagrama.c
TARGET_CLIENT = Cliente_D
SRC_CLIENT = Cliente_Datagrama.c

# Regla por defecto
all: $(TARGET_SERVER)

# Compilar Servidor
$(TARGET_SERVER): $(SRC_SERVER)
	$(CC) $(CFLAGS) $(SRC_SERVER) -o $(TARGET_SERVER)

# Compilar Cliente (si cuentas con el archivo fuente correspondiente)
$(TARGET_CLIENT): $(SRC_CLIENT)
	$(CC) $(CFLAGS) $(SRC_CLIENT) -o $(TARGET_CLIENT)

# Regla de ejecución
run:
ifeq ($(ROLE),servidor)
	./$(TARGET_SERVER)
else ifeq ($(ROLE),cliente)
	./$(TARGET_CLIENT)
else
	@echo "Uso: make run ROLE=servidor o make run ROLE=cliente"
endif

# Limpiar ejecutables
clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT)