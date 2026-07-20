//Kiourt Mechmetali Achmet 1084672
//Lalas Georgios 1093406
//Gkourvelos Leonidas 1072506
//Georgiou Konstantinos 1067419
#include "ipc_utils.h"

// Initialize semaphore
void init_semaphore(sem_t *sem, int value) {
    sem_init(sem, 0, value);
}

// Wait on semaphore
void wait_semaphore(sem_t *sem) {
    sem_wait(sem);
}

// Signal semaphore
void signal_semaphore(sem_t *sem) {
    sem_post(sem);
}

// Destroy semaphore
void destroy_semaphore(sem_t *sem) {
    sem_destroy(sem);
}
