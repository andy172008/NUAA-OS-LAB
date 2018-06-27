#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 10

typedef struct {
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}sema_t;

void sema_init(sema_t *sema, int value)
{
	sema->value = value;
	pthread_mutex_init(&sema->mutex, NULL);
	pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema)
{
	pthread_mutex_lock(&sema->mutex);
	while (sema->value <= 0)
		pthread_cond_wait(&sema->cond, &sema->mutex);
	sema->value--;
	pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema)
{
	pthread_mutex_lock(&sema->mutex);
	++sema->value;
	pthread_cond_signal(&sema->cond);
	pthread_mutex_unlock(&sema->mutex);
}

sema_t mutex_sema[N];
int number;

void *run(void *arg){
	int *index;
	index = (int *)arg;
	sema_wait(&mutex_sema[(*index) % N]);
	//注意这里，是先加再除再加
	int next_index = (*index + 1) % N + 1;
	number++;

	printf("Thread T%d sends %d to T%d \n",(*index) + 1,number,next_index);
	sema_signal(&mutex_sema[(*index + 1)% N]);

	//如果是第一个，等着接收
	if(*index == 0)
	{
		sema_wait(&mutex_sema[(*index) % N]);
		printf("Thread T%d had accept %d from T%d\n",*index +1,number,N);

	}
		
	return NULL;
}

int main(){
	pthread_t tid[N];
	number = 0;
	int i;
	for (i = 0; i<N; i++) {
		sema_init(&mutex_sema[i], 0);
	}
	for(i = 0;i<N;i++){
		int *num = (int *)malloc(sizeof(int));
		*num = i;
		pthread_create(&tid[i],NULL,run,(void *)num);
	}
	//启动这个环
	sema_signal(&mutex_sema[0]);
	pthread_join(tid[0],NULL);

	return 0;
}
