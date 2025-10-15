// publisher_udp.c
// version automatica para pruebas de trafico udp
// envia muchos mensajes seguidos para observar posibles perdidas o desorden

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
    char tema[60] = "partido1";    // tema fijo para pruebas
    char paquete[BUF_TAM];

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("error creando socket udp");
        exit(1);
    }

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(PUERTO_BROKER);
    broker.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("publicador automatico udp enviando mensajes al broker...\n");

    // enviar 450 mensajes seguidos para generar trafico
    for (int i = 0; i < 10; i++) {
        snprintf(paquete, sizeof(paquete), "PUB:%s:mensaje %d en ", tema, i);

        sendto(sock, paquete, strlen(paquete), 0,
               (struct sockaddr *)&broker, sizeof(broker));

        printf("enviado -> %s\n", paquete);

        
        
    }

    printf("envio finalizado.\n");
    close(sock);
    return 0;
}
