//@author 161520211 hxy
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<fcntl.h>
//备份标准输入输出
int true0,true1;
//两个管道,一个用于写，一个用于读，在进程更换时，也要对管道进行交换
int fdR[2],fdW[2];
//管道标识符，0代表没有使用管道
int pFlag = 0;
//指明运行管道时，当前命令所在的位置的类型
//0代表处于管道命令的开端，1代表中间，2代表结尾
int pType = 0;

int mysys(char * cmd)

{
	pid_t pid;


	//保留子进程的返回码
	int back;

	//如果cmd指向字符常量，那么strtok处理时会报错
	char myCmd[100];

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
		//检查是否是管道中的命令，并对标准输入输出与管道进行操作

//		printf("pFlag == %d\n",pFlag);		
		if(pFlag != 0)
		{
			
			if(pType == 0)
			{
				dup2(fdW[1],1);
				close(fdW[0]);
				close(fdW[1]);
			}	
			else if(pType == 1)
			{	
				dup2(fdR[0],0);
				dup2(fdW[1],1);
				close(fdW[0]);
				close(fdW[1]);
				close(fdR[0]);
				close(fdR[1]);

			}
			else if(pType == 2)
			{
				dup2(fdR[0],0);
				close(fdR[0]);
				close(fdR[1]);
			}
		}




		//先对字符串分割，在将其作为参数，执行execlp

		//记录命令参数的个数
		int opCount = 0;


		//储存分离后的参数
		char* aOp[20] = {NULL};
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
		//我们要检查是否有<, >,从而判断是否有文件重定向功能
		if(opCount - 2 >= 0 && strcmp(aOp[opCount - 2],"<") == 0 )
		{
//			printf("I come\n");
			int fd;
			//调整文件描述符	
			
			fd = open(aOp[opCount - 1],O_RDONLY);
		//	if(fd < 0)
		//	printf("errorrrrrrrrrrrr\n");
			dup2(fd,0);
		//	printf("adsfasdfasdf\n");
			//养成关闭fd的好习惯
			close(fd);
			
		//	int i;
		//	for(int i = 0;i <= opCount - 2;i++)
		//	{	
		//		printf("*%s* ",aOp[i]);
		//		
		//	}
		//	printf("\n");

			//切割出命令
			aOp[opCount - 2] = NULL;
					
		}
		else if(opCount - 2 >= 0 && strcmp(aOp[opCount - 2],">") == 0)
		{
			int fd;

			//调整文件描述符	
			fd = open(aOp[opCount - 1],O_RDWR|O_CREAT|O_TRUNC,0777);
			dup2(fd,1);
			//养成关闭fd的好习惯
			close(fd);
			//切割出命令
			aOp[opCount - 2] = NULL;

		}

		execvp(aOp[0],aOp);
		exit(1);

	}		
	else
	{
		//father
		if(pFlag != 0)
		{
			
			if(pType == 0)
			{
				
				close(fdW[1]);
			}	
			else if(pType == 1)
			{	
				
				close(fdW[1]);
				close(fdR[0]);
			}
			else if(pType == 2)
			{
				
				close(fdR[0]);
				close(fdR[1]);
			}
		}
		wait(&back);
	}
	return back;

}
int judgeIn(char * inPut)
{
	//此函数判断是否为内置命令,并根据不同情况采取不同的执行手段
	if(inPut == NULL)
	{

		//避免strcmp出现段错误
		return 0;

	}
	//将字符串开头的空格略去
	while(1)
	{
		if(*inPut == ' ')
		{	
			inPut++;
		}
		else
		{
			break;
		}
	}

	//strtok会破坏字符串，所以要先拷贝一份，用作strtok
	char aCopy[49];
	char * op = NULL;


	strcpy(aCopy,inPut);

	op = strtok(aCopy," ");
	if(op == NULL)
	{

		//避免strcmp出现段错误
		return 0;

	}
	if(!strcmp(op,"cd"))
	{
		char* nextAddr = NULL;
		nextAddr = strtok(NULL," ");
		chdir(nextAddr);

	}
	else if(!strcmp(op,"pwd"))
	{
		char addr[50] ;
		getcwd(addr,49);
		printf("%s\n",addr);
	}
	else if(!strcmp(op,"exit"))
	{
		printf("good bye\n");
		exit(0);

	}

	else
	{
		mysys(inPut);
	}
	return 0;

}
int main(int argc,char * argv[])
{

	//存放用户输入的数据
	char myInPut[49];

	//存放当前路径
	char myPath[49];
	int count;


	//先将标准输入输出做备份
	true0 = dup(0);
	true1 = dup(1);
	//某些进程要先读再写，所以要两个管道
	pipe(fdR);
	pipe(fdW);
	
	while(1)
	{
		//某些进程要先读再写，所以要两个管道
		pipe(fdR);
		pipe(fdW);

		getcwd(myPath,sizeof(myPath));
		printf("161520211@hxy:%s$",myPath);	
		fflush(stdout);
		count = read(0,myInPut,48);
		
//		write(true1,myInPut,49);
		myInPut[count-1] = 0;

		//检查文件是否使用了管道
		char tempIn[49];
		char* aOp[20] = {NULL};
		int opCount = 0;
		//管道标志位一定要置0
		pFlag = 0;
		pType = -1;

		strcpy(tempIn,myInPut);

		aOp[opCount] = strtok(tempIn,"|");
		opCount++;
		while(1)
		{
			aOp[opCount] = strtok(NULL,"|");
			if(aOp[opCount] == NULL)
			{
				break;
			}
			opCount++;
		}

		if(opCount > 1)
		{
			pFlag = 1;
		}
		else
		{
			pFlag = 0;
		}

		if(pFlag == 0)
		{

		//	printf("没有管道\n");
			//没有管道，直接执行
			judgeIn(myInPut);
		}
		else
		{

			for(int i = 0;i < opCount;i++ )
			{
								
				//对管道类型进行判断，调整pType的值
				
				if(i == 0)
				{
					pType = 0;
					
				}
				else if(i == opCount - 1)
				{
					pType = 2;
				}
				else
				{
					
					pType = 1;		
				}

				//当执行某条命令时，上一条命令的的写管道就应该是这一条命令的读管道，要对管道进行修改
				if(pType != 0)
				{
					dup2(fdW[0],fdR[0]);
					close(fdW[0]);
					close(fdW[1]);
					pipe(fdW);
				}
				
				judgeIn(aOp[i]);
			}
			
		}
		close(fdW[0]);
		close(fdW[1]);
		close(fdR[0]);
		close(fdR[1]);
	}
}

