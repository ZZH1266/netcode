#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char const *argv[])
{

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <IP> <Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    // 创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    // 0: 对于 SOCK_STREAM ，默认协议是 TCP
    if (sock < 0)
    {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    // 向服务器(特定的IP和端口)发起请求
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_ip);
    serv_addr.sin_port = htons(server_port);

    // 在客户端程序中将套接字连接到指定的服务器地址,建立与服务器的连接
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect() failed");
        close(sock);
        exit(EXIT_FAILURE);
    }
    // sockfd:  套接字文件描述符，由 socket 函数创建
    // addr:    指向 sockaddr 结构体的指针，该结构体包含要连接的服务器的地址信息
    // addrlen: sockaddr 结构体的长度

    // 读取服务器传回的数据
    char buffer[40];
    // 从套接字中读取数据
    ssize_t numBytes = read(sock, buffer, sizeof(buffer) - 1);
    // fd:    文件描述符。可以是套接字、文件或其他可读设备的描述符
    // buf:   指向一个缓冲区的指针，用于存储读取的数据
    // count: 要读取的字节数;读取的最大字节数为 sizeof(buffer) - 1，以确保缓冲区留有空间存储空字符(结束符)
    if (numBytes < 0)
    {
        perror("read() failed");
        close(sock);
        exit(EXIT_FAILURE);
    }
    buffer[numBytes] = '\0'; // 确保字符串以 null 结尾

    printf("Message form server: %s\n", buffer);

    // 关闭套接字
    close(sock);

    return 0;
}
