#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <unistd.h>

// Function to generate a random MAC address
void generateRandomMacAddress(unsigned char *mac)
{
    for (int i = 0; i < 6; i++)
    {
        mac[i] = rand() % 256;
    }
}

// Function to change the MAC address of a network interface
int changeMacAddress(const char *device)
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_live(device, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL)
    {
        fprintf(stderr, "Error opening device: %s\n", errbuf);
        return -1;
    }

    unsigned char newMac[6];
    generateRandomMacAddress(newMac);

    struct pcap_if *dev;
    int ret = pcap_findalldevs(&dev, errbuf);
    if (ret == -1)
    {
        fprintf(stderr, "Error finding network devices: %s\n", errbuf);
        return -1;
    }

    while (dev != NULL)
    {
        if (strcmp(dev->name, device) == 0)
        {
            struct pcap_addr *addr = dev->addresses;
            while (addr != NULL)
            {
                if (addr->addr->sa_family == AF_INET)
                {
                    struct sockaddr_in *sin = (struct sockaddr_in *)addr->addr;
                    memcpy(sin->sin_addr.s_addr, newMac, 6);
                    break;
                }
                addr = addr->next;
            }
            break;
        }
        dev = dev->next;
    }

    pcap_close(handle);
    return 0;
}

int main()
{
    const char *device = "eth0"; // Replace with your network interface name

    while (1)
    {
        if (changeMacAddress(device) == 0)
        {
            printf("MAC address changed successfully\n");
        }
        else
        {
            printf("Failed to change MAC address\n");
        }
        sleep(120); // Sleep for 2 minutes
    }

    return 0;
}
