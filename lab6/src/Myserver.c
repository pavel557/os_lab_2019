#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

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
    int UDPCount = atoi(buf);
    int active_child_processes = 0;
    pid_t currentPid[UDPCount];
    for (int i = 0; i < UDPCount; i++) 
    {
        pid_t child_pid = -1;
        child_pid = fork();
        active_child_processes += 1;
        if (child_pid == 0) {
         return 0;
      }
      if (child_pid > 0)
      {
        int sockfd;
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket problem");
        exit(1);
        }
        int b = sockfd;
        int a = 1;
        while (b>=10)
        {
            b = b/10;
            a++;
        }
        char* buff = calloc(a+1, sizeof(char));
        sprintf(buff, "%d", sockfd);
        printf("%s\n",buff);
        write(fd, buff, sizeof(buff));

          currentPid[i] = child_pid;
      }
        else {
        printf("fork error\n");
        exit(1);
    }
    }
    printf("active=%d", active_child_processes);
    int status;
    while (active_child_processes > 0) {
        printf("\nThe child processes is killing...\n");
        for (int i =0; i<UDPCount; i++)
            kill(currentPid[i], SIGKILL);
        return 1;
    while (waitpid(-1, &status, WNOHANG) > 0) 
    {
        active_child_processes -= 1;
    }
  }
    close(fd);
    close(server);
    return 0;
}