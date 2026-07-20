#ifndef IPC_UTILS_H
#define IPC_UTILS_H

#include <semaphore.h>
#include <pthread.h>

// Semaphore initialization
void init_semaphore(sem_t *sem, int value);

// Semaphore wait
void wait_semaphore(sem_t *sem);

// Semaphore signal
void signal_semaphore(sem_t *sem);

// Semaphore destroy
void destroy_semaphore(sem_t *sem);

// Shared semaphores (extern for global visibility)
extern sem_t lifeboat_capacity;
extern sem_t queue_access;

#endif
