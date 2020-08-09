#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include<string.h>

int main()
{
	char *buf;
	char read_buf[100];
	char write_buf[100];
	int fd = 0;
	int ret = 0;
	int state = 0;
	fd = open("/dev/led0",O_RDWR);
	while(1)
	{
		state = !state; 
		write(fd,&state,sizeof(state));
		sleep(1);
	}
	memset(read_buf,0,strlen(read_buf));
	memset(write_buf,0,strlen(write_buf));
	//memset(buf,0,sizeof(buf));
	//ret = read(fd,buf,sizeof(buf));
	//printf("user:%d\n",ret);
	//printf("usrdata :%s\n",buf);
	buf = "1234567";
	memcpy(read_buf, buf, strlen(buf)+1);
	write(fd,read_buf,sizeof(read_buf));

	close(fd);
	
	exit(0);
}
