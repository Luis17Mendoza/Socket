// servidor_tcp.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5002
#define BUFFER_SIZE 1024
#define SERVER_NAME "ServidorTCP"

void invertirCadena(char *cadena) {
    int len = strlen(cadena);
    for (int i = 0; i < len / 2; i++) {
        char temp = cadena[i];
        cadena[i] = cadena[len - i - 1];
        cadena[len - i - 1] = temp;
    }
}

int main() {
    int sockfd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_addr_len = sizeof(client_addr);

    // Crear socket TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error al crear el socket");
        return 1;
    }

    // Configurar la dirección del servidor para escuchar solo en 127.0.0.1
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Escuchar solo en localhost
    server_addr.sin_port = htons(PORT);

    // Enlazar el socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error en bind");
        close(sockfd);
        return 1;
    }

    // Escuchar conexiones
    listen(sockfd, 1);
    printf("[%s] Servidor TCP escuchando solo en 127.0.0.1:%d\n", SERVER_NAME, PORT);

    while (1) {
        // Aceptar una conexión
        new_sock = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (new_sock < 0) {
            perror("Error en accept");
            continue;
        }
        printf("[%s] Conexión establecida con el cliente\n", SERVER_NAME);

        // Bucle para recibir múltiples mensajes del cliente
        while (1) {
            int received = recv(new_sock, buffer, BUFFER_SIZE, 0);
            if (received < 0) {
                perror("Error al recibir el mensaje");
                break;  // Sale del bucle si hay un error de recepción
            } else if (received == 0) {
                printf("[%s] El cliente ha cerrado la conexión.\n", SERVER_NAME);
                break;  // Sale del bucle si el cliente cierra la conexión
            }

            // Procesar el mensaje recibido
            buffer[received] = '\0';
            printf("[%s] Mensaje recibido: %s\n", SERVER_NAME, buffer);

            // Invertir el mensaje
            invertirCadena(buffer);
            strcat(buffer, " - ");
            strcat(buffer, SERVER_NAME);

            // Enviar respuesta al cliente
            send(new_sock, buffer, strlen(buffer), 0);
            printf("[%s] Mensaje enviado: %s\n", SERVER_NAME, buffer);
        }

        printf("[%s] Conexión cerrada con el cliente\n", SERVER_NAME);
        close(new_sock);  // Cierra la conexión actual y espera otro cliente
    }

    close(sockfd);
    return 0;
}

