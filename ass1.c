#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX 256

void simulate_cp() {
    char source[100], dest[100];
    FILE *src, *dst;
    char ch;

    printf("Enter source file: ");
    scanf("%s", source);
    printf("Enter destination file: ");
    scanf("%s", dest);

    src = fopen(source, "r");
    if (src == NULL) {
        perror("Error opening source file");
        exit(1);
    }

    dst = fopen(dest, "w");
    if (dst == NULL) {
        perror("Error opening destination file");
        fclose(src);
        exit(1);
    }

    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dst);
    }

    fclose(src);
    fclose(dst);
    printf("File copied successfully from %s to %s\n", source, dest);
}

void simulate_grep() {
    char filename[100], keyword[100];
    FILE *fp;
    char line[MAX];
    int found = 0;

    printf("Enter word to search: ");
    scanf("%s", keyword);
    printf("Enter filename: ");
    scanf("%s", filename);

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, keyword)) {
            printf("%s", line);
            found = 1;
        }
    }

    if (!found) {
        printf("'%s' not found in %s\n", keyword, filename);
    }

    fclose(fp);
}

int main() {
    int choice;
    pid_t pid;

    printf("Choose a command to simulate:\n");
    printf("1. cp (copy file)\n");
    printf("2. grep (search a word)\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0) {
        // Child process
        printf("Child (PID: %d) started.\n", getpid());

        if (choice == 1)
            simulate_cp();
        else if (choice == 2)
            simulate_grep();
        else
            printf("Invalid choice.\n");

        printf("Child (PID: %d) exiting.\n", getpid());
        exit(0);
    }
    else {
        // Parent process
        printf("Parent (PID: %d) waiting for child...\n", getpid());
        wait(NULL);
        printf("Parent: Child has completed.\n");
    }

    return 0;
}
