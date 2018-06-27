//@author 161520211 hxy
#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc,char* argv[])
{	
	if(argc == 3)
	{
		int fd_s,fd_d;
		//定义源文件标识符和目的文件标识符
		char temp[2];
		//定义缓冲区
		int count_r,count_w;


		fd_s = open(argv[1],O_RDONLY);
		if(fd_s == -1)
		{
			//此时源文件打开失败
			printf("error:源文件打开失败\n");
			exit(1);
		}
		else
		{
			fd_d = open(argv[2],O_CREAT|O_WRONLY|O_TRUNC,0777);
			//这里为了方便将权限设置为777，可修改
			if(fd_d == -1)
			{
				//此时目标文件打开失败
				printf("error:目标文件打开失败\n");
				exit(1);
			}
			else
			{
				while(1)
				{
					count_r = read(fd_s,temp,1);
					if(count_r == 0 || count_r == -1)
					{
						break;
					}				
					count_w = write(fd_d,temp,1);

				}
				close(fd_s);
				close(fd_d);
				return 0;
			}
		}
	}
	else
	{
		printf("error:输入的参数个数不对\n");
	}

}
