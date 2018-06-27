//@author 161520211 hxy
#include<stdio.h>
#include<pthread.h>

//定义级数运算的项数
#define NUM 1000


double master_output;
double worker_output; 

void* worker(void *arg)
{
	worker_output = 0;
	int i;
	//用来记录-1的幂
	int p;
	for(i = 1;i <= NUM/2;i++)
	{
		if(i % 2 == 1)
		{
			p = 1;
		}
		else
		{
			p = -1;
		}
		worker_output += 1.0 / (2*i - 1) *  p;
			
	}
}

void master()
{
	master_output = 0;
	int i;
	//用来记录-1的幂
	int p = 1;
	for(i = NUM/2 + 1;i <= NUM;i++)
	{
		if(i % 2 == 1)
		{
			p = 1;
		}
		else
		{
			p = -1;
		}
		master_output += 1.0 / (2*i - 1) *  p;
		
	}
}

int main()
{
	pthread_t worker_tid;
	double tatal;

	pthread_create(&worker_tid,NULL,worker,NULL);
	master();
	pthread_join(worker_tid,NULL);
	printf("total = %lf\n",master_output + worker_output);
}
