#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/msg.h>
#include <unistd.h>

#define N 3 // number of smokers

struct msgbuf {
    long mtype;
    int mtext;
};

int has_tobacco = 0;
int has_paper = 0;
int has_match = 0;

// function for smoker with tobacco
void *smoker_tobacco(void *arg) {
    int msqid = *((int*)arg);
    struct msgbuf buf;

    while (1) {
        // wait for tobacco and paper
        msgrcv(msqid, &buf, sizeof(int), 1, 0);

        printf("Smoker with tobacco is making a cigarette\n");

        // signal the agent that the smoker is done
        buf.mtype = 4;
        buf.mtext = 0;
        msgsnd(msqid, &buf, sizeof(int), 0);

        // signal the match is available
        buf.mtype = 2;
        buf.mtext = 0;
        msgsnd(msqid, &buf, sizeof(int), 0);
    }
}

// function for smoker with paper
void *smoker_paper(void *arg) {
    int msqid = *((int*)arg);
    struct msgbuf buf;

    while (1) {
        // wait for paper and match
        msgrcv(msqid, &buf, sizeof(int), 2, 0);

        printf("Smoker with paper is making a cigarette\n");

        // signal the agent that the smoker is done
        buf.mtype = 4;
        buf.mtext = 0;
        msgsnd(msqid, &buf, sizeof(int), 0);

        // signal the tobacco is available
        buf.mtype = 1;
        buf.mtext = 0;
        msgsnd(msqid, &buf, sizeof(int), 0);
    }
}

// function for smoker with match
void *smoker_match(void *arg) {
    int msqid = *((int*)arg);
    struct msgbuf buf;

    while (1) {
        // wait for tobacco and match
        msgrcv(msqid, &buf, sizeof(int), 3, 0);

        printf("Smoker with match is making a cigarette\n");

        // signal the agent that the smoker is done
        buf.mtype = 4;
        buf.mtext = 0;
        msgsnd(msqid, &buf, sizeof(int), 0);

        // signal the paper is available
        buf.mtype = 2;
        buf.mtext = 0;
        msgsnd(msqid, &buf, sizeof(int), 0);
    }
}

// function for agent
void *agent(void *arg) {
    int msqid = *((int*)arg);
    struct msgbuf buf;

    while (1) {
        // generate random number to decide which ingredients to supply
        int r = rand() % 3;

        if (r == 0) {
            // supply tobacco and paper
            printf("Agent is supplying tobacco and paper\n");
            has_tobacco = 1;
            has_paper = 1;

            // signal the smoker with match
            buf.mtype = 3;
            buf.mtext = 0;
            msgsnd(msqid,&buf, sizeof(int), 0);
} else if (r == 1) {
// supply paper and match
printf("Agent is supplying paper and match\n");
has_paper = 1;
has_match = 1;
        // signal the smoker with tobacco
        buf.mtype = 1;
        buf.mtext = 0;
        msgsnd(msqid, &buf, sizeof(int), 0);
    } else {
        // supply tobacco and match
        printf("Agent is supplying tobacco and match\n");
        has_tobacco = 1;
        has_match = 1;

        // signal the smoker with paper
        buf.mtype = 2;
        buf.mtext = 0;
        msgsnd(msqid, &buf, sizeof(int), 0);
    }

    // wait for the smoker to finish
    msgrcv(msqid, &buf, sizeof(int), 4, 0);

    // reset the ingredients
    has_tobacco = 0;
    has_paper = 0;
    has_match = 0;
}
}

int main() {
pthread_t threads[N+1]; // 3 smoker threads + 1 agent thread
int msqid;
key_t key;
struct msgbuf buf;
// create message queue
key = ftok("progfile", 65);
msqid = msgget(key, 0666 | IPC_CREAT);
buf.mtype = 1;
buf.mtext = 0;
msgsnd(msqid, &buf, sizeof(int), 0);
buf.mtype = 2;
buf.mtext = 0;
msgsnd(msqid, &buf, sizeof(int), 0);
buf.mtype = 3;
buf.mtext = 0;
msgsnd(msqid, &buf, sizeof(int), 0);

// create threads
pthread_create(&threads[0], NULL, agent, (void*)&msqid);
pthread_create(&threads[1], NULL, smoker_tobacco, (void*)&msqid);
pthread_create(&threads[2], NULL, smoker_paper, (void*)&msqid);
pthread_create(&threads[3], NULL, smoker_match, (void*)&msqid);

// wait for threads to finish
for (int i = 0; i < N+1; i++) {
    pthread_join(threads[i], NULL);
}

// delete message queue
msgctl(msqid, IPC_RMID, NULL);

return 0;
}