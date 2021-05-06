#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(34543);
    inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);
    connect(fd, (struct sockaddr*) &adr, sizeof adr);
    write(fd,"Hello\n", 6);
    char buf[256];
    ssize_t nread;
    nread = read(fd, buf, 256);
    write(STDOUT_FILENO, buf, nread);
    close(fd);
    return 0;
}