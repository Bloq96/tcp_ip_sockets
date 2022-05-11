.PHONY: compile link run clean

all: compile link

clean:   
	rm obj/* bin/*

compile:
	gcc -g -Wall -c -Iinclude lib/*
	mv *.o obj

link:
	gcc obj/functions.o obj/sendTo.o -o sendTo
	gcc obj/functions.o obj/connectTo.o -o connectTo
	gcc obj/functions.o obj/connectTo2.o -o connectTo2
	gcc obj/messageServer.o -o messageServer
	gcc obj/streamServer.o -o streamServer
	gcc obj/streamServer2.o -o streamServer2
	mv sendTo connectTo connectTo2 messageServer streamServer streamServer2 -t bin
