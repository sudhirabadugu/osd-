#include<stdio.h>
#include<sys/types.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<fcntl.h>
//A structure for each process that creates memory and cpu cycles and process id
struct process_details
{
int pid;
int memory_kb;
int cpu_cycles;
};
//A function for genaration memory and cpu cycles
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
// To calcualte the time taken for the memory allocation
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
				//printf("\n process id %d,\n cpu cycles %d,\n memory size in %d kb.",list[i].pid,list[i].cpu_cycles,list[i].memory_kb);
				exit(0);
			}
	else
	{
 	sleep(1);
	}
	memory_list[i]=memory_list[i]*1024;
	time=time+cpu_list[i];
	int *o=(int)malloc(memory_list[i]);
	
	free(o);
	}
	gettimeofday(&stop,NULL);
	 time= ((stop.tv_sec * 1000000 + stop.tv_usec)
                  - (start.tv_sec * 1000000 + start.tv_usec));
	time=time/1000000;
	printf("\n time taken for memory allocation completion is  %d micro seconds \n ",time);
}
