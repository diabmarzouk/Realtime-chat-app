# Realtime-chat-app

## Disclaimer
This app was designed for UNIX based systems.

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
