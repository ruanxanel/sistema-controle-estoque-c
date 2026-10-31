
#include <stdio.h>

#include "menu.h"
#include "produto.h"
#include "estoque.h"
#include "arquivo.h"
#include "entrada.h"
#include "cores.h"

// Limpa a área visível do terminal.
//
// static restringe o acesso à função ao arquivo menu.c.
//
// \033 inicia uma sequência de escape ANSI.
// [2J limpa a tela.
// [H posiciona o cursor no início da tela.
//
// fflush(stdout) força o envio da saída pendente
// ao terminal.
static void limparTela(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

// Aguarda o usuário pressionar ENTER antes
// de voltar para a exibição do menu.
//
// getchar() lê um caractere da entrada padrão.
//
// O laço descarta os caracteres recebidos até
// encontrar '\n' (ENTER) ou EOF (fim da entrada).
static void pausarTela(void) {
    int c;

    printf("\nPressione ENTER para continuar...");
    fflush(stdout);

    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Exibe o menu correspondente ao perfil informado.
//
// perfil == 0: menu principal
// perfil == 1: Administrador
// perfil == 2: Usuário
//
// Essa função reúne a apresentação dos menus
// para evitar funções separadas com código repetido.
static void exibirMenu(int perfil) {

    // Limpa o terminal antes de exibir o menu.
    limparTela();

    // COR_AZUL e COR_RESET são definidos em cores.h.
    // Eles controlam as cores usando códigos ANSI.
    printf(COR_AZUL);
    printf("============================================\n");

    // Seleciona o título do menu.
    if (perfil == 0)
        printf("         SISTEMA DE CONTROLE DE ESTOQUE\n");
    else if (perfil == 1)
        printf("             PAINEL ADMINISTRADOR\n");
    else
        printf("                PAINEL USUÁRIO\n");

    printf("============================================\n");
    printf(COR_RESET);

    // Menu principal: permite escolher um perfil
    // ou encerrar o programa.
    if (perfil == 0) {
        printf("\n[1] Administrador\n");
        printf("[2] Usuário\n");
        printf("[0] Sair\n");

    // Administrador: possui todas as operações
    // de gerenciamento de produtos e estoque.
    } else if (perfil == 1) {
        printf("\n[1] Cadastrar produto\n");
        printf("[2] Listar produtos\n");
        printf("[3] Buscar produto\n");
        printf("[4] Editar produto\n");
        printf("[5] Excluir produto\n");
        printf("\n[6] Registrar entrada\n");
        printf("[7] Registrar saída\n");
        printf("[8] Produtos com estoque baixo\n");
        printf("[9] Histórico de movimentações\n");
        printf("\n[0] Voltar ao menu principal\n");

    // Usuário: possui acesso à consulta dos produtos,
    // movimentações de estoque e histórico.
    //
    // Não possui opções para cadastrar, editar
    // ou excluir produtos.
    } else {
        printf("\n[1] Listar produtos\n");
        printf("[2] Buscar produto\n");
        printf("[3] Registrar entrada\n");
        printf("[4] Registrar saída\n");
        printf("[5] Produtos com estoque baixo\n");
        printf("[6] Histórico de movimentações\n");
        printf("\n[0] Voltar ao menu principal\n");
    }

    printf("\n--------------------------------------------\n");
}

// Executa a operação selecionada pelo usuário.
//
// perfil: informa se o menu é de Administrador ou Usuário.
// opcao: número da operação escolhida.
//
// Produto **produtos: permite que cadastrarProduto()
// realoque o vetor, caso necessário.
//
// int *total: permite alterar a quantidade de produtos.
// int *capacidade: permite atualizar a capacidade do vetor.
static void executarOpcao(
    int perfil,
    int opcao,
    Produto **produtos,
    int *total,
    int *capacidade
) {

    // Quando perfil == 1, executa as opções
    // disponíveis para o Administrador.
    if (perfil == 1) {

        // switch compara opcao com cada case.
        //
        // break encerra o switch após executar
        // a operação correspondente.
        switch (opcao) {
            case 1: cadastrarProduto(produtos, total, capacidade); break;
            case 2: listarProdutos(*produtos, *total); break;
            case 3: buscarProduto(*produtos, *total); break;
            case 4: editarProduto(*produtos, *total); break;
            case 5: excluirProduto(*produtos, total); break;
            case 6: registrarEntrada(*produtos, *total); break;
            case 7: registrarSaida(*produtos, *total); break;
            case 8: listarEstoqueBaixo(*produtos, *total); break;
            case 9: listarMovimentacoes(); break;

            // default é executado quando nenhuma
            // das opções anteriores corresponde.
            default: printf(COR_VERMELHO "Opção inválida!\n" COR_RESET);
        }

    } else {

        // Opções do Usuário.
        //
        // Os números são diferentes dos utilizados
        // no painel Administrador.
        //
        // O Usuário não recebe acesso às funções
        // cadastrarProduto(), editarProduto()
        // e excluirProduto() por este menu.
        switch (opcao) {
            case 1: listarProdutos(*produtos, *total); break;
            case 2: buscarProduto(*produtos, *total); break;
            case 3: registrarEntrada(*produtos, *total); break;
            case 4: registrarSaida(*produtos, *total); break;
            case 5: listarEstoqueBaixo(*produtos, *total); break;
            case 6: listarMovimentacoes(); break;
            default: printf(COR_VERMELHO "Opção inválida!\n" COR_RESET);
        }
    }

    // Aguarda ENTER após a operação para permitir
    // que o usuário leia as informações exibidas.
    pausarTela();
}

// Função pública chamada pelo main.c.
//
// Reutiliza exibirMenu(), passando o valor 0,
// que representa o menu principal.
void menuPrincipal(void) {
    exibirMenu(0);
}

// Controla a escolha do perfil e a navegação
// dentro dos submenus.
int lerOpcaoPrincipal(
    int opcao,
    Produto **produtos,
    int *quantidadeProdutos,
    int *capacidade
) {

    // A opção 0 corresponde a sair do sistema.
    //
    // Retorna 1 para indicar que a função
    // terminou normalmente.
    //
    // O encerramento do programa acontece depois,
    // no laço do main.c.
    if (opcao == 0) {
        printf(COR_VERDE "Saindo...\n" COR_RESET);
        return 1;
    }

    // Verifica se o perfil informado é válido.
    //
    // O operador && significa E lógico.
    //
    // Se a opção não for 1 nem 2, exibe erro
    // e retorna ao menu principal.
    if (opcao != 1 && opcao != 2) {
        printf(COR_VERMELHO "Opção inválida!\n" COR_RESET);
        return 1;
    }

    // Armazena a opção escolhida dentro do submenu.
    //
    // Inicia com -1 para indicar que nenhuma
    // opção válida foi selecionada ainda.
    int escolha = -1;

    // O do while executa o bloco pelo menos uma vez.
    // O submenu permanece ativo até a opção 0.
    do {

        // Exibe o menu do perfil escolhido.
        exibirMenu(opcao);

        // Solicita uma opção.
        //
        // &escolha envia o endereço para lerInteiro(),
        // permitindo que a função preencha a variável.
        int resultado = lerInteiro("Escolha uma opção: ", &escolha);

        // -1 indica EOF ou erro de leitura.
        //
        // Retornar 0 informa ao main.c que
        // a navegação deve ser encerrada.
        if (resultado == -1) return 0;

        // 0 indica entrada inválida.
        //
        // continue reinicia o laço sem executar
        // nenhuma operação.
        if (resultado == 0) continue;

        // O valor 0 no submenu significa voltar
        // ao menu principal.
        if (escolha == 0) break;

        // Executa a funcionalidade selecionada.
        //
        // Passa os mesmos endereços recebidos do main.c,
        // permitindo que cadastro e exclusão alterem
        // o vetor ou o contador de produtos.
        executarOpcao(
            opcao,
            escolha,
            produtos,
            quantidadeProdutos,
            capacidade
        );

    } while (escolha != 0);

    // Executado quando o usuário escolhe voltar.
    printf(COR_VERDE "Voltando ao menu principal...\n" COR_RESET);

    return 1;
}
