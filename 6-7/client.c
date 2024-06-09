#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <IP> <Port> <ClientID> <Proposal>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    int client_id = atoi(argv[3]);
    char *proposal = argv[4];

    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    printf("Client %d ready to send to server.\n", client_id);

    // Отправка предложения
    if (sendto(client_socket, proposal, strlen(proposal), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("sendto");
        close(client_socket);
        return EXIT_FAILURE;
    }

    // Ожидание ответа
    socklen_t addr_len = sizeof(server_addr);
    int bytes_received = recvfrom(client_socket, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&server_addr, &addr_len);
    if (bytes_received <= 0)  {
        perror("recvfrom");
        close(client_socket);
        return EXIT_FAILURE;
    }

    buffer[bytes_received] = '\0'; // Null-terminate the received string

    printf("Client %d received: %s\n", client_id, buffer);

    if (strcmp(buffer, "Accepted") == 0) {
        printf("Студент %d выбран студенткой.\n", client_id);
    } else if (strcmp(buffer, "Rejected") == 0) {
        int random_response = rand() % 2;
        if (random_response == 0) {
            printf("О нет! Студент %d в печали.\n", client_id);
        } else {
            printf("Студент %d сохраняет спокойствие несмотря на отказ.\n", client_id);
        }
    }

    close(client_socket);
    return EXIT_SUCCESS;
}
