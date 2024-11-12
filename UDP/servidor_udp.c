// servidor_udp.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5001
#define BUFFER_SIZE 1024
#define SERVER_NAME "ServidorUDP"

void invertirCadena(char *cadena) {
    int len = strlen(cadena);
    for (int i = 0; i < len / 2; i++) {
        char temp = cadena[i];
        cadena[i] = cadena[len - i - 1];
        cadena[len - i - 1] = temp;
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_addr_len = sizeof(client_addr);

    // Crear socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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

    printf("[%s] Servidor UDP escuchando solo en 127.0.0.1:%d\n", SERVER_NAME, PORT);

    while (1) {
        // Recibir mensaje del cliente
        int received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (received < 0) {
            perror("Error al recibir el mensaje");
            continue;
        }
        buffer[received] = '\0';
        printf("[%s] Mensaje recibido: %s\n", SERVER_NAME, buffer);

        // Invertir el mensaje
        invertirCadena(buffer);
        strcat(buffer, " - ");
        strcat(buffer, SERVER_NAME);

        // Enviar respuesta al cliente
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, client_addr_len);
        printf("[%s] Mensaje enviado: %s\n", SERVER_NAME, buffer);
    }

    close(sockfd);
    return 0;
}

