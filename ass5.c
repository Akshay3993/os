#include <stdio.h>
#include <stdbool.h>

#define MAX 10 // Max processes/resources

int P, R;
int Allocation[MAX][MAX], Max[MAX][MAX], Need[MAX][MAX];
int Available[MAX], Work[MAX];
bool Finish[MAX];
int safeSeq[MAX];

// Recursive function to find all safe sequences
void findSafeSequences(int index) {
    for (int i = 0; i < P; i++) {
        if (!Finish[i]) {
            bool canRun = true;

            for (int j = 0; j < R; j++) {
                if (Need[i][j] > Work[j]) {
                    canRun = false;
                    break;
                }
            }

            if (canRun) {
                // Assume process i runs
                for (int j = 0; j < R; j++)
                    Work[j] += Allocation[i][j];

                Finish[i] = true;
                safeSeq[index] = i;

                findSafeSequences(index + 1);

                // Backtrack
                Finish[i] = false;
                for (int j = 0; j < R; j++)
                    Work[j] -= Allocation[i][j];
            }
        }
    }

    // If all processes finished, print the sequence
    if (index == P) {
        printf("Safe sequence: ");
        for (int i = 0; i < P; i++)
            printf("P%d ", safeSeq[i]);
        printf("\n");
    }
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &P);

    printf("Enter number of resource types: ");
    scanf("%d", &R);

    printf("\nEnter Allocation matrix (%d x %d):\n", P, R);
    for (int i = 0; i < P; i++) {
        printf("Process P%d: ", i);
        for (int j = 0; j < R; j++)
            scanf("%d", &Allocation[i][j]);
    }

    printf("\nEnter Max matrix (%d x %d):\n", P, R);
    for (int i = 0; i < P; i++) {
        printf("Process P%d: ", i);
        for (int j = 0; j < R; j++)
            scanf("%d", &Max[i][j]);
    }

    printf("\nEnter Available resources (%d values): ", R);
    for (int i = 0; i < R; i++) {
        scanf("%d", &Available[i]);
        Work[i] = Available[i]; // initialize Work
    }

    // Calculate Need matrix
    for (int i = 0; i < P; i++) {
        Finish[i] = false;
        for (int j = 0; j < R; j++) {
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }

    printf("\nAll possible safe sequences:\n");
    findSafeSequences(0);

    return 0;
}
