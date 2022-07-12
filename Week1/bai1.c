#include"stdio.h"
#include"stdint.h"
#include"pthread.h"
#include"time.h"
#include"string.h"
#include"stdlib.h"
#include "unistd.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
struct timespec tmp;
struct timespec time1;
long freq;
long check_freq;
  

long get_freq()
{
 	long data;
 	FILE *f;
	f = fopen("freq.txt","r");
	char buff[150];
	fgets(buff,sizeof(buff),f);
	char *eptr;
	data = strtol(buff,&eptr,10);
	fclose(f);
	return data;
 }

void *getTime(void *args )
{		long x = *((long*)args);

		clock_gettime(CLOCK_REALTIME,&tmp);	
		return NULL;
}

void *getFreq(void *args)
{
	pthread_mutex_lock(&mutex);
	long x = *((long*)args);
	long old_freq = get_freq();
	if(old_freq == x)
	{
		pthread_mutex_lock(&mutex);
		return NULL;
	}
   	else
   	{
		FILE *f;
		f = fopen("freq.txt","w");
		fprintf(f,"%ld",x);
		fclose(f);
		freq = x;
		time1.tv_sec = 0;
	   	time1.tv_nsec = freq;
   	}
	pthread_mutex_lock(&mutex); 
   	return NULL;

}
void *save_time(void *args)
{
	struct timespec *tp = (struct timespec *)args;

	//get time from time_and_interval;
	FILE *f;
	f = fopen("time_and_interval.txt","r");
	char buff[100];
	fgets(buff,sizeof(buff),f);
	char *t_sec;
	char *t_nsec;
	t_sec = strtok(buff,".");
	t_nsec = strtok(NULL,".");
	fclose(f);
	char *eptr;
	long old_nsec;
	long old_sec;
	old_nsec = strtol(t_nsec,&eptr,10);
	old_sec = strtol(t_sec,&eptr,10);

	//save time
	FILE *file;
	file = fopen("time_and_interval.txt","w+");
	long diff_sec = ((long) tp->tv_sec) - old_sec ;
	long diff_nsec;
	if(tp->tv_nsec > old_nsec)
		{
			diff_nsec = tp->tv_nsec - old_nsec;
		}
	else 
		{
			diff_nsec = old_nsec - tp->tv_nsec;
			diff_sec = diff_sec - 1;
		}
	fprintf(file, "%ld.%09ld\n%ld.%09ld",tp->tv_sec,tp->tv_nsec,diff_sec,diff_nsec);	
	fclose(file);
}

int main(int argc, char const *argv[])
{
	
		FILE *fp;
		freq = get_freq();
        pthread_t SAMPLE;
        pthread_t INPUT;
        pthread_t LOGGING;
        int a1, a2, a3;
   		time1.tv_sec = 0;
   		time1.tv_nsec = freq;
		pthread_mutex_init(&mutex, NULL);        
   		while(1)
        {
			a1 = pthread_create(&INPUT,NULL,getFreq,&check_freq);
        	a2 = pthread_create(&SAMPLE, NULL, getTime,&freq);
        	a3 = pthread_create(&LOGGING,NULL,save_time,&tmp);
        	pthread_join(INPUT,NULL);
        	pthread_join(SAMPLE,NULL);
			pthread_join(LOGGING,NULL);
			pthread_mutex_init(&mutex, NULL);
			sleep(1);
			
        }	
		return 0;
}