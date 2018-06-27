//@author 161520211 hxy
#include<stdio.h>
#include<pthread.h>

//定义数组长度
#define LEN 10
//定义排序的数组
int num[] ={1,10,3,8,5,6,7,4,9,2}; 



void* worker(void *arg)
{
	int i,j;

	//简单的选择排序
	for(i = 0;i < LEN/2;i++)
	{
		int min = i;
		for(j = i;j < LEN/2;j++)
		{
			if(num[j] < num[min])
			{
				min = j;
			}
		}
		int temp = num[i];
		num[i] = num[min];
		num[min] = temp;
	}
}

void master()
{
	int i,j;

	//简单的选择排序
	for(i = LEN/2;i < LEN;i++)
	{
		int min = i;
		for(j = i;j < LEN;j++)
		{
			if(num[j] < num[min])
			{
				min = j;
			}
		}
		int temp = num[i];
		num[i] = num[min];
		num[min] = temp;
	}
}

int main()
{
	pthread_t worker_tid;

	pthread_create(&worker_tid,NULL,worker,NULL);
	master();
	pthread_join(worker_tid,NULL);
	
	int i;
//	for(i = 0;i <LEN;i++)	printf("%d ",num[i]);

	//进行归并排序
	int temp[LEN];
	int p = 0,q = LEN / 2;		
	i = 0;	
	while(p < LEN/2 || q < LEN )
	{
		if(p >= LEN/2)	
		{
			temp[i] = num[q];
			i++;
			q++;
		}
		else if(q >= LEN)	
		{
			temp[i] = num[p];
			i++;
			p++;
		}
		else if(num[p] < num[q])
		{
			temp[i] = num[p];
			i++;
			p++;	
		}
		else
		{
			temp[i] = num[q];
			i++;
			q++;
		}
	}
	for(i = 0;i <LEN;i++)	printf("%d ",temp[i]);
	printf("\n");
}
