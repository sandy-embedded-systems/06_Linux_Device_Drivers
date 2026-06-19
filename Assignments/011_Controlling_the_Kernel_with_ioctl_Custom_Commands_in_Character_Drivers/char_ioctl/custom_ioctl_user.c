#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define MAJOR_NUM 240

#define IOCTL_RESET_BUFFER _IO(MAJOR_NUM, 0)
#define IOCTL_GET_COUNT    _IOR(MAJOR_NUM, 1, int)
#define IOCTL_SET_DEBUG    _IOW(MAJOR_NUM, 2, int)

int main(void)
{
    int fd;
    int count;
    int dbg;

    fd = open("/dev/ioctldev", O_RDWR);

    if (fd < 0) {
        perror("open");
        return 1;
    }

    dbg = 1;
    ioctl(fd, IOCTL_SET_DEBUG, &dbg);

    write(fd, "Hello Driver", 12);

    ioctl(fd, IOCTL_GET_COUNT, &count);
    printf("Write count = %d\n", count);

    dbg = 0;
    ioctl(fd, IOCTL_SET_DEBUG, &dbg);

    write(fd, "Second Write", 12);

    ioctl(fd, IOCTL_RESET_BUFFER);

    close(fd);

    return 0;
}
