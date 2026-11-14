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