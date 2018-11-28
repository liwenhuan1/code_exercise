#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argn, char **argv)
{
    if (argn != 2) {
        printf("input param error!\n");
        return 0;
    }
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        printf("socket error!\n");
        return 0;
    }

    printf("net name: %s\n", argv[1]);

    struct ifreq ifr;
    strcpy(ifr.ifr_name, argv[1]);

    if (ioctl(fd, SIOCGIFHWADDR, &ifr)) {
        printf("not find the net! ioctl error\n");
        close(fd);
        return 0;
    } else
        printf("device mac: %02x:%02x:%02x:%02x:%02x:%02x\n",
            (unsigned char)ifr.ifr_hwaddr.sa_data[0],
            (unsigned char)ifr.ifr_hwaddr.sa_data[1],
            (unsigned char)ifr.ifr_hwaddr.sa_data[2],
            (unsigned char)ifr.ifr_hwaddr.sa_data[3],
            (unsigned char)ifr.ifr_hwaddr.sa_data[4],
            (unsigned char)ifr.ifr_hwaddr.sa_data[5]);

    ioctl(fd, SIOCGIFADDR, (char *)&ifr);
    printf("device ip: %s\n",
        (char *)inet_ntoa(((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr));

    ioctl(fd, SIOCGIFBRDADDR, &ifr);
    printf("device broadAddr: %s\n",
        (char *)inet_ntoa(((struct sockaddr_in *)&(ifr.ifr_broadaddr))->sin_addr));

    ioctl(fd, SIOCGIFNETMASK, &ifr);
    printf("device subnetMask: %s\n",
        (char *)inet_ntoa(((struct sockaddr_in *)&(ifr.ifr_netmask))->sin_addr));

    close(fd);
    return 0;
}
