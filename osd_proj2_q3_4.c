#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
int  time=0;
int rear_wait=-1;
int free_memory=1024*20;
static unsigned char our_memory[1024 * 20]; //reserve 20kb for malloc
static size_t next_index = 0;
int wait_que[50];
int cpu_list[50];
int memory_list[50];
// Insert function for inserting a process in the queue while memory is unavialable
void insert_wait(int i)
{
	rear_wait=rear_wait+1;
	wait_que[rear_wait]=i;
}
void delete(int i)
{
	for(i;i<=rear_wait;i++)
        {
        	wait_que[i]=wait_que[i+1];
        }
    	rear_wait=rear_wait-1;


}
//Replica of system's malloc
void *mymalloc(size_t sz)
{
    void *mem;

    if(sizeof our_memory - next_index < sz)
        return NULL;
 	 next_index+=sz;
    mem = &our_memory[next_index];
    return mem;
    free_memory=free_memory-sz;
}
// Replica of system's free
void myfree(void *mem,int sz)
{
next_index=next_index-sz;
free_memory=free_memory+sz;

}
// to allocate the memory for every process created
void memory_allocation(int i)
{
	int m;
	memory_list[i]=memory_list[i]*1024;
	if(memory_list[i]>free_memory)
		{
		       	printf("\n memory is insuffient waiting in the queue");
			insert_wait(i+1);
		}
	else
	{
		printf("\n memeory allocated");
		int *k=(int)mymalloc(memory_list[i]);
		time=time+cpu_list[i];
		printf("\n memory freed");
		myfree(k,memory_list[i]);
	}	
	while(rear_wait!=-1)
	{
	i=0;
	for(i;i<=rear_wait;i++)
	{
	m=wait_que[i];
	m=m-1;
	//Converting memory into bytes from kb
	memory_list[m]=memory_list[m]*1024;
	if(memory_list[m]<free_memory)
		{
	int *l=(int)mymalloc(memory_list[m]);
	time=time+cpu_list[m];
	myfree(l,memory_list[m]);
	delete(m);
		}	

	}
	}	
}
// A structure for each process that creates a processid,required memory and cpu cycles
struct process_details
{
	int pid;
	int memory_kb;
	int cpu_cycles;
	int arrival_time;
};
//To genrate the values for memory and cpucycles
void gen(int min,int max,int mean,int k,int a[])
{
	int i=0,sum=0;
	if(k%2==0)
	{
		while(i!=k)
		{
			a[i]=rand()%((max)+1-min)+min;
			i=i+1;
			a[i]=(2*mean) -a[i-1];
			i++;
		}		
	}
	else
	{
	a[i]=mean;
	i=i+1;
	while(i!=k)
	{
	a[i]=rand()%((max)+1-min)+min;
	i=i+1;
	a[i]=(2*mean)-a[i-1];
	i++;
	}	
	}
}
int main()
{
	// To calculate the time of the memory allocation
	struct timeval start,stop;
	gettimeofday(&start,NULL);
	int i,status,p,n,time=0;
	printf("\n Asuming  that the entire memory required is 8 MB \n ");
	printf("\n The memory block avaialable for management is 10 percent of required \n");
	printf("\n enter number of process to be created:");
	scanf("%d",&n);
	struct process_details list[n];
	gen(1000,11000,6000,n,cpu_list);
	gen(1,200,200,n,memory_list);
	for(i=0;i<n;i++)
	{
		if(!(p=fork()))
			{
				printf("\n %d procees created ",i+1);
				list[i].pid=getpid();
				list[i].cpu_cycles=cpu_list[i];
				list[i].memory_kb=memory_list[i];
				list[i].arrival_time=i*50;
				p=wait(&status);
				printf("\n parent id %d,\n cpu cycles %d,\n memory size %d.",list[i].pid,list[i].cpu_cycles,list[i].memory_kb);
				exit(0);
			}		
		else
			{
 			sleep(1);
			}
		memory_allocation(i);
	}	
	gettimeofday(&stop,NULL);
 	time= ((stop.tv_sec * 1000000 + stop.tv_usec)
                  - (start.tv_sec * 1000000 + start.tv_usec));
	time=time/1000000;
	printf("\n time taken for memory allocation completion is  %d micro seconds \n ",time);
}
