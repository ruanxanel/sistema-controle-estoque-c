#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct {
    int codigo;
    char nome[100];
} Cliente;

void cadastrarCliente(Cliente **clientes, int *total, int *capacidade);

void listarClientes(Cliente clientes[], int total);

int localizarClientePorCodigo(Cliente clientes[], int total, int codigo);

void buscarCliente(Cliente clientes[], int total);

#endif