#include"stdio.h"
#include"stdint.h"
#include"pthread.h"
#include"time.h"
#include"string.h"
#include"stdlib.h"
#include "unistd.h"

struct timespec tmp1;
struct timespec tmp2;
struct timespec t1;
struct timespec t2;
unsigned long freq;
  
unsigned long get_freq()
{
 	unsigned long data;
 	FILE *f;
	f = fopen("freq.txt","r");
	char buff[100];
	fgets(buff,sizeof(buff),f);
	char *eptr;
	data = strtol(buff,&eptr,10);
	fclose(f);
	return data;
 }

void *getTime(void *args )
{		
	while (1)
	{
		//unsigned long x = *((unsigned long*)args);
		if (clock_nanosleep(CLOCK_REALTIME,0,&t1,&t2) == 0)
		{
			clock_gettime(CLOCK_REALTIME,&tmp1);
		}
		
		
			
			
	}
		
		
}

void *getFreq(void *args)
{
	unsigned long x = *((unsigned long*)args);
	while(1){
		
		
		unsigned long new_freq = get_freq();
		
		if ( new_freq != x )
		{
			freq = new_freq;
			
			if (freq<1000000000)
			{
				t1.tv_sec = 0;
				t1.tv_nsec = freq;
			}
			else{
				t1.tv_sec = freq/1000000000;
				t1.tv_nsec = freq%1000000000;
			} 
		}
		
		}
	
}

void *save_time(void *args)
{
	tmp1 = *((struct timespec *)args);
	tmp2.tv_sec = 0;
	tmp2.tv_nsec = 0; 
	while(1){
		
  
		if(tmp2.tv_sec != tmp1.tv_sec | tmp2.tv_nsec != tmp1.tv_nsec)
		{
		FILE *file;
		file = fopen("time_and_interval.txt","a+");
		//file = fopen("100.txt","a+");
		
		unsigned long sub_sec;
		unsigned long sub_nsec;

		
		if(tmp1.tv_nsec < tmp2.tv_nsec){
			sub_nsec = 1000000000 + tmp1.tv_nsec - tmp2.tv_nsec;
			sub_sec = tmp1.tv_sec - tmp2.tv_sec - 1;

		}
		else
		{
			sub_nsec =  tmp1.tv_nsec - tmp2.tv_nsec;
			sub_sec = tmp1.tv_sec - tmp2.tv_sec;
		}
		
		fprintf(file,"\n%ld.%09ld\n%ld.%09ld",tmp1.tv_sec,tmp1.tv_nsec,sub_sec,sub_nsec); 
		//fprintf(file,"\n%ld.%09ld",sub_sec,sub_nsec);  
		fclose(file);
		tmp2.tv_sec = tmp1.tv_sec;
		tmp2.tv_nsec = tmp1.tv_nsec;
		
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
   	t1.tv_sec = 0;
   	t1.tv_nsec = freq; 
	tmp1.tv_sec = 0;
	tmp1.tv_sec = 0;
	       
   	a1 = pthread_create(&INPUT,NULL,getFreq,&freq);
    a2 = pthread_create(&SAMPLE, NULL, getTime,&freq);
    a3 = pthread_create(&LOGGING,NULL,save_time,&tmp1);
    pthread_join(INPUT,NULL);
    pthread_join(SAMPLE,NULL);
	pthread_join(LOGGING,NULL);
	return 0;
}