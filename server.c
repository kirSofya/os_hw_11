#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage:  %s <Server Port>\n", argv[0]);
        exit(1);
    }
    int server_fd;
    int client_fd[2];
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr[2];
    char buffer[BUF_SIZE] = {0};
    int val;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("socket failed");
        exit(1);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(1);
    }
    
    printf("Server\'s ip address: %s\n", inet_ntoa(server_addr.sin_addr));
    printf("Server\'s port: %d\n", ntohs(server_addr.sin_port));
    
    if (listen(server_fd, 2) < 0) {
        perror("listen failed");
        exit(1);
    }
    
    for (int i = 0; i < 2; i++) {
      if ((client_fd[i] = accept(server_fd, (struct sockaddr *)&client_addr[i], (socklen_t *)&client_addr[i])) < 0) {
        perror("accept failed");
        exit(1);
      }
    }

    printf("Connect client1 and client2\n");

    while ((val = read(client_fd[0], buffer, BUF_SIZE)) > 0) {
        printf("Message received from client1: %s\n", buffer);
        
        if (strcmp(buffer, "The End\n") == 0) {
            printf("The end\n");
            break;
        }

        if (send(client_fd[1], buffer, val, 0) < 0) {
            perror("send failed");
            exit(1);
        }

        printf("Message sent to client2\n");
        memset(buffer, 0, BUF_SIZE);
    }

    close(client_fd[0]);
    close(client_fd[1]);
    close(server_fd);
    return 0;
}
