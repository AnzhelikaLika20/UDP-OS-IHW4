#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Server IP> <Monitor Port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);

    int monitor_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    if ((monitor_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    if (bind(monitor_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        return EXIT_FAILURE;
    }

    printf("Monitor listening on %s:%d\n", ip, port);

    socklen_t client_addr_len = sizeof(client_addr);
    while (1) {
        int bytes_received = recvfrom(monitor_socket, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (bytes_received <= 0) {
            perror("recvfrom");
            break;
        }

        buffer[bytes_received] = '\0'; // Null-terminate the received string
        printf("%s", buffer);
        if (strcmp(buffer, "stop") == 0) {
            break;
        }
    }

    close(monitor_socket);
    return EXIT_SUCCESS;
}
