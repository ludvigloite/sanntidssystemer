#include <pthread.h>
#include <stdio.h>

#define N 5
pthread_mutex_t mtx[N];
pthread_t threadHandles[N];

int forkflag[N];

static inline void nonOptimizedBusyWait(void){
    for(long i = 0; i < 10000000; i++){
        // "Memory clobber" - tells the compiler optimizer that all the memory 
        // is being touched, and that therefore the loop cannot be optimized out
        asm volatile("" ::: "memory");
    }
}
/*
void* eat(int i){

    while(1){
        printf("philisopher %d is waiting for fork :)\n", i);

        int j = 0;

        if (i == 0){
            j = 4;
        }else{
            j = i-1;
        }

        pthread_mutex_lock(&mtx[i]);
        pthread_mutex_lock(&mtx[j]);
        printf("låste gaffel %d og %d \n",i,j);
        nonOptimizedBusyWait();
        pthread_mutex_unlock(&mtx[j]);
        pthread_mutex_unlock(&mtx[i]);
        //nonOptimizedBusyWait();
    }
    

    return NULL;
}
*/


void* eat(int i){

    while(1){
        printf("philisopher %d is waiting for fork :)\n", i);

        int j = 0;

        if (i == 0){
            j = 4;
        }else{
            j = i-1;
        }

        pthread_mutex_lock(&mtx[i]);
        forkflag[i] = 1;
        if (forkflag[j]){
            forkflag[i] = 0;
            pthread_mutex_unlock(&mtx[i]);
        }
        pthread_mutex_lock(&mtx[j]);
        forkflag[j] = 1;
        printf("låste gaffel %d og %d \n",i,j);
        nonOptimizedBusyWait();
        forkflag[j] = 0;
        pthread_mutex_unlock(&mtx[j]);
        forkflag[i] = 0;
        pthread_mutex_unlock(&mtx[i]);
        //nonOptimizedBusyWait();
    }
    

    return NULL;
}

int main(){

    for(long i = 0; i < N; i++){
        // 2nd arg is a pthread_mutexattr_t
        pthread_mutex_init(&mtx[i], NULL);
    }
    
    for(int i = 0; i < N; i++){
        pthread_create(&threadHandles[i], NULL, (void *)eat, (int*)i);
    }

    for(long i = 0; i < N; i++){
        pthread_join(threadHandles[i], NULL);
    }

    pthread_mutex_lock(&mtx[1]);
    // Critical section
    pthread_mutex_unlock(&mtx[1]);


    for(long i = 0; i < N; i++){
        pthread_mutex_destroy(&mtx[i]);
    }
}