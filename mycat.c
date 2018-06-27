//@author  161520211 hxy
#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc,char * argv[])
{
	int fd;
	char rs[2];
	int count;
	//只有文件名
	if(argc == 2)
	{
		fd = open(argv[1],O_RDONLY);
		if(fd == -1)
		{
			printf("error:路径不正确\n");
			exit(1);
		}
		while(1)
		{		
			count = read(fd,rs,1);
			if(count == 0 || count == -1)
			{
				break;
			}
			printf("%c",rs[0]);
		}		
		close(fd);
		return 0;
	}
	else
	{
		printf("error:输入的参数个数不对\n");
		exit(1);
	}
}
