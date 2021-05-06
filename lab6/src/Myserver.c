#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    int server = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(34543);
    bind(server, (struct sockaddr *)&adr, sizeof adr);
    listen(server, 5);
    socklen_t adrlen = sizeof adr;
    int fd = accept(server, (struct sockaddr *)&adr, &adrlen);
    ssize_t nread;
    char buf[256];
    nread = read(fd, buf, 256);
    if (nread == 0)
    {
        printf("End of file\n");
    }
    write(STDOUT_FILENO, buf, nread);
    write(fd, buf, nread);

    sleep(1);

    close(fd);
    close(server);
    return 0;
}