// broker_udp.c
// laboratorio 3 - version udp del broker
// este programa recibe mensajes de los publicadores y los reenvia a los subscriptores

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PUERTO_BROKER 8081
#define MAX_SUBS 100
#define BUF_TAM 1024

// estructura pa guardar los subscriptores
typedef struct {
    char tema[60];
    struct sockaddr_in dir;
} Subscripcion;

Subscripcion listaSubs[MAX_SUBS];
int totalSubs = 0;

// agrega un nuevo subscriptor
void agregar_subs(const char *tema, struct sockaddr_in cli) {
    if (totalSubs >= MAX_SUBS) {
        printf("maximo de subscriptores alcanzado\n");
        return;
    }
    strcpy(listaSubs[totalSubs].tema, tema);
    listaSubs[totalSubs].dir = cli;
    totalSubs++;
    printf("nuevo subscriptor al tema '%s' desde %s:%d\n",
           tema, inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
}

// envia mensaje a todos los subs de ese tema
void enviar_a_subs(int sock, const char *tema, const char *msj) {
    for (int i = 0; i < totalSubs; i++) {
        if (strcmp(listaSubs[i].tema, tema) == 0) {
            sendto(sock, msj, strlen(msj), 0,
                   (struct sockaddr *)&listaSubs[i].dir, sizeof(listaSubs[i].dir));
        }
    }
}

int main() {
    int sock;
    struct sockaddr_in servidor, cliente;
    char buffer[BUF_TAM];
    socklen_t len = sizeof(cliente);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("error creando socket udp");
        exit(1);
    }

    memset(&servidor, 0, sizeof(servidor));
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(PUERTO_BROKER);
    servidor.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&servidor, sizeof(servidor)) < 0) {
        perror("error en bind");
        close(sock);
        exit(1);
    }

    printf("broker udp escuchando en puerto %d...\n", PUERTO_BROKER);

    while (1) {
        int n = recvfrom(sock, buffer, BUF_TAM, 0, (struct sockaddr *)&cliente, &len);
        if (n <= 0) continue;
        buffer[n] = '\0';

        char *comando = strtok(buffer, ":");
        if (!comando) continue;

        if (strcmp(comando, "SUB") == 0) {
            char *tema = strtok(NULL, "");
            if (tema) agregar_subs(tema, cliente);
        } else if (strcmp(comando, "PUB") == 0) {
            char *tema = strtok(NULL, ":");
            char *mensaje = strtok(NULL, "");
            if (tema && mensaje) {
                printf("publicacion recibida en '%s': %s\n", tema, mensaje);
                enviar_a_subs(sock, tema, mensaje);
            }
        }
    }

    close(sock);
    return 0;
}
