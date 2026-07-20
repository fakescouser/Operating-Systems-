//Kiourt Mechmetali Achmet 1084672
//Lalas Georgios 1093406
//Gkourvelos Leonidas 1072506
//Georgiou Konstantinos 1067419
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "ipc_utils.h"

#define MAX_PASSENGERS 100

// Orismos Koinwn Sumaforwn
sem_t lifeboat_capacity;
sem_t queue_access;

// Passenger thread function (declared in passenger.c)
extern void* passenger_board(void* arg);

int main() {
    int total_passengers, lifeboat_capacity_val;

    printf("Enter the total number of passengers: ");
    scanf("%d", &total_passengers);

    printf("Enter the lifeboat capacity: ");
    scanf("%d", &lifeboat_capacity_val);

    if (total_passengers > MAX_PASSENGERS) {
        printf("Error: Exceeded maximum passengers (%d).\n", MAX_PASSENGERS);
        return 1;
    }

    pthread_t passengers[total_passengers];
    int passenger_ids[total_passengers];

    // Initialize semaphores
    init_semaphore(&lifeboat_capacity, lifeboat_capacity_val);
    init_semaphore(&queue_access, 1);

    // Create passenger threads
    for (int i = 0; i < total_passengers; i++) {
        passenger_ids[i] = i + 1;
        pthread_create(&passengers[i], NULL, passenger_board, &passenger_ids[i]);
        sleep(0.1); // delay stin prosomoiwsi tis diadikasias epivivasis
    }

    // Join passenger threads
    for (int i = 0; i < total_passengers; i++) {
        pthread_join(passengers[i], NULL);
    }

    // Destroy semaphores
    destroy_semaphore(&lifeboat_capacity);
    destroy_semaphore(&queue_access);

    printf("All passengers have boarded. Lifeboat process complete.\n");
    return 0;
}
