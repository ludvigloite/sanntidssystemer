#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


#include <native/task.h>
#include <native/mutex.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>

#include <native/sem.h>


RT_MUTEX mutex;

#define TIME_UNIT 1000000



int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}


void sem_task_low(void){
	set_cpu(0);
	rt_mutex_acquire(&mutex,TM_INFINITE);
	rt_printf("Task LOW begynner på jobb! \n");
	rt_timer_spin(3*TIME_UNIT);
	rt_mutex_release(&mutex);
	rt_printf("Task LOW har fullført jobb! \n");
		
}


void sem_task_medium(void){
	set_cpu(0);
	rt_task_sleep(TIME_UNIT);

	rt_printf("Task MEDIUM begynner busy wait! \n");
	rt_timer_spin(5*TIME_UNIT);
	rt_printf("Task MEDIUM har fullført busy wait! \n");

	
		
}

void sem_task_high(void){
	set_cpu(0);
	rt_task_sleep(2*TIME_UNIT);
	rt_mutex_acquire(&mutex,TM_INFINITE);

	rt_printf("Task HIGH begynner å jobbe! \n");
	rt_timer_spin(2*TIME_UNIT);
	rt_mutex_release(&mutex);
	rt_printf("Task HIGH har fullført jobben! \n");
	
}


int main(){
	mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_print_auto_init(1);
	rt_task_shadow(NULL, NULL, 95, T_CPU(0));


	RT_TASK taskLOW, taskMEDIUM, taskHIGH;

	rt_mutex_create(&mutex, NULL); 

	rt_task_create(&taskLOW, NULL, 0, 50, T_CPU(0));
	rt_task_create(&taskMEDIUM, NULL, 0, 70, T_CPU(0));	
	rt_task_create(&taskHIGH, NULL, 0, 90, T_CPU(0));	

	rt_task_start(&taskLOW, (void*) sem_task_low, NULL);
	rt_task_start(&taskMEDIUM, (void*) sem_task_medium, NULL);
	rt_task_start(&taskHIGH, (void*) sem_task_high, NULL);



	rt_task_sleep(100000000); //100ms

	rt_mutex_delete(&mutex);
	

	return 0;

}


