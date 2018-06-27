//@author 161520211 hxy
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
int mysys(char * cmd)
{
	pid_t pid;
	int back;
	//保留子进程的返回码
	char myCmd[100];
	//如果cmd指向字符常量，那么strtok处理时会报错


	if(cmd == NULL)
	{
		return 1;
	}

	if(strlen(cmd) > 99)
	{
		printf("cmd len error\n");
		exit(1);
	}
	strcpy(myCmd,cmd);

	pid = fork();
	if(pid < 0)
	{
		printf("error:进程创建时出现错误\n");	
		exit(1);
	}

	if(pid == 0)
	{
		//child
		//先对字符串分割，在将其作为参数，执行execlp
		int opCount = 0;
		//记录命令参数的个数

		char* aOp[10] = {NULL};
		//储存分离后的参数
		aOp[opCount] = strtok(myCmd," ");
		opCount++;
		while(1)
		{
			aOp[opCount] = strtok(NULL," ");
			if(aOp[opCount] == NULL)
			{
				break;
			}
			opCount++;
		}
		execvp(aOp[0],aOp);

	}		
	else
	{
		//father
		wait(&back);
	}
	return back;

}
int main(int argc,char * argv[])
{
	//	printf("--------------------------------------------------\n");
	
	mysys("echo 161520211 hxy");
	//	printf("--------------------------------------------------\n");
	mysys("ls test.c -l");
	//	printf("--------------------------------------------------\n");
	return 0;
}

