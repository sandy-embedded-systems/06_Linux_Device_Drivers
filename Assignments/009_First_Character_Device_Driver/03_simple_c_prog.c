#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	char buffer[100];
	int fd = open("/dev/mychardev", O_RDWR);
	if(fd <0){
		perror("open");
		return 0;
	}
	write(fd, "Testing from app", 16);
	read(fd, buffer, 100);
	close(fd);
	return 0;
}


