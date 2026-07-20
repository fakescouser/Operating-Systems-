//Kiourt Mechmetali Achmet 1084672
//Lalas Georgios 1093406
//Gkourvelos Leonidas 1072506
//Georgiou Konstantinos 1067419


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MEMORY_SIZE 512
#define TIME_QUANTUM 3

typedef struct {
    int pid;
    int arrival_time;
    int duration;
    int remaining_time;
    int memory_needed;
    bool in_memory;
} Process;

typedef struct {
    int start;
    int size;
    bool free;
    int pid;
} MemoryBlock;

MemoryBlock memory[MEMORY_SIZE]; // Μοντέλο Memory
int memory_block_count = 1;

// Initialize memory
void init_memory() {
    memory[0].start = 0;
    memory[0].size = MEMORY_SIZE;
    memory[0].free = true;
    memory[0].pid = -1;
}

// Βρείτε ένα μπλοκ μνήμης με βάση τον αλγόριθμο First Fit
int allocate_memory(int pid, int size) {
    for (int i = 0; i < memory_block_count; i++) {
        if (memory[i].free && memory[i].size >= size) {
            if (memory[i].size > size) {

                for (int j = memory_block_count; j > i; j--) {
                    memory[j] = memory[j - 1];
                }
                memory_block_count++;
                memory[i + 1].start = memory[i].start + size;
                memory[i + 1].size = memory[i].size - size;
                memory[i + 1].free = true;
                memory[i + 1].pid = -1;
            }
            memory[i].size = size;
            memory[i].free = false;
            memory[i].pid = pid;
            return i;
        }
    }
    return -1;
}

// Free memory
void free_memory(int pid) {
    for (int i = 0; i < memory_block_count; i++) {
        if (memory[i].pid == pid) {
            memory[i].free = true;
            memory[i].pid = -1;

            // Merge με τον επόμενο άδειο μπλοκ
            if (i + 1 < memory_block_count && memory[i + 1].free) {
                memory[i].size += memory[i + 1].size;
                for (int j = i + 1; j < memory_block_count - 1; j++) {
                    memory[j] = memory[j + 1];
                }
                memory_block_count--;
            }

            // Merge με προηγούμενο άδειο μπλοκ
            if (i > 0 && memory[i - 1].free) {
                memory[i - 1].size += memory[i].size;
                for (int j = i; j < memory_block_count - 1; j++) {
                    memory[j] = memory[j + 1];
                }
                memory_block_count--;
            }
            return;
        }
    }
}

// Print μνήμη
void print_memory() {
    printf("Memory:\n");
    for (int i = 0; i < memory_block_count; i++) {
        printf("Block %d: Start=%d, Size=%d, Free=%s, PID=%d\n",
               i, memory[i].start, memory[i].size,
               memory[i].free ? "Yes" : "No",
               memory[i].pid);
    }
    printf("\n");
}

// Print processes
void print_processes(Process *processes, int n) {
    printf("Processes:\n");
    for (int i = 0; i < n; i++) {
        printf("PID=%d, Arrival=%d, Duration=%d, Remaining=%d, MemoryNeeded=%d, InMemory=%s\n",
               processes[i].pid, processes[i].arrival_time, processes[i].duration,
               processes[i].remaining_time, processes[i].memory_needed,
               processes[i].in_memory ? "Yes" : "No");
    }
    printf("\n");
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process processes[n];
    for (int i = 0; i < n; i++) {
        printf("Enter details for process %d (arrival_time duration memory_needed): ", i + 1);
        processes[i].pid = i + 1;
        scanf("%d %d %d", &processes[i].arrival_time, &processes[i].duration, &processes[i].memory_needed);
        processes[i].remaining_time = processes[i].duration;
        processes[i].in_memory = false;
    }

    init_memory();

    int time = 0, completed = 0;
    while (completed < n) {
        bool executed = false;  // Σημαία ελέγχου εάν κάποια διεργασία εκτελέστηκε στον τρέχοντα κύκλο

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && processes[i].remaining_time > 0) {
                if (!processes[i].in_memory) {
                    int mem_index = allocate_memory(processes[i].pid, processes[i].memory_needed);
                    if (mem_index != -1) {
                        processes[i].in_memory = true;
                        printf("Time %d: Process %d allocated memory.\n", time, processes[i].pid);
                    } else {
                        continue; // Δεν υπάρχει αρκετή μνήμη, η διαδικασία θα περιμένει
                    }
                }

                int exec_time = (processes[i].remaining_time > TIME_QUANTUM) ? TIME_QUANTUM : processes[i].remaining_time;
                printf("Time %d: Process %d executing for %d ms.\n", time, processes[i].pid, exec_time);

                processes[i].remaining_time -= exec_time;
                time += exec_time;  // Αύξηση χρόνου μόνο εάν εκτελεστεί μια διαδικασία
                executed = true;

                if (processes[i].remaining_time == 0) {
                    completed++;
                    free_memory(processes[i].pid);
                    processes[i].in_memory = false;
                    printf("Time %d: Process %d completed.\n", time, processes[i].pid);
                }
            }
        }

        // Αύξηση χρόνου μόνο εάν εκτελούνταν μια διεργασία
        if (!executed) {
            time++;  //Αύξηση χρόνου εάν δεν εκτελέστηκε καμία διαδικασία (περιμένουμε να φτάσει η επόμενη διαδικασία)
        }

        print_memory();
        print_processes(processes, n);
    }

    printf("All processes completed.\n");
    return 0;
}


