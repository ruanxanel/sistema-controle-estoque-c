
#ifndef DATA_H
#define DATA_H

// Evita que o conteúdo deste arquivo seja incluído
// mais de uma vez durante a compilação.

// Define size_t, utilizado para representar tamanhos.
#include <stddef.h>

// Disponibiliza os tipos e funções relacionados
// à manipulação de datas e horários.
#include <time.h>

// Declara a função responsável por obter a data e
// a hora atuais e armazená-las como texto.
//
// destino[]: vetor de caracteres que receberá a data.
// tamanho: capacidade do vetor, incluindo o '\0'.
//
// Retornos:
// 1 = data e hora obtidas e formatadas corretamente.
// 0 = ocorreu um erro.
int obterDataHora(
    char destino[],
    size_t tamanho
);

#endif
