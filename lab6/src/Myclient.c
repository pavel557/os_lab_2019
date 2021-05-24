#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>

#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)
#define SLEN sizeof(struct sockaddr_in)

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(34543);
    inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);
    connect(fd, (struct sockaddr*) &adr, sizeof adr);
    write(STDOUT_FILENO,"Input count UDP\n", 16);
    char buf[256];
    ssize_t nread;
    nread = read(STDIN_FILENO, buf, 256);
    write(fd, buf, 256);
    int UDPCount = atoi(buf);
    sleep(1);
    int fdUDP[UDPCount];
    uint16_t portt = 40000;
    for (int i=0; i<UDPCount; i++)
    {
        read(fd, buf, 256);
        fdUDP[i] = atoi(buf);
        printf("%d\n", fdUDP[i]);
        
    }
    for (int i=0; i<UDPCount; i++)
    {
      portt++;
      struct sockaddr_in servaddr;
      memset(&servaddr, 0, SLEN);
      servaddr.sin_family = AF_INET;
      servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
      servaddr.sin_port = htons(portt);
      
      bind(fdUDP[i], (SADDR *)&servaddr, SLEN);
      
      if (sendto(fdUDP[i], "sendline", 9, 0, (SADDR *)&servaddr, SLEN) == -1) {
        perror("sendto problem");
        exit(1);
      
    }
    }
    close(fd);
    return 0;
}