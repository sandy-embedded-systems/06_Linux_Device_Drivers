#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
#include <sys/ioctl.h>

#define DEVICE "/dev/polldev"

/* Same definitions as driver */
#define POLL_IOC_MAGIC          'P'
#define IOCTL_SET_READ_READY    _IO(POLL_IOC_MAGIC, 1)
#define IOCTL_CLR_READ_READY    _IO(POLL_IOC_MAGIC, 2)
#define IOCTL_SET_CAN_WRITE     _IO(POLL_IOC_MAGIC, 3)
#define IOCTL_CLR_CAN_WRITE     _IO(POLL_IOC_MAGIC, 4)

static void show_events(short revents)
{
    printf("revents = 0x%x\n", revents);

    if (revents & POLLIN)
        printf("  POLLIN\n");

    if (revents & POLLOUT)
        printf("  POLLOUT\n");

    if (revents & POLLERR)
        printf("  POLLERR\n");

    if (revents & POLLHUP)
        printf("  POLLHUP\n");
}

int main(void)
{
    int fd;
    struct pollfd pfd;
    char buf[256];

    fd = open(DEVICE, O_RDWR);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    pfd.fd = fd;
    pfd.events = POLLIN | POLLOUT;

    printf("\n=== Test 1 : Initial poll ===\n");

    poll(&pfd, 1, 0);
    show_events(pfd.revents);

    printf("\n=== Test 2 : Disable writing ===\n");

    ioctl(fd, IOCTL_CLR_CAN_WRITE);

    pfd.revents = 0;
    poll(&pfd, 1, 0);
    show_events(pfd.revents);

    printf("\n=== Test 3 : Enable writing ===\n");

    ioctl(fd, IOCTL_SET_CAN_WRITE);

    pfd.revents = 0;
    poll(&pfd, 1, 0);
    show_events(pfd.revents);

    printf("\n=== Test 4 : Force READ_READY using ioctl ===\n");

    ioctl(fd, IOCTL_SET_READ_READY);

    pfd.revents = 0;
    poll(&pfd, 1, 0);
    show_events(pfd.revents);

    printf("\n=== Test 5 : Read data ===\n");

    memset(buf, 0, sizeof(buf));

    read(fd, buf, sizeof(buf));

    printf("Read returned: \"%s\"\n", buf);

    pfd.revents = 0;
    poll(&pfd, 1, 0);
    show_events(pfd.revents);

    printf("\n=== Test 6 : Write data ===\n");

    write(fd, "Hello Driver", 12);

    pfd.revents = 0;
    poll(&pfd, 1, 0);
    show_events(pfd.revents);

    memset(buf, 0, sizeof(buf));
    read(fd, buf, sizeof(buf));

    printf("Read after write: \"%s\"\n", buf);

    close(fd);

    return 0;
}
