#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server IP> <server port>\n", argv[0]);
        exit(1);
    }
    
    int client_fd = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE] = {0};

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket failed \n");
        exit(1);
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("inet_pton failed\n");
        exit(1);
    }

    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("connect Failed\n");
        exit(1);
    }

    printf("connected to server\n");

    while (1) {
        if (read(client_fd, buffer, BUF_SIZE) == 0) {
            printf("read faild\n");
            break;
        }

        printf("Received message from Server: %s\n", buffer);
        memset(buffer, 0, BUF_SIZE);
    }

    close(client_fd);
    return 0;
}
