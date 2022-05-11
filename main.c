#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>

#include "screen.h"
#include "receiver.h"
#include "sender.h"
#include "list.h"
#include "keyboard.h"


// Instantiate many globals to be shared between the threads for singalling, and helping with shut down

// Globals for IP and Ports
int REMOTE_PORT = 0;
char *REMOTE_IP = "";
int LOCAL_PORT = 0;

// Globals for mutexes and signallers
pthread_mutex_t inputLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t outputLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t inputCondSignaller = PTHREAD_COND_INITIALIZER;
pthread_cond_t outputCondSignaller = PTHREAD_COND_INITIALIZER;

// Globals for shutting down
bool SHUTDOWN = false;
bool KEYBOARD_SHUTDOWN = false;
bool RECEIVER_SHUTDOWN = false;

// Globals for threads
pthread_t keyboardThreadPID;
pthread_t senderThreadPID;
pthread_t recvThreadPID;
pthread_t screenThreadPID;

// Globals for sockets and message strings
char *inputMsg;
int senderSD;
char *msg;
int recvSD;
char *messageRx;
char *outputMsg;


//Main function
int main(int argc, char *argv[]){

    // Set argv to global variables for the threads to use
    LOCAL_PORT = atol(argv[1]);
    REMOTE_IP = argv[2];
    REMOTE_PORT = atol(argv[3]);

    // Instantiate the two Lists
    List *inputList = List_create();
    List *outputList = List_create();

    // Initialize the threads
    Keyboard_init(inputList);
    Sender_init(inputList);
    Receiver_init(outputList);
    Screen_init(outputList);

    //Wait for "!" to be passed through by keyboard
    while(1){

        //Shutting down if equal to true
        if(SHUTDOWN == true){
            
            // Free all malloc'd strings
            free(msg);
            free(inputMsg);
            free(messageRx);
            free(outputMsg);

            //Shutdown the sockets for safe re-useability of ports
            shutdown(senderSD, SHUT_RD);
            shutdown(recvSD, SHUT_RD);
            return 0;
        }
    }

    return 0;
}