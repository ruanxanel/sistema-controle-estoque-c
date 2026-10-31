
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "entrada.h"

// Função auxiliar que lê uma linha digitada no terminal.
//
// static: a função só pode ser chamada dentro de entrada.c.
//
// const char *mensagem: ponteiro para o texto que será exibido.
// char buffer[]: vetor onde a entrada será armazenada.
// size_t tamanho: capacidade do buffer.
//
// Retornos:
//  1 = leitura válida
//  0 = linha muito longa ou incompleta
// -1 = fim da entrada ou erro de leitura
static int lerLinha(const char *mensagem, char buffer[], size_t tamanho) {

    // Exibe a mensagem sem adicionar quebra de linha.
    printf("%s", mensagem);

    // fgets() lê caracteres de stdin até encontrar
    // uma quebra de linha, atingir o limite do buffer
    // ou chegar ao fim da entrada.
    //
    // sizeof(buffer) não é utilizado aqui porque
    // buffer é um parâmetro de função e funciona
    // como ponteiro. Por isso recebemos tamanho.
    //
    // Se fgets() retornar NULL, a leitura terminou
    // ou ocorreu um erro.
    if (fgets(buffer, tamanho, stdin) == NULL)
        return -1;

    // strchr() procura o caractere '\n' dentro do buffer.
    //
    // Retorna um ponteiro para a posição encontrada
    // ou NULL quando o caractere não existe.
    char *fim = strchr(buffer, '\n');

    // Se '\n' não foi encontrado, a entrada não coube
    // completamente na linha lida ou terminou sem '\n'.
    if (fim == NULL) {

        // Descarta os caracteres restantes da entrada
        // até encontrar ENTER ou EOF.
        //
        // Isso evita que o texto restante seja interpretado
        // como uma nova entrada na próxima leitura.
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}

        printf("Entrada muito longa!\n");
        return 0;
    }

    // Substitui a quebra de linha pelo terminador de string.
    //
    // '\0' indica onde a string termina em C.
    //
    // Exemplo:
    // Antes: "Arroz\n"
    // Depois: "Arroz\0"
    *fim = '\0';

    return 1;
}

// Lê uma entrada textual e a converte para int.
//
// strtol() é utilizado em vez de scanf() porque
// permite verificar se a entrada contém caracteres
// inválidos e se o número excede os limites permitidos.
int lerInteiro(const char *mensagem, int *valor) {

    // buffer guarda o texto digitado.
    // fim apontará para onde a conversão terminar.
    char buffer[100], *fim;

    // Reutiliza a função auxiliar para ler a linha.
    int resultado = lerLinha(mensagem, buffer, sizeof(buffer));

    // Se a leitura falhou, devolve o código correspondente.
    if (resultado != 1) return resultado;

    // Zera errno antes da conversão para detectar
    // erros que possam ocorrer em strtol().
    errno = 0;

    // Converte uma string para long.
    //
    // buffer: texto que será convertido.
    // &fim: permite identificar até onde houve conversão.
    // 10: utiliza o sistema decimal (base 10).
    long numero = strtol(buffer, &fim, 10);

    // Ignora espaços em branco após o número.
    //
    // isspace() identifica espaços, tabulações etc.
    // O cast para unsigned char evita comportamento
    // indefinido com valores negativos de char.
    while (isspace((unsigned char)*fim)) fim++;

    // Verifica quatro situações:
    //
    // fim == buffer: nenhum número foi convertido.
    // *fim != '\0': sobraram caracteres inválidos.
    // errno == ERANGE: valor fora do alcance de long.
    // numero fora de INT_MIN/INT_MAX: não cabe em int.
    //
    // O operador || representa OU lógico.
    if (fim == buffer || *fim != '\0' || errno == ERANGE ||
        numero < INT_MIN || numero > INT_MAX) {

        printf("Número inteiro inválido!\n");
        return 0;
    }

    // Converte long para int depois de validar os limites.
    //
    // *valor modifica a variável original recebida
    // através do ponteiro.
    *valor = (int)numero;

    return 1;
}

// Lê uma string e verifica se ela contém texto válido.
int lerTexto(const char *mensagem, char destino[], size_t tamanho) {

    // Impede escrever em um ponteiro nulo ou em um
    // vetor sem espaço suficiente para a leitura esperada.
    if (destino == NULL || tamanho < 2) {
        printf("Destino inválido!\n");
        return 0;
    }

    // Lê a string diretamente no vetor de destino.
    int resultado = lerLinha(mensagem, destino, tamanho);

    if (resultado != 1) return resultado;

    // Percorre o texto procurando pelo menos um
    // caractere que não seja espaço em branco.
    //
    // O laço termina quando encontra '\0',
    // que marca o fim da string.
    for (size_t i = 0; destino[i] != '\0'; i++) {

        // O operador ! significa negação.
        // Se o caractere não for espaço, o texto é válido.
        if (!isspace((unsigned char)destino[i]))
            return 1;
    }

    // Se percorreu tudo sem encontrar texto,
    // a entrada era vazia ou continha apenas espaços.
    printf("O texto não pode estar vazio!\n");
    return 0;
}

// Lê uma entrada textual e converte para double.
//
// Double é utilizado para representar números com
// casas decimais, como os preços dos produtos.
int lerDouble(const char *mensagem, double *valor) {
    char buffer[100], *fim;

    // Realiza a leitura utilizando a função auxiliar.
    int resultado = lerLinha(mensagem, buffer, sizeof(buffer));

    if (resultado != 1) return resultado;

    // Limpa o indicador de erro antes da conversão.
    errno = 0;

    // strtod() converte uma string para double.
    //
    // &fim permite identificar os caracteres que
    // não fizeram parte da conversão.
    double numero = strtod(buffer, &fim);

    // Ignora espaços após o valor decimal.
    while (isspace((unsigned char)*fim)) fim++;

    // Valida:
    //
    // fim == buffer: nenhum número convertido.
    // *fim != '\0': existem caracteres inválidos.
    // errno == ERANGE: resultado fora do intervalo.
    // !isfinite(): resultado infinito ou NaN.
    //
    // Isso evita aceitar valores inválidos nos preços.
    if (fim == buffer || *fim != '\0' || errno == ERANGE || !isfinite(numero)) {
        printf("Número decimal inválido!\n");
        return 0;
    }

    // Armazena o número convertido na variável original.
    *valor = numero;

    return 1;
}
