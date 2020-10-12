#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

// Note the argument and return types: void*

long variable = 0;
sem_t semVar;

void* fn(void* args){

    long nuIterations = 50000000;
    long localVar = 0;
    for(int i=0;i<nuIterations;i++){
        localVar += 1;
        sem_wait(&semVar);
        variable+= 1;
        sem_post(&semVar);
        
    }
    printf("local: %ld, global: %ld \t",localVar,variable);
    printf("\n");
    return NULL;
}

int main(){
    sem_init(&semVar, 0, 1);

    pthread_t threadHandle;
    pthread_t threadHandle2;
    pthread_t threadHandle3;
    pthread_create(&threadHandle, NULL, fn, NULL);
    pthread_create(&threadHandle2, NULL, fn, NULL);
    pthread_create(&threadHandle3, NULL, fn, NULL);
    pthread_join(threadHandle, NULL);
    pthread_join(threadHandle2, NULL);
    pthread_join(threadHandle3, NULL);
    return 0;
}
