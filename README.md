# Realtime-chat-app

## Disclaimer
This program was designed for UNIX based systems.

## About
This program is a real time chatting program where two computers can connect anywhere in the world given the IP's and ports, and the user can start typing between each terminal instance by connecting a UDP socket given the IP's and ports.

This program also was built to work with multithreading. The program will run on 4 threads, one for each sending text, receiving text, waiting for text from keyboard, and displaying text that was received. Screen.c and receiver.c work together as in they used the same linked list for sending data from receiving to the screen, and the same for Keyboard.c and sender.c where they would send data between each other. Furthermore, those two pairs share mutexes by locking and signalling each other when they are ready to access the shared linked list and then do one of the 4 actions.

## How to use
This program successfully works as a UDP chat bot. To use, type ./s-talk [Your port] [remote IP/pc name] [remote port], eg., ./s-talk 4444 127.0.0.1 3333

This program works for both an IP or pc name thats past in. While it theoretically would work on computers connecting across the world, there are limitations due to firewalls and ports needing to be opened for that to work.

## Testing/Running the program
I did testing of this program on two computers in the same network and it worked flawlessly.
I would recommend testing this program on one computer at a time. Just open two terminals/consoles and run the program as mentioned above with the self ip of 127.0.0.1 and use different ports for each instance.
This program prints out to show which message you sent/about to send and as well as which message was received. This way, its easier to test.
To shut down the program, type "!" in both processes and it will shut down.

## Makefile
To use the make file, for first time compilation, type 'make build', afterwards, you can just type 'make' as it uses a clean function to rid of .o files.
