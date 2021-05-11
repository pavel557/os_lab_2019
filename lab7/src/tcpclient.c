#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>

#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
  int fd;
  int nread;
  int BUFSIZE = -1;

  int tnum = -1;
  int port = -1;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"BUFSIZE", required_argument, 0, 0},
                                      {"port", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        BUFSIZE = atoi(optarg);
        if (BUFSIZE <= 0) 
            {
                printf("BUFSIZE is a positive number\n");
                return 1;
            }
        break;
        case 1:
        port = atoi(optarg);
        if (port <= 0) 
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

  if (BUFSIZE == -1) {
    fprintf(stderr, "BUFSIZE = %d\n", BUFSIZE);
    return 1;
  }  
  
  char* buf = calloc(BUFSIZE, sizeof(char));
  struct sockaddr_in servaddr;

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creating");
    exit(1);
  }

  memset(&servaddr, 0, SIZE);
  servaddr.sin_family = AF_INET;

  if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
    perror("bad address");
    exit(1);
  }

  servaddr.sin_port = htons(port);

  if (connect(fd, (SADDR *)&servaddr, SIZE) < 0) {
    perror("connect");
    exit(1);
  }

  write(1, "Input count UDP\n", 22);
  nread = read(0, buf, BUFSIZE);
  write(fd, buf, nread);
  int UDPCount = atoi(buf);
  int sockfd[UDPCount];
  int i=0;
      while ((nread = read(fd, buf, BUFSIZE)) > 0)
      {
            sockfd[i] = atoi(buf);
            i++;
      }
      int portt = 30000;
  for (int i=0; i<UDPCount; i++)
  {
      printf("%d\n", sockfd[i]);
      portt++;
      struct sockaddr_in servaddr;
      memset(&servaddr, 0, SLEN);
      servaddr.sin_family = AF_INET;
      servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
      servaddr.sin_port = htons(portt);
      if (bind(sockfd[i], (SADDR *)&servaddr, SLEN) < 0) {
        perror("bind problem");
        exit(1);
  }
      
      if (sendto(sockfd[i], "sendline", 9, 0, (SADDR *)&servaddr, SLEN) == -1) {
        perror("sendto problem");
        exit(1);
      
    }
  }
  close(fd);
  exit(0);
}
