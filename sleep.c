#include<stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <x86intrin.h>
#include <sched.h>
#include <string.h>

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
        
struct timespec timespec_sub(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec - rhs.tv_sec, lhs.tv_nsec - rhs.tv_nsec);
}
   
    
struct timespec timespec_add(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec + rhs.tv_sec, lhs.tv_nsec + rhs.tv_nsec);
}
    

int timespec_cmp(struct timespec lhs, struct timespec rhs){
    if (lhs.tv_sec < rhs.tv_sec)
        return -1;
    if (lhs.tv_sec > rhs.tv_sec)
        return 1;
    return lhs.tv_nsec - rhs.tv_nsec;
}


void sleep_sec(struct timespec t){
    nanosleep(&t, NULL);
}


void busy_wait(struct timespec t){
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    struct timespec then = timespec_add(now, t);

    while(timespec_cmp(now, then) < 0){
        for(int i = 0; i < 10000; i++){}
        clock_gettime(CLOCK_MONOTONIC, &now);
    }
}


void busy_wait2(struct tms t){
    static struct tms var;
    times(&var);
    struct tms then = var;
    while (then.tms_stime + 100 > var.tms_stime){
        for(int i = 0; i < 10; i++){}
            times(&var);
    }
}

void latencyEstimator(){
    for(int i = 0; i < 10*1000*1000; i++){
           __rdtsc();
    }
}
void resolutionEstimate(){
    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);
    static struct tms tmsVar;
    struct tms t1;
    struct tms t2;

    for(int i = 0; i < 10*1000*1000; i++){
        times(&t1);
        times(&t2);
        //t1 = rdtsc();
        //t2 = rdtsc();
        int ns = (t2.tms_stime - t1.tms_stime) * 100;

        if(ns >= 0 && ns < ns_max){
            histogram[ns]++;
        }
    }
    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
}

void contextSwitchEstimate(){
    int ns_max = 700;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);
    struct timespec now;
    struct timespec then;
    for(int i = 0; i < 10*1000*1000; i++){
        clock_gettime(CLOCK_MONOTONIC, &then);
        sched_yield();
        clock_gettime(CLOCK_MONOTONIC, &now);
        int ns = now.tv_nsec - then.tv_nsec;
        //printf("%d",ns);

        if(ns >= 0 && ns < ns_max){
            histogram[ns]++;
        }
    }
    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
}


int main(){
    struct timespec timeSpecVar;
    timeSpecVar.tv_sec = 1;
    timeSpecVar.tv_nsec = 0L;

    static struct tms tmsVar;

    sleep_sec(timeSpecVar);

    //busy_wait2(tmsVar);
    //busy_wait(timeSpecVar);
    
    //latencyEstimator();
    //times() - 5,399sek
    //clock_gettime() - 1,227sek
    //rdtsc() - 1,086sek
    contextSwitchEstimate();
    //resolutionEstimate();





    return 0;
}
