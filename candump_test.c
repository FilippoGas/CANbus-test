#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <mosquitto.h>

#include <linux/can.h>
#include <linux/can/raw.h>

int main(int argc, char const *argv[])
{
    

    int nbytes, s, i;
    
    struct sockaddr_can addr;
    struct can_frame frame;
    struct ifreq ifr;

    

    

    //get interface name from command line
    const char *ifname = argv[1];

    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Error while opening socket\n");
        return -1;
    }
    
    strcpy(ifr.ifr_name, ifname);
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if ((bind(s, (struct sockaddr *)&addr, sizeof(addr))) < 0)
    {
        perror("Error in socket bind\n");
        return -2;
    }

    
    
    
    while (1)
    {
        
        nbytes = read(s, &frame,16);

        

        printf("ID: %0*X   DLC: [%d] data: ",3,frame.can_id,frame.can_dlc);
        {

        for (i = 0; i< frame.can_dlc ; i++)  
            printf("%0*X ",2,frame.data[i]);

            
        }
        printf("\n");
        
    }


    return 0;
}
