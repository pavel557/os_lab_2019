#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/time.h>
#include <pthread.h>

int res = 1;
int k = -1;
int pnum = -1;
int mod = -1;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void factorial(int* number)
{
    int n = k/pnum;
    int i = n * (*number) + 1;
    int end;
    if ((*(number) + 1)==pnum)
    {
        end = k + 1;
    }
    else {
        end = n * (*number + 1) + 1;
    }

    for(i = i; i < end; i++)
    {   
        pthread_mutex_lock(&mut);
        res = (res * (i % mod)) % mod;
        pthread_mutex_unlock(&mut);
    }
    printf("number= %d res= %d\n",*number,res);
}

int main(int argc, char **argv) {
  
    while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);
    
    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            k = atoi(optarg);
            if (k <= 0) 
            {
                printf("k is a positive number\n");
                return 1;
            }
            break;
          case 1:
            pnum = atoi(optarg);
            if (pnum <= 0) {
                printf("pnum is a positive number\n");
                return 1;
            }
            break;
          case 2:
            mod = atoi(optarg);
            if (mod <= 0) {
                printf("mod is a positive number\n");
                return 1;
            }
            break;
          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (k == -1 || pnum == -1 || mod == -1) {
    printf("Usage: %s --k \"num\" --pnum \"num\" --mod \"num\" \n", argv[0]);
    return 1;
  }

   pthread_t Threads_mass[pnum];
    int num[pnum];
    for(int i = 0; i < pnum; i++)
    {    
        *(num+i)=i;
        if (pthread_create(Threads_mass + i, NULL, (void *)factorial,
              (void *)(num+i)) != 0) {
        perror("pthread_create");
        exit(1);
        }
    }
    
    for(int i = 0; i < pnum; i++){
        if (pthread_join(*(Threads_mass + i), NULL) != 0) {
            perror("pthread_join");
            exit(1);
        }
    }
    printf("\n%d!mod(%d) = %d\n", k, mod, res);

    return 0;
}