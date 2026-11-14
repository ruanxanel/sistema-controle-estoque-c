#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "cliente.h"
#include "entrada.h"
#include "cores.h"

int localizarClientePorCodigo(Cliente clientes[], int total, int codigo) {
    for (int i = 0; i < total; i++) {
        if (clientes[i].codigo == codigo)
            return i;
    }

    return -1;
}

void cadastrarCliente(Cliente **clientes, int *total, int *capacidade) {
    Cliente c;

    if (lerInteiro("Código do cliente: ", &c.codigo) != 1 || c.codigo <= 0) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    if (localizarClientePorCodigo(*clientes, *total, c.codigo) != -1) {
        printf(COR_VERMELHO "Cliente já cadastrado!\n" COR_RESET);
        return;
    }

    if (lerTexto("Nome do cliente: ", c.nome, sizeof(c.nome)) != 1) {
        printf(COR_VERMELHO "Nome inválido!\n" COR_RESET);
        return;
    }

    if (*total == *capacidade) {
        if (*capacidade <= 0 || *capacidade > INT_MAX / 2) {
            printf(COR_VERMELHO "Limite de clientes atingido!\n" COR_RESET);
            return;
        }

        int novaCapacidade = *capacidade * 2;

        Cliente *temp = realloc(
            *clientes,
            (size_t)novaCapacidade * sizeof(Cliente)
        );

        if (temp == NULL) {
            printf(COR_VERMELHO "Erro ao alocar memória!\n" COR_RESET);
            return;
        }

        *clientes = temp;
        *capacidade = novaCapacidade;
    }

    (*clientes)[(*total)++] = c;

    printf(COR_VERDE "Cliente %s cadastrado com sucesso!\n" COR_RESET, c.nome);
}

void listarClientes(Cliente clientes[], int total) {
    if (total == 0) {
        printf(COR_AMARELO "Nenhum cliente cadastrado!\n" COR_RESET);
        return;
    }

    printf(COR_AZUL "\n========================================\n");
    printf("          CLIENTES CADASTRADOS\n");
    printf("========================================\n" COR_RESET);

    printf("%-8s %-20s\n", "Código", "Nome");
    printf("----------------------------------------\n");

    for (int i = 0; i < total; i++) {
        printf("%-8d %-20.20s\n",
            clientes[i].codigo, clientes[i].nome);
    }

    printf("----------------------------------------\n");
    printf("Total de clientes: %d\n", total);
}

void buscarCliente(Cliente clientes[], int total) {
    int codigo;

    if (lerInteiro("Código do cliente: ", &codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    int indice = localizarClientePorCodigo(clientes, total, codigo);

    if (indice == -1) {
        printf(COR_VERMELHO "Cliente não encontrado!\n" COR_RESET);
        return;
    }

    printf("\nCódigo: %d\nNome: %s\n",
        clientes[indice].codigo, clientes[indice].nome);
}