#include <stdio.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    pid_t child_pid = fork();
    if (child_pid == 0) {
        
        execl("./sequential_min_max","sequential_min_max" ,"2", "9", NULL);
    }
    return 0;
}
