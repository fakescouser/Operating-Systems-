//Kiourt Mechmetali Achmet 1084672
//Lalas Georgios 1093406
//Gkourvelos Leonidas 1072506
//Georgiou Konstantinos 1067419
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#define MAX_PROCESSORS 4

typedef struct proc_desc {
    struct proc_desc *next;
    char name[80];
    int pid;
    int status;
    int processors_needed;
    double t_submission, t_start, t_end;
} proc_t;

struct single_queue {
    proc_t *first;
    proc_t *last;
    long members;
};

struct single_queue global_q;

#define PROC_NEW        0
#define PROC_RUNNING    2
#define PROC_EXITED     3

int processors[MAX_PROCESSORS] = {0};  // Array to track processor usage

// Queue initialization
void proc_queue_init(struct single_queue *q) {
    q->first = q->last = NULL;
    q->members = 0;
}

// Add process to the end of the queue
void proc_to_rq_end(proc_t *proc) {
    if (global_q.last) {
        global_q.last->next = proc;
    } else {
        global_q.first = proc;
    }
    global_q.last = proc;
    proc->next = NULL;
}

// Dequeue process from the queue
proc_t *proc_rq_dequeue() {
    proc_t *proc = global_q.first;
    if (proc) {
        global_q.first = proc->next;
        if (!global_q.first) {
            global_q.last = NULL;
        }
    }
    return proc;
}

// Get the current time
double proc_gettime() {
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// Count the number of free processors
int count_free_processors() {
    int count = 0;
    for (int i = 0; i < MAX_PROCESSORS; i++) {
        if (processors[i] == 0) {
            count++;
        }
    }
    return count;
}

// Allocate processors to a process
void allocate_processors(int needed) {
    int allocated = 0;
    for (int i = 0; i < MAX_PROCESSORS && allocated < needed; i++) {
        if (processors[i] == 0) {
            processors[i] = 1;  // Mark processor as busy
            allocated++;
        }
    }
}

// Free processors used by a process
void free_processors(int needed) {
    int freed = 0;
    for (int i = 0; i < MAX_PROCESSORS && freed < needed; i++) {
        if (processors[i] == 1) {
            processors[i] = 0;  // Mark processor as free
            freed++;
        }
    }
}

// Read processes and their processor requirements
void parse_input() {
    // Simulate input for testing purposes
    // Each process will have a predefined number of processors
    proc_t *proc1 = malloc(sizeof(proc_t));
    strcpy(proc1->name, "process1");
    proc1->processors_needed = 2;
    proc1->status = PROC_NEW;
    proc1->t_submission = proc_gettime();
    proc_to_rq_end(proc1);

    proc_t *proc2 = malloc(sizeof(proc_t));
    strcpy(proc2->name, "process2");
    proc2->processors_needed = 1;
    proc2->status = PROC_NEW;
    proc2->t_submission = proc_gettime();
    proc_to_rq_end(proc2);
    
     proc_t *proc3 = malloc(sizeof(proc_t));
    strcpy(proc3->name, "process3");
    proc3->processors_needed = 4;
    proc3->status = PROC_NEW;
    proc3->t_submission = proc_gettime();
    proc_to_rq_end(proc3);
    
     proc_t *proc4 = malloc(sizeof(proc_t));
    strcpy(proc4->name, "process4");
    proc4->processors_needed = 3;
    proc4->status = PROC_NEW;
    proc4->t_submission = proc_gettime();
    proc_to_rq_end(proc4);
}

// FCFS Scheduler
void fcfs() {
    proc_t *proc;
    int pid;
    int status;

    while ((proc = proc_rq_dequeue()) != NULL) {
        // Wait until enough processors are available
        while (count_free_processors() < proc->processors_needed) {
            printf("Insufficient processors for %s (needs %d). Waiting...\n",
                   proc->name, proc->processors_needed);
            sleep(1);
        }

        // Allocate processors for the process
        allocate_processors(proc->processors_needed);
        proc->t_start = proc_gettime();

        pid = fork();
        if (pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            printf("Executing %s using %d processors\n", proc->name, proc->processors_needed);
            sleep(2); // Simulate process execution
            exit(0);
        } else {
            proc->pid = pid;
            proc->status = PROC_RUNNING;
            waitpid(proc->pid, &status, 0);
            free_processors(proc->processors_needed);
            proc->status = PROC_EXITED;
            proc->t_end = proc_gettime();

            // Print process details
            printf("PID %d - CMD: %s\n", pid, proc->name);
            printf("\tElapsed time = %.2lf secs\n", proc->t_end - proc->t_submission);
            printf("\tExecution time = %.2lf secs\n", proc->t_end - proc->t_start);
        }
    }
}

// Main function
int main() {
    proc_queue_init(&global_q);
    parse_input();
    fcfs();
    return 0;
}

