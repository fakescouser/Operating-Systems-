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
#define CHANGE_MIND_PROBABILITY 0.2 // 20% na allaksi gnomi

// Leitourgia gia epivates pou prothpasoun na epivivastoun 
void* passenger_board(void* arg) {
    int passenger_id = *((int*)arg);

    while (1) {
        printf("Passenger %d: Waiting to board the lifeboat...\n", passenger_id);

        wait_semaphore(&queue_access); // Prosvasi stin oura epivivasis
        if (sem_trywait(&lifeboat_capacity) == 0) { // Elegxos an uparxei xwros
            // Prosomoiwsi allagis apofasis
            if ((rand() % 100) < (CHANGE_MIND_PROBABILITY * 100)) {
                printf("Passenger %d: Changed their mind! Leaving the queue and rejoining at the end.\n", passenger_id);
                signal_semaphore(&lifeboat_capacity); // Eleutherwsi thesis
                signal_semaphore(&queue_access);      // Eleutherwsi provasis sti seira
                sleep(1); // Kathisterisi prin tin epanentaxi
                continue; // Try again
            }

            // Epivivasi sto lifeboat
            printf("Passenger %d: Boarding the lifeboat.\n", passenger_id);
            sleep(1); // Xronos gia tin epivivasi

            signal_semaphore(&queue_access); // Eleutherwsi prosvasis sti seira
            printf("Passenger %d: Successfully boarded.\n", passenger_id);
            break; // Eksodos meta tin epivivasi
        } else {
            // Oura gemati, eleutherwnoume tin prosvasi kai try again
            printf("Passenger %d: Lifeboat full, waiting...\n", passenger_id);
            signal_semaphore(&queue_access);
            sleep(1);
        }
    }
    return NULL;
}
