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


#define SADDR struct sockaddr

int main(int argc, char *argv[]) {
  const size_t kSize = sizeof(struct sockaddr_in);
  int SERV_PORT=-1;
  int BUFSIZE=-1;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"SERV_PORT", required_argument, 0, 0},
                                      {"BUFSIZE", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        SERV_PORT = atoi(optarg);
        if (SERV_PORT <= 0) 
            {
                printf("SERV_PORT is a positive number\n");
                return 1;
            }
        break;
        case 1:
        BUFSIZE = atoi(optarg);
        if (BUFSIZE <= 0) 
            {
                printf("BUFSIZE is a positive number\n");
                return 1;
            }
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Unknown argument\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (BUFSIZE == -1||SERV_PORT == -1) {
    fprintf(stderr, "BUFSIZE = %d\n", BUFSIZE);
    return 1;
  }

  int lfd, cfd;
  int nread;
  char* buf = calloc(BUFSIZE, sizeof(char));
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&servaddr, 0, kSize);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  if (bind(lfd, (SADDR *)&servaddr, kSize) < 0) {
    perror("bind");
    exit(1);
  }

  if (listen(lfd, 5) < 0) {
    perror("listen");
    exit(1);
  }

  
    int UDPCount = -1;
    unsigned int clilen = kSize;

    if ((cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
      perror("accept");
      exit(1);
    }
    printf("connection established\n");

    nread = read(cfd, buf, BUFSIZE);


    if (nread == -1) {
      perror("read");
      exit(1);
    }
    UDPCount = atoi(buf);
    printf("UDPCount= %d\n", UDPCount);
    if (UDPCount > 4)
    {
        exit(1);
    }

int active_child_processes = 0;

pid_t currentPid[UDPCount];

  int fd1[2], fd2[2];
  if(pipe(fd1) == -1)
  {
      return -1;
  }
  if(pipe(fd2) == -1)
  {
      return -1;
  }

  for (int i = 0; i < UDPCount; i++) {
    pid_t child_pid = -1;
    child_pid = fork();
    if (child_pid >= 0) {
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
        char* buff = calloc(10, sizeof(char));
        sprintf(buff, "%d", sockfd);
        printf("%s\n",buff);
        write(cfd, buff, sizeof(int));

          currentPid[i] = child_pid;
      }
    } else {
        printf("Fork failed!\n");
      return 1;
    }

  }

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
   close(cfd);
  
}
