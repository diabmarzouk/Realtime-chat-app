#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "keyboard.h"
#include "list.h"

#define MAX_LEN 1024

// Externs for all the globals
extern pthread_t keyboardThreadPID;
extern pthread_mutex_t inputLock;
extern pthread_cond_t inputCondSignaller;
extern bool SHUTDOWN;
extern bool KEYBOARD_SHUTDOWN;
extern bool RECEIVER_SHUTDOWN;
extern char *inputMsg;

void *KeyboardThread(void *sharedList){
    
    
    // Get input from stdin and put it into inputMsg, 
    while(SHUTDOWN == false){

        // Malloc inputMsg as MAX_LEN 
        inputMsg = (char *)malloc(MAX_LEN * sizeof(char));

        // Wait for input from keyboard
        if (fgets(inputMsg, MAX_LEN - 1, stdin) != NULL){

            // To help with what you sent vs what other process send
            printf("This is the message you are sending: %s\n", inputMsg);
            

            // Critical Section! -- lock the list and then send a signal to the sender thread
            pthread_mutex_lock(&inputLock);
            {
                List_add(sharedList, (char *)inputMsg);
                pthread_cond_signal(&inputCondSignaller);
            }
            pthread_mutex_unlock(&inputLock);
            
            // If the input was a "!", then begin shutdown of program.
            if(inputMsg[0] == '!' && strlen(inputMsg) == 2){
                SHUTDOWN = true;
                return NULL;
            }

        }else{
            perror("Unable to fgets() from keyboard...\n");
            exit(EXIT_FAILURE);
        }
    }
    
    return NULL;

}


// Initializer for thread
void Keyboard_init(List* sharedList){
    
    pthread_create(
        &keyboardThreadPID,    //PID by pointer
        NULL,           //Attributes
        KeyboardThread,     //Function
        (void *) sharedList);   
}
