
#ifndef ENTRADA_H
#define ENTRADA_H

// Evita a inclusão múltipla deste arquivo.

// Biblioteca que define o tipo size_t,
// utilizado para representar tamanhos de memória.
#include <stddef.h>

// Lê um número inteiro digitado pelo usuário.
//
// mensagem: texto exibido antes da leitura.
// valor: ponteiro para armazenar o inteiro recebido.
//
// Retornos:
//  1 = leitura válida
//  0 = entrada inválida
// -1 = fim da entrada (EOF)
int lerInteiro(const char *mensagem, int *valor);

// Lê um texto e verifica se ele não está vazio.
//
// destino[]: vetor de caracteres onde o texto será armazenado.
// tamanho: capacidade do vetor, incluindo o caractere '\0'.
//
// Retorna 1, 0 ou -1, seguindo a mesma convenção.
int lerTexto(const char *mensagem, char destino[], size_t tamanho);

// Lê e converte um número decimal para double.
//
// valor: endereço da variável que receberá o número.
//
// Retorna 1, 0 ou -1.
int lerDouble(const char *mensagem, double *valor);

#endif
