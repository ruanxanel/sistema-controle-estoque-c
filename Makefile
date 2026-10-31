
# Define o compilador utilizado no projeto.
# gcc é o compilador GNU para a linguagem C.
CC = gcc

# Define as opções de compilação.
# -Wall: habilita vários avisos do compilador.
# -Wextra: habilita avisos adicionais.
# -Iinclude: informa onde procurar os arquivos .h.
CFLAGS = -Wall -Wextra -Iinclude

# wildcard procura todos os arquivos .c dentro de src/.
# Assim, novos arquivos .c são incluídos automaticamente.
SRC = $(wildcard src/*.c)

# Define o caminho e o nome do programa executável.
TARGET = build/estoque

# Alvo padrão executado quando digitamos apenas "make".
# Depende da geração do arquivo executável.
all: $(TARGET)

# Regra responsável pela compilação.
#
# O executável depende dos arquivos .c e .h.
# Se algum deles for modificado, o Make recompila o programa.
#
# $(SRC) representa os arquivos .c encontrados.
# $(TARGET) representa o executável.
$(TARGET): $(SRC) $(wildcard include/*.h)

# Cria a pasta build caso ela ainda não exista.
	mkdir -p build

# Compila todos os arquivos .c em um único executável.
# -o define o nome e o caminho do arquivo gerado.
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Compila o projeto, caso necessário, e executa o programa.
run: all
	./$(TARGET)

# Remove a pasta build e o executável compilado.
# Os arquivos de código e os dados em data/ são preservados.
clean:
	rm -rf build

# Declara que all, run e clean são comandos,
# não nomes de arquivos que precisam ser gerados.
.PHONY: all run clean
