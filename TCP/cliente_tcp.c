// cliente_tcp.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5002
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Crear socket TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error al crear el socket");
        return 1;
    }

    // Configurar la dirección del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Dirección IP del servidor
    server_addr.sin_port = htons(PORT);

    // Conectarse al servidor
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error en connect");
        close(sockfd);
        return 1;
    }
    printf("Conexión establecida con el servidor TCP.\n");

    while (1) {
        // Solicitar mensaje al usuario
        printf("Introduce un mensaje (deja vacío para salir): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  // Eliminar el salto de línea

        // Terminar si el usuario envía un mensaje vacío
        if (strlen(buffer) == 0) {
            printf("Cerrando la conexión...\n");
            break;
        }

        // Enviar mensaje al servidor
        if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
            perror("Error al enviar el mensaje");
            break;
        }

        // Recibir respuesta del servidor
        int received = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (received < 0) {
            perror("Error al recibir la respuesta del servidor");
            break;
        } else if (received == 0) {
            printf("El servidor ha cerrado la conexión.\n");
            break;
        }

        // Mostrar la respuesta del servidor
        buffer[received] = '\0';
        printf("Respuesta del servidor: %s\n", buffer);
    }

    // Cerrar el socket
    close(sockfd);
    printf("Conexión finalizada.\n");
    return 0;
}

