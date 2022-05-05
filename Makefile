.PHONY: compile link run clean

all: compile link

clean:   
	rm obj/* bin/*

compile:
	gcc -g -Wall -c -Iinclude lib/*
	mv *.o obj

link:
	gcc obj/functions.o obj/connectTo.o -o connectTo
	gcc obj/messageServer.o -o messageServer
	mv connectTo messageServer -t bin

run:
	bin/connectTo
