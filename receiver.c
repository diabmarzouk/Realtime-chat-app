#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "receiver.h"
#include "list.h"

#define MAX_LEN 1024

// Externs for all the globals
extern pthread_t recvThreadPID;
extern int recvSD;
extern char *messageRx;
extern int LOCAL_PORT;
extern pthread_mutex_t outputLock;
extern pthread_cond_t outputCondSignaller;
extern bool SHUTDOWN;
extern bool KEYBOARD_SHUTDOWN;
extern bool RECEIVER_SHUTDOWN;

struct sockaddr_in recvSocket;

void *recvThread(void* sharedList){
    
    //Set up the socket...

    //Clear the data in the socket (s stands for socket and in stands for internet, sin)
    memset(&recvSocket, 0, sizeof(recvSocket));
    recvSocket.sin_family = AF_INET;   // Connect may be from anywhere
    recvSocket.sin_addr.s_addr = htonl(INADDR_ANY);
    recvSocket.sin_port = htons(LOCAL_PORT);

    //Create the socket
    recvSD = socket(AF_INET, SOCK_DGRAM, 0);
    
    //Bind the socket, which opens it to the outside world
    int bindCheck = bind(recvSD, (struct sockaddr*)&recvSocket, sizeof(recvSocket));
    
    // Error checking
    if(bindCheck < 0){
        perror("Binding failed\n");
        exit(EXIT_FAILURE);
    }

    if(recvSD < 0){
        perror("Socket failed to create\n");
        exit(EXIT_FAILURE);
    }


    while(1){

        //Receiving Data...
        struct sockaddr_in sinRemote;
        unsigned int sin_len = sizeof(sinRemote);
        memset(&sinRemote, 0, sin_len);

        //Clients data is written into messageRx
        messageRx = (char *)malloc(MAX_LEN * sizeof(char));

        // Checking for shutdown
        if(messageRx[0] == '!' && strlen(messageRx) == 2){
            SHUTDOWN = true;
            return NULL;
        }

        // Receiving from outside function
        ssize_t bytesRx = recvfrom(recvSD, messageRx, MAX_LEN, 0,
                                    (struct sockaddr*)&sinRemote, &sin_len);
        if(bytesRx == -1){
            printf("recvfrom() has failed.\n");
            return NULL;
        }


        //Critical Section -- Accessing the shared list, so lock it, add the data, then unlock and continue.
        pthread_mutex_lock(&outputLock);
        {
            List_add(sharedList, messageRx);
            //Send signal to screen thread that it can access the list now
            pthread_cond_signal(&outputCondSignaller);
        }
        pthread_mutex_unlock(&outputLock);

        
    }

    return NULL;
}

// Initializer for thread
void Receiver_init(List* sharedList){
    
    pthread_create(
        &recvThreadPID,    //PID by pointer
        NULL,           //Attributes
        recvThread,     //Function
        (void *) sharedList);   
}
