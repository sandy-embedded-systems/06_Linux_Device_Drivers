#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    int fd;
    int i;
    char devname[32];
    char msg[64];
    char buffer[512];
    int n;

    for (i = 0; i < 3; i++) {

        snprintf(devname,
                 sizeof(devname),
                 "/dev/mydev%d",
                 i);

        fd = open(devname, O_RDWR);

        if (fd < 0) {
            perror(devname);
            continue;
        }

        snprintf(msg,
                 sizeof(msg),
                 "Message for device %d",
                 i);

        write(fd, msg, strlen(msg));

        lseek(fd, 0, SEEK_SET);

        n = read(fd, buffer, sizeof(buffer) - 1);

        if (n > 0) {
            buffer[n] = '\0';

            printf("----- %s -----\n",
                   devname);

            printf("%s\n", buffer);
        }

        close(fd);
    }

    return 0;
}
