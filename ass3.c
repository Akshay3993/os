#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main() {
    int fd[2];
    pid_t pid;

    // Open file for reading
    int file = open("file1", O_RDONLY);
    if (file < 0) {
        perror("file open");
        exit(1);
    }

    // Redirect standard input to file
    dup2(file, STDIN_FILENO);
    close(file);

    // Create pipe
    pipe(fd);

    pid = fork();

    if (pid == 0) {
        // Child process: sort
        dup2(fd[1], STDOUT_FILENO); // write to pipe
        close(fd[0]); // not needed
        close(fd[1]);
        execl("/usr/bin/sort", "sort", NULL);
    }
    else if (pid > 0) {
        // Parent process: uniq
        dup2(fd[0], STDIN_FILENO); // read from pipe
        close(fd[1]); // not needed
        close(fd[0]);
        execl("/usr/bin/uniq", "uniq", NULL);
    }
    else {
        perror("fork failed");
        exit(1);
    }

    return 0;
}
