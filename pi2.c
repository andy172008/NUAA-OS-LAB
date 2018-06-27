//@author 161520211 hxy
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

//定义级数运算的项数
#define NUM 1000
#define CPU 4
#define CHILD (NUM / CPU)

struct param
{
	int begin;
	int end;
};
struct result
{
	double sum;
};

void* worker(void *arg)
{
	struct param *param;
	struct result *result;
	int i;


	result = malloc(sizeof(struct result));
	result->sum = 0;
	//用来记录-1的幂
	int p = 1;
	param = (struct param *)arg;
	for(i = param->begin;i <= param->end;i++)
	{
		if(i % 2 == 1)
		{
			p = 1;
		}
		else
		{
			p = -1;
		}
		result->sum += 1.0 / (2*i - 1) *  p;

	}
	return result;
}



int main()
{
	pthread_t workers[CPU];
	struct param params[CPU];
	int i;

	for(i = 0;i < CPU;i++)
	{
		struct param *param;
		param = &params[i];
		param->begin = i * CHILD + 1; 
		param->end = (i + 1)  * CHILD;
		pthread_create(&workers[i], NULL, worker, param);
	}

	double sum = 0;
	for (i = 0; i < CPU; i++) {
		struct result *result;
		pthread_join(workers[i], (void **)&result);
		sum += result->sum;
		free(result);
	}

	printf("sum = %lf\n", sum);
	return 0;
}
