#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>

static unsigned char our_memory[1024 * 1024*10]; //reserve 10 MB for malloc
static size_t next_index = 0;
// Replica of systems malloc
void *mymalloc(size_t sz)
{
    void *mem;
    if(sizeof our_memory - next_index < sz)
		{
		printf("\n memory is not available");
        	return NULL;
		}	
    next_index=next_index+sz;
    mem = &our_memory[next_index];
    return mem;
}
//Replica of system's free
void myfree(void *mem,int sz)
{
next_index=next_index-sz;
}
// A structure for each process created for its memory and cpu
struct process_details
{
int pid;
int memory_kb;
int cpu_cycles;
};
// A function to genrate the memory and cp cycles
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
// To calculate time taken for memory management
	struct timeval start,stop; 
	gettimeofday(&start,NULL);
	int i,status,p,n,time=0;
	printf("\n enter number of process to be created :");
	scanf("%d",&n);
	struct process_details list[n];
	int cpu_list[n];
	int memory_list[n];
	gen(1000,11000,6000,n,cpu_list);
	gen(1,200,200,n,memory_list);
	for(i=0;i<n;i++)
	{
		if(!(p=fork()))
		{
			//printf("\n %d procees created ",i+1);
			list[i].pid=getpid();
			list[i].cpu_cycles=cpu_list[i];
			list[i].memory_kb=memory_list[i];
			p=wait(&status);
			//printf("\n parent id %d,\n cpu cycles %d,\n memory size %d in kb.",list[i].pid,list[i].cpu_cycles,list[i].memory_kb);
			exit(0);
		}
	else
	{
 		sleep(1);
	}
	memory_list[i]=memory_list[i]*1024;
	int *l= mymalloc(memory_list[i]);
	//printf("\n memory allocated \n ");
	myfree(l,memory_list[i]);
	//printf("\n memory freed");

}
	gettimeofday(&stop,NULL);
 	time= ((stop.tv_sec * 1000000 + stop.tv_usec)
                  - (start.tv_sec * 1000000 + start.tv_usec));
	time=time/1000000;
	printf("\n time taken for memory allocation completion is  %d micro seconds \n ",time);


}
