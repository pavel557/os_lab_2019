#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

bool kill_flag = false;

void handle_alarm(int sig)
{
    kill_flag = true;
}

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  int timeout = -1;
  bool with_files = false;

  

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {"timeout", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);
    
    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if (seed <= 0) 
            {
                printf("seed is a positive number\n");
                return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size <= 0) {
                printf("array_size is a positive number\n");
                return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            if (pnum <= 0) {
                printf("pnum is a positive number\n");
                return 1;
            }
            break;
          case 3:
            with_files = true;
            break;
          case 4:
            timeout = atoi(optarg);
			if (timeout <= 0) {
				printf("timeout is a positive number\n");
				return 1;
			}
			break;  
          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
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

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }


    FILE *fp;
    fp=fopen("min.txt", "w");
    fclose(fp);
    fp=fopen("max.txt", "w");
    fclose(fp);

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);



  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  if (timeout != -1){
        signal( SIGALRM, handle_alarm );
        alarm( timeout );
    }

    pid_t currentPid[pnum];

  int fd1[2], fd2[2];
  if(pipe(fd1) == -1)
  {
      return -1;
  }
  if(pipe(fd2) == -1)
  {
      return -1;
  }

    
    printf("%d\n", seed);
    printf("%d\n", array_size);
    printf("%d\n", pnum);

  for (int i = 0; i < pnum; i++) {
    pid_t w, child_pid = -1;
    child_pid = fork();
    if (child_pid >= 0) {
      active_child_processes += 1;
      if (child_pid == 0) {
        int begin, end;
        begin = i * array_size/pnum;
        if (i < pnum - 1)
        {
            end = begin + array_size/pnum;
        }
        else {
            end = array_size;
        }
        struct MinMax min_max = GetMinMax(array, begin, end);
        
        

        if (with_files) {
            char buff[20];
            FILE *fp;
            fp=fopen("min.txt", "a");
            fprintf(fp, "%d\n", min_max.min);
            fclose(fp);
            fp=fopen("max.txt", "a");
            fprintf(fp, "%d\n", min_max.max);
            fclose(fp);
        } else {
            close(fd1[0]);
            close(fd2[0]);
            write(fd1[1], &(min_max.min), sizeof(min_max.min));
            write(fd2[1], &(min_max.max), sizeof(min_max.max));
            close(fd1[1]);
            close(fd2[1]);
        }
        
        return 0;
      }
      if (child_pid > 0)
      {
          currentPid[i] = child_pid;
      }
    } else {
        printf("Fork failed!\n");
      return 1;
    }

  }

int status;

  while (active_child_processes > 0) {
    if (kill_flag == true){
        printf("\nThe child processes is killing...\n");
        for (int i =0; i<pnum; i++)
            kill(currentPid[i], SIGKILL);
        return 1;
    }
    while (waitpid(-1, &status, WNOHANG) > 0) 
    {
        
        active_child_processes -= 1;
    }
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  
    FILE *fp1, *fp2;
fp1=fopen("min.txt", "r");
fp2=fopen("max.txt", "r");
  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
        fscanf(fp1, "%d", &min);
        fgetc(fp1);
        fscanf(fp2, "%d", &max);
        fgetc(fp2);
    } else {
        close(fd1[1]);
        close(fd2[1]);
        read(fd1[0], &(min), sizeof(min));
        read(fd2[0], &(max), sizeof(max));
        
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

    fclose(fp1);
    fclose(fp2);
    close(fd1[0]);
    close(fd2[0]);

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
