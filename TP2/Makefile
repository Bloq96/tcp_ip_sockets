.PHONY: config compile link run clean

all: compile link

config:
	mkdir obj bin

clean:   
	rm obj/* bin/*

compile:
	gcc -g -Wall -c -Iinclude src/*
	mv *.o obj

link:
	gcc obj/functions.o obj/loopClient.o -o loopClient
	gcc obj/functions.o obj/loopServer.o -o loopServer
	mv loopClient loopServer -t bin
