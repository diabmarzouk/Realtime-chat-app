
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE_200112L #-Werror

all: clean build

build: main.o receiver.o sender.o keyboard.o screen.o
	gcc $(CFLAGS) list.o main.o receiver.o sender.o keyboard.o screen.o -lpthread -o s-talk

main.o:
	gcc $(CFLAGS) -c main.c

receiver.o:
	gcc $(CFLAGS) -c receiver.c

sender.o:
	gcc $(CFLAGS) -c sender.c

keyboard.o:
	gcc $(CFLAGS) -c keyboard.c

screen.o:
	gcc $(CFLAGS) -c screen.c


clean:
	rm sender.o receiver.o main.o keyboard.o screen.o s-talk