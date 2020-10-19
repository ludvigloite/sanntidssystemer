#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>

#include "io.h"


int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}


void* response_task1(void* args){
	set_cpu(0);
	
	unsigned long duration = 50000000000; // 10 second timeout
	unsigned long endTime = rt_timer_read() + duration;
	int channel = 1;
	while(1){
		rt_task_wait_period(NULL);
		if(io_read(channel)==0){ //samme som øving 6, kun byttet delaytype
		    io_write(channel, 0);
		    rt_timer_spin(5000); //5ms
		    io_write(channel, 1);
		}
			

		if(rt_timer_read() > endTime){
			rt_printf("Time expired\n");
			rt_task_delete(NULL);
		}
		if(rt_task_yield()){
			rt_printf("Task failed to yield\n");
			rt_task_delete(NULL);
		}
	}

}

void* response_task2(void* args){
	set_cpu(0);
	
	unsigned long duration = 50000000000; // 10 second timeout
	unsigned long endTime = rt_timer_read() + duration;
	int channel = 2;
	while(1){
		rt_task_wait_period(NULL);
		if(io_read(channel)==0){ //samme som øving 6, kun byttet delaytype
		    io_write(channel, 0);
		    rt_timer_spin(5000); //5ms
		    io_write(channel, 1);
		}
				


		if(rt_timer_read() > endTime){
			rt_printf("Time expired\n");
			rt_task_delete(NULL);
		}
		if(rt_task_yield()){
			rt_printf("Task failed to yield\n");
			rt_task_delete(NULL);
		}
	}

}

void* response_task3(void* args){
	set_cpu(0);
	
	unsigned long duration = 50000000000; // 10 second timeout
	unsigned long endTime = rt_timer_read() + duration;
	int channel = 3;
	while(1){
		rt_task_wait_period(NULL);
		if(io_read(channel)==0){ //samme som øving 6, kun byttet delaytype
		    io_write(channel, 0);
		    rt_timer_spin(5000); //5ms
		    io_write(channel, 1);
		}
				


		if(rt_timer_read() > endTime){
			rt_printf("Time expired\n");
			rt_task_delete(NULL);
		}
		if(rt_task_yield()){
			rt_printf("Task failed to yield\n");
			rt_task_delete(NULL);
		}
	}

}

void* disturbance_func(void* args){ //Som øving 6
	set_cpu(0);
   	while(1){
        	asm volatile("" ::: "memory");
	}

}

int main(){
	mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_print_auto_init(1);
	io_init();

	int n = 10;

	RT_TASK task1, task2, task3;
	pthread_t dist_threads[n];

	rt_task_create(&task1, NULL, 0, 95, T_CPU(0));
	rt_task_create(&task2, NULL, 0, 95, T_CPU(0));	
	rt_task_create(&task3, NULL, 0, 95, T_CPU(0));

	rt_task_set_periodic(&task1, TM_NOW, 1000);

	
	for(int i = 0; i<n; i++) {
		pthread_create(&dist_threads[i], NULL, (void *)disturbance_func, NULL);
	}
	

	rt_task_start(&task1, (void*) response_task1, NULL);
	rt_task_start(&task2, (void*) response_task2, NULL);
	rt_task_start(&task3, (void*) response_task3, NULL);
	
	
	for(int i = 0; i<n; i++) {
		  pthread_join(dist_threads[i], NULL);
	}
		

	while(1){
		usleep(-1);
	}

	return 0;

}
