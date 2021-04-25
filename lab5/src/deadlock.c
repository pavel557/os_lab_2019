#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t violin = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bow = PTHREAD_MUTEX_INITIALIZER;

void TheFirstViolinistPlays()
{
    pthread_mutex_lock(&violin);
    sleep(1);
    pthread_mutex_lock(&bow);
    printf("The First Violinist Plays");
    pthread_mutex_unlock(&bow);
    pthread_mutex_unlock(&violin);
}

void TheSecondViolinistPlays()
{
    pthread_mutex_lock(&violin);
    sleep(1);
    pthread_mutex_lock(&bow);
    printf("The Second Violinist Plays");
    pthread_mutex_unlock(&violin);
    pthread_mutex_unlock(&bow);
}

int main() 
{
    pthread_t FirstViolinist, SecondViolinist;
    if (pthread_create(&FirstViolinist, NULL, (void *)TheFirstViolinistPlays, NULL) != 0) 
    {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_create(&SecondViolinist, NULL, (void *)TheSecondViolinistPlays, NULL) != 0) 
    {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_join(FirstViolinist, NULL) != 0) 
    {
        perror("pthread_join");
        exit(1);
    }

  if (pthread_join(SecondViolinist, NULL) != 0) 
    {
        perror("pthread_join");
        exit(1);
    }
}