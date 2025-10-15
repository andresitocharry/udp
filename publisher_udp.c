// publisher_udp.c
// este programa actua como publicador udp y envia mensajes al broker
// cada mensaje se manda con un tema especifico

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
    char tema[60], mensaje[256], paquete[BUF_TAM];

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("error creando socket udp");
        exit(1);
    }

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(PUERTO_BROKER);
    broker.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("publicador udp listo. escribe 'salir' para terminar.\n");

    while (1) {
        printf("\ntema: ");
        fgets(tema, sizeof(tema), stdin);
        tema[strcspn(tema, "\n")] = 0;
        if (strcmp(tema, "salir") == 0) break;

        printf("mensaje: ");
        fgets(mensaje, sizeof(mensaje), stdin);
        mensaje[strcspn(mensaje, "\n")] = 0;

        snprintf(paquete, sizeof(paquete), "PUB:%s:%s", tema, mensaje);

        sendto(sock, paquete, strlen(paquete), 0,
               (struct sockaddr *)&broker, sizeof(broker));

        printf("mensaje enviado al broker en tema '%s'\n", tema);
    }

    close(sock);
    return 0;
}
