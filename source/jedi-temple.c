#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <fcntl.h>

#define YOUNGLINGS_COUNT 15


pid_t younglings_pids[YOUNGLINGS_COUNT];


void set_process_limit(int limit) {
    struct rlimit rl;
    rl.rlim_cur = limit;
    rl.rlim_max = limit;
    if (setrlimit(RLIMIT_NPROC, &rl) == -1) {
        perror("setrlimit failed");
    }
}


void sigchld_handler(int sig) {
    int saved_errno = errno;
    pid_t pid;

    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        for (int i = 0; i < YOUNGLINGS_COUNT; i++) {
            if (younglings_pids[i] == pid) {
                younglings_pids[i] = -1;
                break;
            }
        }
    }
    errno = saved_errno;
}

int spawn_youngling() {
    pid_t pid = fork();
    if (pid == 0) {
        while (1) sleep(3600); // Lives until killed or server resets
    } else if (pid > 0) {
        printf("Youngling spawned.\n");
        return pid;
    } else {
        return -1;
    }
}

void print_flag() {
    char flag[128];
    int fd = open("flag.txt", O_RDONLY);
    if (fd == -1) {
        return;
    }
    
    ssize_t n = read(fd, flag, sizeof(flag) - 1);
    if (n > 0) {
        flag[n] = '\0';
        printf("\nSuccess Anakin. The Jedi Order has fallen.\nFLAG: %s\n", flag);
    }
    close(fd);
}


int main() {

    // Reduce infra costs
    set_process_limit(500);

    setvbuf(stdout, NULL, _IONBF, 0);
    signal(SIGCHLD, sigchld_handler);
    
    printf("https://www.youtube.com/watch?v=L83d6Z9Voic\n");
    printf("Commands: SPAWN, KILL\n");

    for (int i = 0; i < YOUNGLINGS_COUNT; i++) {
        younglings_pids[i] = spawn_youngling(i);
    }

    char cmd[64];
    while (1) {
        int living = 0;

        // Count living younglings
        for (int i = 0; i < YOUNGLINGS_COUNT; i++) {
            if (younglings_pids[i] != -1 && kill(younglings_pids[i], 0) == 0) {
                living++;
            }
        }

        if (living == 0) {
            print_flag();
            sleep(2);
            continue;
        }

        printf("\n[%d Younglings Remaining] Commands: SPAWN, KILL\n> ", living);
        
        if (scanf("%63s", cmd) <= 0) break;

        if (strcmp(cmd, "SPAWN") == 0) {
            spawn_youngling();
        } else if (strcmp(cmd, "KILL") == 0) {
            int idx;
            if (scanf("%d", &idx) > 0 && idx >= 0 && idx < YOUNGLINGS_COUNT) {
                if (younglings_pids[idx] != -1) {
                    kill(younglings_pids[idx], SIGKILL);
                    printf("%d terminated.\n", idx);
                } else {
                    printf("%d is already down.\n", idx);
                }
            } else {
                printf("Invalid Index.\n");
            }
        }

        // Respawn dead younglings
        for (int i = 0; i < YOUNGLINGS_COUNT; i++) {
            if (younglings_pids[i] == -1) {
                younglings_pids[i] = spawn_youngling();
            }
        }

    }
    return 0;
}