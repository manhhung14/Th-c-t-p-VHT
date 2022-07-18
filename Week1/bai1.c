#include"stdio.h"
#include"stdint.h"
#include"pthread.h"
#include"time.h"
#include"string.h"
#include"stdlib.h"
#include "unistd.h"

struct timespec T;
struct timespec tp;
struct timespec temp;
long freq;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void sleep_until(struct timespec *ts, int delay)
{
        ts->tv_nsec += delay;
        if(ts->tv_nsec > 1000*1000*1000){
                ts->tv_nsec -= 1000*1000*1000;
                ts->tv_sec++;
        }
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, ts,  NULL);
}

long get_freq()
{
 	long data;
 	FILE *f;
	f = fopen("freq.txt","r");
	char buff[100];
	fgets(buff,sizeof(buff),f);
	char *eptr;
	data = strtol(buff,&eptr,10);
	fclose(f);
	return data;
 }

void *getTime(void *args)
{	long freq = *((long*)args);
	clock_gettime(CLOCK_REALTIME,&temp);	
	while (1)
	{
		
		sleep_until(&temp, freq);
		clock_gettime(CLOCK_REALTIME,&T);
			
	}
		
		
}

void *getFreq(void *args)
{
	long freq = *((long*)args);
	
	while(1){
		
		pthread_mutex_lock(&lock);
		long new_freq = get_freq();
		
		if ( new_freq != freq )
		{
			freq = new_freq;
			
			
		}
		pthread_mutex_unlock(&lock);
		
		
		}
	
}

void *save_time(void *args)
{
	tp.tv_sec = 0;
	tp.tv_nsec = 0; 
	while(1){
		
		if(tp.tv_sec != T.tv_sec || tp.tv_nsec != T.tv_nsec)
		{
		FILE *file;
		file = fopen("time_and_interval.txt","a+");
		//file1 = fopen("100.txt","a+");
		long sub_sec;
		long sub_nsec;
		if(T.tv_nsec < tp.tv_nsec){
			sub_nsec = 1000000000 + T.tv_nsec - tp.tv_nsec;
			sub_sec = T.tv_sec - tp.tv_sec - 1;

		}
		else
		{
			sub_nsec =  T.tv_nsec - tp.tv_nsec;
			sub_sec = T.tv_sec - tp.tv_sec;
		}
		
		fprintf(file,"\n%ld.%09ld\n%ld.%09ld",T.tv_sec,T.tv_nsec,sub_sec,sub_nsec); 
		//fprintf(file1,"\n%ld.%09ld",sub_sec,sub_nsec); 
		tp.tv_sec = T.tv_sec;
		tp.tv_nsec = T.tv_nsec; 
		fclose(file);
		//fclose(file1);
		
		}
		
	}
  
}

int main(int argc, char const *argv[])
{
	
	freq = get_freq();
    	pthread_t SAMPLE;
    	pthread_t INPUT;
    	pthread_t LOGGING;
    	int a1, a2, a3;
	T.tv_sec = 0;
	T.tv_nsec = 0;
	pthread_mutex_init(&lock, NULL);      
   	a1 = pthread_create(&INPUT,NULL,getFreq,&freq);
    	a2 = pthread_create(&SAMPLE, NULL,getTime,&freq);
    	a3 = pthread_create(&LOGGING,NULL,save_time,&T);
    	pthread_join(INPUT,NULL);
    	pthread_join(SAMPLE,NULL);
	pthread_join(LOGGING,NULL);
	pthread_mutex_destroy(&lock);
	return 0;
}
