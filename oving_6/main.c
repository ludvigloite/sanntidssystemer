#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

#include "io.h"


int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}


void* testfunc1(void* args){
    int channel=1;
    while(1){
        if(io_read(channel)==0){
            io_write(channel, 0);
            usleep(5);
            io_write(channel, 1);
        }
    }
}
void* testfunc2(void* args){
    int channel = 2;
    while(1){
        if(io_read(channel)==0){
            io_write(channel, 0);
            usleep(5);
            io_write(channel, 1);
        }
    }
}
void* testfunc3(void* args){
    int channel = 3;
    while(1){
        if(io_read(channel)==0){
            io_write(channel, 0);
            usleep(5);
            io_write(channel, 1);
        }
    }
}



int main(){
    printf("gello world");
    io_init();

    pthread_t threadHandle; 
    pthread_t threadHandle2; 
    pthread_t threadHandle3;
    
    printf("gello world2");
    pthread_create(&threadHandle, NULL, testfunc1, NULL);
    pthread_create(&threadHandle2, NULL, testfunc2, NULL);
    pthread_create(&threadHandle3, NULL, testfunc3, NULL);

    pthread_join(threadHandle, NULL);
    pthread_join(threadHandle2, NULL);
    pthread_join(threadHandle3, NULL);

    return 0;
}