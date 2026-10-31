
#ifndef CORES_H
#define CORES_H

// Include guard: impede que as macros sejam definidas
// novamente caso este arquivo seja incluído várias vezes.
//
// #ifndef verifica se CORES_H ainda não foi definido.
// #define CORES_H define essa identificação.
// #endif encerra a proteção.

// As macros abaixo definem sequências de escape ANSI,
// utilizadas para alterar a cor dos textos no terminal.
//
// \033 representa o caractere ESC (escape).
// [ inicia uma sequência de controle ANSI.
// 1 ativa a intensidade de texto em negrito.
// 31, 32, 33 e 34 representam as cores.
// m finaliza a sequência de formatação.

// Vermelho: utilizado em mensagens de erro.
#define COR_VERMELHO "\033[1;31m"

// Verde: utilizado em mensagens de sucesso.
#define COR_VERDE "\033[1;32m"

// Amarelo: utilizado em avisos e alertas de estoque.
#define COR_AMARELO "\033[1;33m"

// Azul: utilizado nos títulos dos menus e tabelas.
#define COR_AZUL "\033[1;34m"

// Restaura a formatação padrão do terminal.
#define COR_RESET "\033[0m"

#endif
