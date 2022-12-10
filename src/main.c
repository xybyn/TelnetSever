#include <stdio.h>
#include <unistd.h>
#include "commands/dump/dump.h"
#include "commands/call/call.h"
#include "commands/resolve/resolve.h"
#include "commands/resolve_by_address/resolve_by_address.h"
#include "commands/write_by_address/write_by_address.h"
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <pthread.h>

typedef int (*command)(struct printer out, const char *command);
#define COMMANDS_COUNT 5
command commands[COMMANDS_COUNT];

void send_socket(const char *text, void *data)
{
    int client = (int)data;
    send(client, text, strlen(text) + 1, 0);
}

void parse_command(int socket, const char *c)
{
    struct printer p;
    p.args = socket;
    p.out = send_socket;
    for (int i = 0; i < COMMANDS_COUNT; ++i)
    {
        if (commands[i](p, c) == OK)
            return;
    }
}

void init_socket(struct sockaddr_in *address_sock, int port)
{
    address_sock->sin_family = AF_INET;
    address_sock->sin_port = htons(port);
    address_sock->sin_addr.s_addr = htonl(INADDR_ANY);
}

void *handle_user(void *socket)
{
    while (1)
    {
        if (send((int)socket, "> ", 3, 0) < 0)
        {
            printf("CLIENT DISCONNECTED\n");
            break;
        }
        char data[255];
        for (int i = 0; i < 255; ++i)
        {
            data[i] = '\0';
        }
        int received_bytes_len = recv((int)socket, data, sizeof(char) * 256, 0);
        if (received_bytes_len == 0)
        {
            printf("CLIENT DISCONNECTED\n");
            break;
        }
        data[strlen(data) - 2] = '\0';

        parse_command((int)socket, data);
    }
    return 0;
}

void *handle_users(void *server_socket)
{
    while (1)
    {
        struct sockaddr_in client;
        socklen_t size = sizeof(client);
        printf("Waiting client\n");
        int client_socket = accept(server_socket, (struct sockaddr *)&client, &size);
        if (client_socket < 0)
            continue;

        pthread_t tid;     
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, handle_user, client_socket);

        printf("Client accepted\n");
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int fd = STDOUT_FILENO;

    char input_buffer[256];
    commands[0] = dump_command;
    commands[1] = resolve_command;
    commands[2] = call_command;
    commands[3] = resolve_by_address_command;
    commands[4] = write_by_address_command;

    int server_socket = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address_sock;
    init_socket(&address_sock, atoi(argv[1]));
    int r = bind(server_socket, (struct sockaddr *)&address_sock, sizeof(struct sockaddr_in));

    listen(server_socket, 0);

    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, handle_users, server_socket);
    pthread_join(tid, NULL);
    printf("Server closed\n");
    return 0;
}
