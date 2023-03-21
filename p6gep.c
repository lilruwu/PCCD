#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/msg.h>
#include <unistd.h>

#define N 5 // number of philosophers

struct msgbuf {
    long mtype;
    int mtext;
};

int chopsticks[N]; // array to keep track of the availability of chopsticks

// function for philosopher
void *philosopher(void *arg) {
    int id = *((int*)arg);
    int left_chopstick = id;
    int right_chopstick = (id + 1) % N;
    struct msgbuf buf;

    while (1) {
        // pick up left chopstick
        while (chopsticks[left_chopstick] == 1) {
            usleep(rand() % 1000); // wait for a random amount of time
        }
        chopsticks[left_chopstick] = 1;

        // pick up right chopstick
        while (chopsticks[right_chopstick] == 1) {
            usleep(rand() % 1000); // wait for a random amount of time
        }
        chopsticks[right_chopstick] = 1;

        printf("Philosopher %d is eating\n", id);

        // put down left chopstick
        chopsticks[left_chopstick] = 0;

        // put down right chopstick
        chopsticks[right_chopstick] = 0;

        printf("Philosopher %d is thinking\n", id);
    }
}

// function for creating message queues
int create_message_queue(key_t key) {
    int msqid = msgget(key, IPC_CREAT | 0666);
    if (msqid == -1) {
        perror("msgget");
        exit(1);
    }
    return msqid;
}

int main() {
    int i, id[N], msqid[N];
    pthread_t tid[N];

    // initialize chopsticks array to all zeros
    for (i = 0; i < N; i++) {
        chopsticks[i] = 0;
    }

    // create message queues
    for (i = 0; i < N; i++) {
        key_t key = ftok(".", i);
        msqid[i] = create_message_queue(key);
    }

    // create threads for philosophers
    for (i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&tid[i], NULL, philosopher, &id[i]);
    }

    // wait for threads to terminate
    for (i = 0; i < N; i++) {
        pthread_join(tid[i], NULL);
    }

    // delete message queues
    for (i = 0; i < N; i++) {
        if (msgctl(msqid[i], IPC_RMID, NULL) == -1) {
            perror("msgctl");
            exit(1);
        }
    }

    return 0;
}
