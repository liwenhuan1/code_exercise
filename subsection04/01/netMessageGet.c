#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>   //#include <unistd.h>
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

    struct ifreq buf[16];
    struct ifconf ifc;
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t)buf;
    if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc)) {
        int ifNum = ifc.ifc_len / sizeof(struct ifreq);
 //       printf("interface Num = %d\n", ifNum);
        while (ifNum -- > 0) {
            if (strcmp(argv[1], buf[ifNum].ifr_name) != 0)
                continue;
            printf("net name: %s\n", buf[ifNum].ifr_name);
            struct ifreq ifrCopy = buf[ifNum];
            if (ioctl(fd, SIOCGIFFLAGS, &ifrCopy))
                printf("ioctl: SIOCGIFFLAGS error!\n");

            //MAC
            if (!ioctl(fd, SIOCGIFHWADDR, (char *)(&buf[ifNum]))) {
                char mac[16];
                memset(mac, 0, sizeof(mac));
                snprintf(mac, sizeof(mac), "%02x%02x%02x%02x%02x%02x",
                    (unsigned char)buf[ifNum].ifr_hwaddr.sa_data[0],
                    (unsigned char)buf[ifNum].ifr_hwaddr.sa_data[1],
                    (unsigned char)buf[ifNum].ifr_hwaddr.sa_data[2],
                    (unsigned char)buf[ifNum].ifr_hwaddr.sa_data[3],
                    (unsigned char)buf[ifNum].ifr_hwaddr.sa_data[4],
                    (unsigned char)buf[ifNum].ifr_hwaddr.sa_data[5]);
                printf("device mac: %s\n", mac);
            } else
                printf("ioctl: SIOCGIFHWADDR error!\n");

            //IP
            if (!ioctl(fd, SIOCGIFADDR, (char *)&buf[ifNum])) {
                char ip[32] = {0};
                snprintf(ip, sizeof(ip), "%s",
                    (char *)inet_ntoa(((struct sockaddr_in *)&(buf[ifNum].ifr_addr))->sin_addr));
                printf("device ip: %s\n", ip);
            } else
                printf("ioctl: SIOCGIFADDR error!\n");

            //BoardAddr
            if (!ioctl(fd, SIOCGIFBRDADDR, &buf[ifNum])) {
                char broadAddr[32] = {0};
                snprintf(broadAddr, sizeof(broadAddr), "%s",
                    (char *)inet_ntoa(((struct sockaddr_in *)&(buf[ifNum].ifr_broadaddr))->sin_addr));
                printf("device broadAddr: %s\n", broadAddr);
            } else
                printf("ioctl: SIOCGIFBRDADDR error!\n");

            //SubnetMask
            if (!ioctl(fd, SIOCGIFNETMASK, &buf[ifNum])) {
                char subnetMask[32] = {0};
                snprintf(subnetMask, sizeof(subnetMask), "%s",
                    (char *)inet_ntoa(((struct sockaddr_in *)&(buf[ifNum].ifr_netmask))->sin_addr));
                printf("device subnetMask: %s\n", subnetMask);
            } else
                printf("ioctl: SIOCGIFNETMASK error!\n");

            close(fd);
            return 0;
        }
    }

    printf("%s not find\n", argv[1]);
    close(fd);
    return 0;
}
