#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define FORK_ERROR -1
#define IS_A_CHILD 0

#define GLOBAL_VALUE 100
#define LOCAL_VALUE 200

#define NEW_GLOBAL_VALUE 999
#define NEW_LOCAL_VALUE 888

#define EXIT_CODE 5
#define SUCCESS 0

#define CHILD_SLEEP_TIME 30
#define PARENT_SLEEP_TIME 60


int global_var = GLOBAL_VALUE;

int main() {
    int local_var = LOCAL_VALUE;

    printf("=== Parent process (PID: %d) ===\n", getpid());
    printf("Address of global_var = %p, value = %d\n", (void*)&global_var, global_var);
    printf("Address of local_var  = %p, value = %d\n", (void*)&local_var, local_var);

    pid_t pid = fork();

    if (pid == FORK_ERROR) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == IS_A_CHILD) {
        printf("\n=== Child process (PID: %d, Parent PID: %d) ===\n", getpid(), getppid());

        printf("Initial values in child:\n");
        printf("  global_var = %d at %p\n", global_var, (void*)&global_var);
        printf("  local_var  = %d at %p\n", local_var, (void*)&local_var);

        global_var = NEW_GLOBAL_VALUE;
        local_var  = NEW_LOCAL_VALUE;
        printf("After modification in child:\n");
        printf("  global_var = %d at %p\n", global_var, (void*)&global_var);
        printf("  local_var  = %d at %p\n", local_var, (void*)&local_var);

        sleep(CHILD_SLEEP_TIME);
        printf("Child exits with code %d\n", EXIT_CODE);
        exit(EXIT_CODE);
    }
    else {
        printf("\n=== Parent process (PID: %d) after fork ===\n", getpid());
        printf("Parent values (unchanged):\n");
        printf("  global_var = %d at %p\n", global_var, (void*)&global_var);
        printf("  local_var  = %d at %p\n", local_var, (void*)&local_var);

        printf("Parent sleeping for %d seconds...", PARENT_SLEEP_TIME);
        sleep(PARENT_SLEEP_TIME);

        int status;
        pid_t terminated_pid = wait(&status);
        printf("\nParent: child with PID %d terminated\n", terminated_pid);

        if (WIFEXITED(status)) {
            printf("Child terminated normally with exit code: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child terminated by signal: %d (%s)\n", WTERMSIG(status), strsignal(WTERMSIG(status)));
        } else {
            printf("Child terminated abnormally (unknown reason)\n");
        }

    }

    return SUCCESS;
}
