#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include "sender.h"
#include "list.h"

#define MAX_LEN 1024

// Externs for all the globals
extern int senderSD;
extern pthread_t senderThreadPID;
extern int REMOTE_PORT;
extern char *REMOTE_IP;
extern pthread_mutex_t inputLock;
extern pthread_cond_t inputCondSignaller;
extern struct sockaddr_in recvSocket;
extern bool KEYBOARD_SHUTDOWN;
extern char *msg;

void *senderThread(void* sharedList){

    // Decipher the host name to an IP or an IP to an IP
    struct hostent* hostName = gethostbyname(REMOTE_IP);
    struct sockaddr_in sinRemote;


    // Create a IP and apply it to sinRemote
    if (hostName->h_addrtype == AF_INET)
    {
        struct in_addr address_list = *(struct in_addr *)hostName->h_addr_list[0];

        printf("Remote IP connected to: %s\n", inet_ntoa(address_list));

        memset(&sinRemote, 0, sizeof(sinRemote));

        sinRemote.sin_family = AF_INET;
        sinRemote.sin_addr = address_list;
        sinRemote.sin_port = htons(REMOTE_PORT);
    }else{
        perror("Not IPv4\n");
        exit(EXIT_FAILURE);
    }
    
    //Create a socket
    senderSD = socket(AF_INET, SOCK_DGRAM, 0);
    unsigned int sin_len = sizeof(sinRemote);

    int err;
    while(1){

        pthread_mutex_lock(&inputLock);
        {
            //Wait for signal from keyboard before proceeding
            pthread_cond_wait(&inputCondSignaller, &inputLock);
            
            // Retrieve msg from the sharedList
            msg = (char *)List_remove(sharedList);
            
            
            //Send reply...
            err = sendto(senderSD, msg, strlen(msg), 0, 
                    (struct sockaddr*)&sinRemote, sin_len);
            if(err == -1){
                perror("Sendto was unsuccessful\n");
                exit(EXIT_FAILURE);
            }


            // Check to see if shutting down and send to other process that we are shutting down
            if(msg[0] == '!' && strlen(msg) == 2){
                printf("Sender shutting down \n");
                sendto(senderSD, msg, strlen(msg), 0, 
                    (struct sockaddr*)&recvSocket, sin_len);
                return NULL;
            }

        }
        pthread_mutex_unlock(&inputLock);   

    }

    return NULL;
        
}

// Initializer for thread
void Sender_init(List* sharedList){
    pthread_create(
        &senderThreadPID,    //PID by pointer
        NULL,           //Attributes
        senderThread,     //Function
        (void *) sharedList);   
}
