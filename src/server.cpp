#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

int main(int argc, char const *argv[])
{
    /****************************** 接受用户输入的IP地址和端口号 ******************************/

    if (argc != 3) // 检查传递给程序的命令行参数的数量
    {
        fprintf(stderr, "Usage: %s <IP> <Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 将命令行参数中的第一个参数（IP 地址）存储在一个字符串指针 server_ip 中
    const char *server_ip = argv[1];

    // 将命令行参数中的第二个参数（端口号）转换为整数，并存储在 server_port 变量中
    // atoi 函数用于将字符串转换为整数
    int server_port = atoi(argv[2]);

    /****************************** 网络编程 ******************************/

    // 创建套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // AF_INET:     表示可使用IPv4地址
    // SOCK_STREAM: 表示使用面向连接的数据传输方式
    // IPPROTO_TCP: 表示使用TCP协议
    if (serv_sock < 0)
    {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    // 创建socket IPv4地址
    struct sockaddr_in serv_addr;
    // sin_family: 地址族，通常为AF_INET(表示IPv4地址)
    // sin_port:   端口号
    // sin_addr:   IP地址

    // 将serv_addr结构体的内存全部设置为0，即初始化地址
    memset(&serv_addr, 0, sizeof(serv_addr));

    // 初始化IPv4地址和端口
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_ip);
    serv_addr.sin_port = htons(server_port);

    // 将服务器socket与初始化后的IPv4地址和端口的结构体绑定
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind() failed");
        close(serv_sock);
        exit(EXIT_FAILURE);
    }
    // sockfd:  这是要绑定的套接字文件描述符，通常由 socket 函数创建
    // addr:    这是一个指向 sockaddr 结构体的指针，包含了要绑定的IP地址和端口号
    // addrlen: 这是 sockaddr 结构体的大小

    // 进入监听状态，等待用户发起请求
    if (listen(serv_sock, 20) < 0)
    {
        perror("listen() failed");
        close(serv_sock);
        exit(EXIT_FAILURE);
    }
    // sockfd:  要监听的套接字文件描述符。该套接字必须已经通过 socket 函数创建，并通过 bind 函数绑定到一个特定的地址和端口
    // backlog: 指定连接队列的最大长度，即在 accept 函数处理连接请求之前，可以有多少个连接处于等待状态。如果等待的连接数超过了此值，新的连接请求将被拒绝

    // 接收客户端请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    // socklen_t 是一个适合存储套接字地址大小的类型
    int clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    // serv_sock:                     服务器监听的套接字描述符
    // (struct sockaddr *)&clnt_addr: 指向 sockaddr 结构体的指针，用于存储客户端的地址信息。这里将 clnt_addr 强制转换为 struct sockaddr 类型的指针
    // &clnt_addr_size:               指向 socklen_t 类型变量的指针，初始时包含 clnt_addr 结构体的大小。accept 函数会更新这个值，以反映实际的客户端地址大小
    if (clnt_sock < 0)
    {
        perror("accept() failed");
        close(serv_sock);
        exit(EXIT_FAILURE);
    }

    // 向客户端发送数据
    char str[] = "Hello World!";
    if (write(clnt_sock, str, sizeof(str)) < 0)
    {
        perror("write() failed");
    }
    // fd:    文件描述符，指定要写入的文件或套接字
    // buf:   指向要写入的数据缓冲区
    // count: 要写入的数据字节数

    // 关闭套接字
    close(clnt_sock);
    close(serv_sock);

    return 0;
}