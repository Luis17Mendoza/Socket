// cliente_udp.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5001
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Crear socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error al crear el socket");
        return 1;
    }

    // Configurar la dirección del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Dirección IP del servidor
    server_addr.sin_port = htons(PORT);

    printf("Cliente UDP - Escriba un mensaje para enviar (deje vacío para salir).\n");
    while (1) {
        printf("Mensaje: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  // Eliminar el salto de línea

        if (strlen(buffer) == 0) break;

        // Enviar mensaje al servidor
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        // Recibir respuesta del servidor
        int received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        buffer[received] = '\0';
        printf("Respuesta del servidor: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
