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

struct timespec timespec_normalized(time_t sec, long nsec){
    while(nsec >= 1000000000){
        nsec -= 1000000000;
        ++sec;
        }
        while(nsec < 0){
            nsec += 1000000000;
            --sec;
        }
    return (struct timespec){sec, nsec};
}

struct timespec timespec_add(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec + rhs.tv_sec, lhs.tv_nsec + rhs.tv_nsec);
}


void* testfunc1(void* args){
    set_cpu(0);
    int channel=1;
   
    struct timespec waketime;
    clock_gettime(CLOCK_REALTIME, &waketime);
    struct timespec period = {.tv_sec = 0, .tv_nsec = 1*1000*1000};

    while(1){
        if(io_read(channel)==0){
            io_write(channel, 0);
            usleep(5);
            io_write(channel, 1);
        }
        waketime = timespec_add(waketime, period);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
    }
}
void* testfunc2(void* args){
    set_cpu(0);
    int channel=2;
   
    struct timespec waketime;
    clock_gettime(CLOCK_REALTIME, &waketime);
    struct timespec period = {.tv_sec = 0, .tv_nsec = 1*1000*1000};

    while(1){
        if(io_read(channel)==0){
            io_write(channel, 0);
            usleep(5);
            io_write(channel, 1);
        }
        waketime = timespec_add(waketime, period);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
    }
}
void* testfunc3(void* args){
    set_cpu(0);
    int channel=3;
   
    struct timespec waketime;
    clock_gettime(CLOCK_REALTIME, &waketime);
    struct timespec period = {.tv_sec = 0, .tv_nsec = 1*1000*1000};

    while(1){
        if(io_read(channel)==0){
            io_write(channel, 0);
            usleep(5);
            io_write(channel, 1);
        }
        waketime = timespec_add(waketime, period);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
    }
}

/*
void* testfunc2(void* args){
    set_cpu(0);
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
    set_cpu(0);
    int channel = 3;
    while(1){
        if(io_read(channel)==0){
            io_write(channel, 0);
            usleep(5);
            io_write(channel, 1);
        }
    }
}*/

/*
struct timespec waketime;
clock_gettime(CLOCK_REALTIME, &waketime);
struct timespec period = {.tv_sec = 0, .tv_nsec = 500*1000*1000};
while(1){
    // do periodic work ...
    // sleep
    waketime = timespec_add(waketime, period);
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
}
*/


void* distfunc(void* args){
    set_cpu(0);
    while(1){
        asm volatile("" ::: "memory");
    }
}


int main(){
    int n=10;
    int disturbanceOn=1;

    io_init();

    pthread_t threadHandle; 
    pthread_t threadHandle2; 
    pthread_t threadHandle3;

    pthread_t distThreadHandles[n];

    if (disturbanceOn){
        for(int i = 0; i < n; i++){
            pthread_create(&distThreadHandles[i], NULL, distfunc, NULL);
        }
    }

    pthread_create(&threadHandle, NULL, testfunc1, NULL);
    pthread_create(&threadHandle2, NULL, testfunc2, NULL);
    pthread_create(&threadHandle3, NULL, testfunc3, NULL);

    if (disturbanceOn){
        for(long i = 0; i < n; i++){
            pthread_join(distThreadHandles[i], NULL);
        }
    }
    

    pthread_join(threadHandle, NULL);
    pthread_join(threadHandle2, NULL);
    pthread_join(threadHandle3, NULL);

    return 0;
}