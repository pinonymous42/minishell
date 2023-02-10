#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

int main() {
    pid_t child_pid, wtr, pid_value;
    int wstatus;

    child_pid = fork();
	// printf(">child pid is %d<\n", child_pid);
	// printf("%s, %d\n", __FILE__, __LINE__);
    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
	// printf("|child pid is %d|\n", child_pid);
	// printf("%s, %d\n", __FILE__, __LINE__);
    if (child_pid == 0) {
		pid_value = getpid();
		while (1)
		{
        	printf("Child PID is %d\n", pid_value);
			sleep(1);
		}
        // pause();
        _exit(EXIT_FAILURE);
    } else {
		sleep(3);
		kill(child_pid, SIGKILL);
        wtr = waitpid(child_pid, &wstatus, WUNTRACED | WCONTINUED);
        if (wtr == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(wstatus)) {
            printf("exited, status=%d\n", WEXITSTATUS(wstatus));
        } else if (WIFSIGNALED(wstatus)) {
            printf("killed by signal %d\n", WTERMSIG(wstatus));
        }
    }
    exit(EXIT_SUCCESS);
}