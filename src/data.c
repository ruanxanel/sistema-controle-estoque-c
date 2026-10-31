
#include <stdio.h>
#include "data.h"

// Obtém a data e hora atuais do computador
// e transforma essas informações em uma string.
//
// A função recebe o vetor onde a data será armazenada
// e o tamanho disponível nesse vetor.
int obterDataHora(
    char destino[],
    size_t tamanho
) {

    // time_t é um tipo definido em <time.h>
    // para representar valores de tempo.
    //
    // time(NULL) obtém o horário atual do sistema.
    // Em sistemas comuns, esse valor representa
    // os segundos decorridos desde 01/01/1970,
    // mas a representação exata depende da plataforma.
    time_t agora = time(NULL);

    // localtime() converte o valor de time_t
    // em uma estrutura com os componentes da data.
    //
    // &agora passa o endereço da variável.
    //
    // struct tm possui campos como:
    // tm_mday: dia do mês
    // tm_mon: mês (0 a 11)
    // tm_year: anos desde 1900
    // tm_hour: hora
    // tm_min: minuto
    //
    // O ponteiro data aponta para a estrutura
    // retornada por localtime().
    struct tm *data = localtime(&agora);

    // Verifica se a conversão da data falhou.
    //
    // NULL significa que não foi possível obter
    // um ponteiro válido para a estrutura.
    if (data == NULL) {
        printf("Erro ao obter data e hora!\n");
        return 0;
    }

    // strftime() formata a data e a hora como texto.
    //
    // destino: vetor onde será armazenado o resultado.
    // tamanho: quantidade máxima de bytes disponíveis.
    // "%d/%m/%Y %H:%M": formato escolhido.
    // data: estrutura que contém as informações.
    //
    // A função retorna a quantidade de caracteres
    // gravados, sem contar o '\0'.
    //
    // Exemplo de resultado:
    // 31/10/2026 16:18
    size_t resultado = strftime(
        destino,
        tamanho,
        "%d/%m/%Y %H:%M",
        data
    );

    // Se strftime() retornar 0, o texto não pôde
    // ser armazenado corretamente no espaço disponível.
    if (resultado == 0) {
        printf("Erro ao formatar data e hora!\n");
        return 0;
    }

    // Retorna 1 indicando que a operação funcionou.
    return 1;
}
