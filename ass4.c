#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define READER_COUNT 5
#define WRITER_COUNT 2
#define READ_TIMES 3
#define WRITE_TIMES 2

int account_balance = 2000;
int active_readers = 0;

pthread_mutex_t lock_access;
sem_t writer_control;

// ------------------------- SYNC READER -------------------------
void* sync_reader(void* arg) {
    int id = *(int*)arg;
    free(arg);

    for (int i = 0; i < READ_TIMES; i++) {
        pthread_mutex_lock(&lock_access);
        active_readers++;
        if (active_readers == 1)
            sem_wait(&writer_control); // Block writer
        pthread_mutex_unlock(&lock_access);

        printf("[SYNC READER-%d] Current Balance: $%d\n", id, account_balance);
        usleep(400000); // simulate read delay

        pthread_mutex_lock(&lock_access);
        active_readers--;
        if (active_readers == 0)
            sem_post(&writer_control); // Allow writer
        pthread_mutex_unlock(&lock_access);

        usleep(500000);
    }
    return NULL;
}

// ------------------------- SYNC WRITER -------------------------
void* sync_writer(void* arg) {
    int id = *(int*)arg;
    free(arg);

    for (int i = 0; i < WRITE_TIMES; i++) {
        sem_wait(&writer_control); // Wait if readers active

        int amount = (id * 25) + 50;
        account_balance += amount;
        printf("[SYNC WRITER-%d] Updated Balance: $%d (+$%d)\n", id, account_balance, amount);

        usleep(900000); // simulate write delay
        sem_post(&writer_control); // Allow readers/writers
        usleep(500000);
    }
    return NULL;
}

// ---------------------- UNSYNC READER --------------------------
void* unsync_reader(void* arg) {
    int id = *(int*)arg;
    free(arg);

    for (int i = 0; i < READ_TIMES; i++) {
        printf("[UNSYNC READER-%d] Balance: $%d\n", id, account_balance);
        usleep(400000);
    }
    return NULL;
}

// ---------------------- UNSYNC WRITER --------------------------
void* unsync_writer(void* arg) {
    int id = *(int*)arg;
    free(arg);

    for (int i = 0; i < WRITE_TIMES; i++) {
        int amount = (id * 35) + 40;
        account_balance += amount;
        printf("[UNSYNC WRITER-%d] Balance Changed: $%d (+$%d)\n", id, account_balance, amount);
        usleep(800000);
    }
    return NULL;
}

// ---------------------- RUN SYNC VERSION -----------------------
void run_sync() {
    pthread_t readers[READER_COUNT], writers[WRITER_COUNT];

    pthread_mutex_init(&lock_access, NULL);
    sem_init(&writer_control, 0, 1);

    for (int i = 0; i < READER_COUNT; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&readers[i], NULL, sync_reader, id);
    }

    for (int i = 0; i < WRITER_COUNT; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&writers[i], NULL, sync_writer, id);
    }

    for (int i = 0; i < READER_COUNT; i++)
        pthread_join(readers[i], NULL);
    for (int i = 0; i < WRITER_COUNT; i++)
        pthread_join(writers[i], NULL);

    pthread_mutex_destroy(&lock_access);
    sem_destroy(&writer_control);
}

// --------------------- RUN UNSYNC VERSION ----------------------
void run_unsync() {
    pthread_t readers[READER_COUNT], writers[WRITER_COUNT];

    for (int i = 0; i < READER_COUNT; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&readers[i], NULL, unsync_reader, id);
    }

    for (int i = 0; i < WRITER_COUNT; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&writers[i], NULL, unsync_writer, id);
    }

    for (int i = 0; i < READER_COUNT; i++)
        pthread_join(readers[i], NULL);
    for (int i = 0; i < WRITER_COUNT; i++)
        pthread_join(writers[i], NULL);
}

// ------------------------------ MAIN ----------------------------
int main() {
    int choice;
    printf("Choose Mode:\n1. Synchronized\n2. Unsynchronized\nEnter choice (1/2): ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("\n--- Running Synchronized Execution ---\n");
        run_sync();
    } else if (choice == 2) {
        printf("\n--- Running Unsynchronized Execution ---\n");
        run_unsync();
    } else {
        printf("Invalid choice!\n");
    }

    printf("\nExecution Completed!\n");
    return 0;
}
