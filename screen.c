#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "screen.h"
#include "list.h"

#define MAX_LEN 1024

// Externs for all the globals
extern pthread_t screenThreadPID;
extern pthread_mutex_t outputLock;
extern pthread_cond_t outputCondSignaller;
extern bool RECEIVER_SHUTDOWN;
extern bool SHUTDOWN;
extern char *outputMsg;

void *ScreenThread(void *sharedList){

    while(SHUTDOWN == false){
        
        pthread_mutex_lock(&outputLock);
        {
            // Wait for signal from receiver
            pthread_cond_wait(&outputCondSignaller, &outputLock);
            
            outputMsg = (char *)List_remove(sharedList);

            // Shutdown checker
            if(outputMsg[0] == '!' && strlen(outputMsg) == 2){
                
                return NULL;
            }
            
            // This is so its easier to see what you sent vs what they sent
            fputs("Message received: ", stdout);    
            fputs(outputMsg, stdout);

            
        }
        pthread_mutex_unlock(&outputLock);
        
    }

    return NULL;
}

void Screen_init(List* sharedList){
    pthread_create(
        &screenThreadPID,    //PID by pointer
        NULL,           //Attributes
        ScreenThread,     //Function
        (void *) sharedList);   
}
