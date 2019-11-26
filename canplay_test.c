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
    int i,s,nbytes;
    struct sockaddr_can addr;
    struct can_frame frame;
    struct ifreq ifr;

    FILE *stream;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char *ptr;
    time_t timestamp;
    time_t timestamp_prev=0;
    double deltaT;
    char *interface;
    char *id;


    
    //check if parameters are correct
    if (argc != 3)  
    {
        printf("ERROR, usage : %s <CANbus interface> <FILE>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    
    //Get interface from command line
    const char *ifname = argv[1];

    if ((s = socket(PF_CAN,SOCK_RAW,CAN_RAW)) < 0)
    {
        perror("Error while opening socket\n");
        return -1;
    }

    strcpy(ifr.ifr_name, ifname);
    ioctl(s,SIOCGIFINDEX,&ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if ((bind(s,(struct sockaddr *)&addr,sizeof(addr))) < 0)
    {
        perror("Error in socket bind\n");
        return -2;
    }
    //CANbus is now ready
    

    //open FILE stream
    stream = fopen(argv[2],"r");
    if (stream == NULL)
    {
        perror("couldn't open the file stream\n");
        exit(EXIT_FAILURE);
    }
    
    while ((nread = getline(&line,&len,stream)) != -1)
    {

        //parse line, get timestamp,interface,id and payload, pute them into frame
        ptr = strtok(line," ");
        timestamp = (time_t)atoi(ptr);                  //read timestamp in epoch format into string ptr, convert it to int and cast to 
        printf("timestamp: %ld  ",(long)timestamp);     //time_t to be able to work it with time.h library
        ptr = strtok(NULL," ");
        id = ptr;

        i=0;
        
        ptr = strtok(NULL," ");
        while(strlen(ptr)==2 || strlen(ptr)==3){    
            printf("i: %d ptr: %s ",i,ptr);
            frame.data[i] = strtol(ptr,NULL,16);
            i++;       
            if(strlen(ptr)==3){ 
                break;
            }else{
                ptr = strtok(NULL," ");
            }
        }
        printf("\n");
        frame.can_id = strtol(id,NULL,16);

        frame.can_dlc = i;
        
        //calcolo temporizzazione
        if(timestamp_prev == 0){
            deltaT = 0;
            timestamp_prev = timestamp;
        }else{
            deltaT = difftime(timestamp,timestamp_prev);
            timestamp_prev = timestamp;
        }

        printf("Waited %f ms\n",deltaT*1000);
        sleep((unsigned int)deltaT);
        
        //Write into CANinterface;
        if( (nbytes = write(s,&frame,sizeof(struct can_frame))) < 0 ){
            perror("Error in sending CANframe");
        }
        
    }
    
    free(line);
    fclose(stream);

    return 0;
}