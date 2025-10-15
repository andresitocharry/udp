// subscriber_udp.c
// programa que se suscribe a un tema y recibe mensajes udp del broker
// parte del laboratorio 3 de redes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PUERTO_BROKER 8081
#define BUF_TAM 1024

int main() {
    int sock;
    struct sockaddr_in broker;
    char buffer[BUF_TAM];
    char tema[60];

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("error creando socket udp");
        exit(1);
    }

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(PUERTO_BROKER);
    broker.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("tema al que quieres suscribirte: ");
    fgets(tema, sizeof(tema), stdin);
    tema[strcspn(tema, "\n")] = 0;

    char paquete[BUF_TAM];
    snprintf(paquete, sizeof(paquete), "SUB:%s", tema);

    sendto(sock, paquete, strlen(paquete), 0,
           (struct sockaddr *)&broker, sizeof(broker));

    printf("suscrito al tema '%s', esperando mensajes...\n", tema);

    while (1) {
        int n = recvfrom(sock, buffer, BUF_TAM, 0, NULL, NULL);
        if (n <= 0) continue;
        buffer[n] = '\0';
        printf("actualizacion [%s]: %s\n", tema, buffer);
    }

    close(sock);
    return 0;
}
