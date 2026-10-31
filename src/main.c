
#include <stdio.h>   
#include <stdlib.h>

// Bibliotecas do próprio projeto
#include "menu.h"    
#include "entrada.h" 
#include "produto.h" 
#include "arquivo.h" 
#include "cores.h"   

// Função principal: ponto de entrada do programa
int main(void) {

    // opcao: armazena a escolha do usuário no menu
    // total: quantidade de produtos cadastrados
    // capacidade: quantidade de produtos que cabem na memória alocada
    int opcao, total = 0, capacidade = 2;

    // Ponteiro que aponta para um vetor dinâmico de produtos.
    //
    // malloc() reserva memória para 2 estruturas Produto.
    // sizeof(Produto) informa quantos bytes cada estrutura ocupa.
    //
    // Diferentemente de um vetor fixo, esse vetor pode aumentar
    // durante a execução usando realloc(), em produto.c.
    Produto *produtos = malloc(capacidade * sizeof(Produto));

    // Verifica se a memória foi alocada corretamente.
    // malloc() retorna NULL quando a alocação falha.
    if (produtos == NULL) {
        printf(COR_VERMELHO "Erro ao alocar memória!\n" COR_RESET);

        // return 1 indica que o programa terminou com erro.
        return 1;
    }

    // Carrega os produtos salvos anteriormente no arquivo binário.
    //
    // &produtos: endereço do ponteiro, permitindo que a função
    // altere o endereço do vetor caso precise realocar memória.
    //
    // &total: permite atualizar a quantidade de produtos.
    // &capacidade: permite atualizar a capacidade do vetor.
    //
    // O operador ! inverte o resultado lógico:
    // se carregarProdutos() retornar 0, a condição será verdadeira.
    if (!carregarProdutos(&produtos, &total, &capacidade)) {

        // Libera a memória antes de encerrar por erro.
        free(produtos);
        return 1;
    }

    // Executa o menu repetidamente até o usuário escolher sair
    // ou até ocorrer uma condição de encerramento.
    do {

        // Exibe o menu principal.
        menuPrincipal();

        // Lê a opção digitada pelo usuário.
        //
        // &opcao permite que lerInteiro() altere seu valor.
        // O resultado informa se a leitura foi bem-sucedida.
        int resultado = lerInteiro("Escolha uma opção: ", &opcao);

        // resultado == 0: entrada inválida.
        // continue reinicia o laço sem executar o menu escolhido.
        if (resultado == 0) continue;

        // resultado == -1: fim da entrada (EOF).
        // break interrompe o laço.
        if (resultado == -1) break;

        // Executa o perfil escolhido: Administrador ou Usuário.
        //
        // Os endereços são passados para permitir que outras
        // funções modifiquem o vetor e seus controles.
        //
        // Se lerOpcaoPrincipal() retornar 0, o laço termina.
        if (!lerOpcaoPrincipal(opcao, &produtos, &total, &capacidade))
            break;

    } while (opcao != 0);

    // Salva os produtos no arquivo binário antes de encerrar.
    // A função retorna 1 em caso de sucesso ou 0 em caso de erro.
    int sucesso = salvarProdutos(produtos, total);

    // Libera a memória que foi reservada usando malloc().
    // Isso evita que a alocação continue ocupada até o fim
    // da execução do processo.
    free(produtos);

    // Verifica se os produtos foram salvos corretamente.
    if (!sucesso) {
        printf(COR_VERMELHO "Erro ao salvar produtos!\n" COR_RESET);
        return 1;
    }

    // return 0 indica encerramento bem-sucedido.
    return 0;
}
