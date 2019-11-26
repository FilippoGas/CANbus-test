//Basically equal to candump_test but instead of printing in stdout, this log in a file
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

int main(int argc, char const *argv[])
{
    

    int nbytes, s, i;
    
    struct sockaddr_can addr;
    struct can_frame frame;
    struct ifreq ifr;

    FILE *stream;

    time_t current_time;

    //check if parameters are correct
    if (argc != 2)  
    {
        printf("ERROR, usage : %s <CANbus interface>\n",argv[0]);
        exit(EXIT_FAILURE);
    }    

    //Generate file name as canlog-<date>
    char filename[sizeof("canlog-1111_11_11-11_11.txt")];
    char *prefix = "canlog-";
    char *filename_time;

    //get current time
    current_time = time(NULL);
    struct tm tm = *localtime(&current_time);
    printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1,tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("%d\n",current_time);

    //generate file name
    sprintf(filename,"canlog-%d_%d_%d-%d_%d.txt",tm.tm_year+1900,tm.tm_mon,tm.tm_mday,tm.tm_hour,tm.tm_min);
    printf("Filename: %s\n",filename);

    //Create the log file and open buffer
    stream = fopen(filename,"w");
    if(stream == NULL){
        perror("Error while opening the file, terminating...");
        exit(EXIT_FAILURE);
    }

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

        current_time = time(NULL);
        

        printf("ID: %0*X   DLC: [%d] data: ",3,frame.can_id,frame.can_dlc);
        fprintf(stream,"%ld ",current_time);
        fprintf(stream,"%0*X ",3,frame.can_id);
        fflush(stream);
        {

        for (i = 0; i< frame.can_dlc ; i++)  
            //printf("%0*X ",2,frame.data[i]); //stampando a terminale non stampa più su file....
            fprintf(stream," %0*X",2,frame.data[i]);
            fflush(stream);
            
        }
        printf("\n");
        fprintf(stream,"\n");
        fflush(stream);
        
    }
    fclose(stream);

    return 0;
}
