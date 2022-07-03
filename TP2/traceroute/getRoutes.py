#!/bin/python3
import subprocess as subp
import sys
import time

def getRoutes(destination, output):
    for it in range(10):
        output.write("Attempt " +str(it+1)+": "+
        time.ctime(time.time())+"\n\n");
        output.write(subp.Popen(["traceroute", "-T", "-4",
        destination], stdout=subp.PIPE, text=True).stdout.read());
        output.write("-"*100+"\n");

def main():
    destinations = sys.argv[1:];
    it = 0;
    with open("output.txt", "w") as outputFile:
        outputFile.write("-"*100+"\n");
        outputFile.write("-"*100+"\n\n");
        for destination in destinations:
            it = it + 1;
            outputFile.write("Destination: "+str(it)+": "+
            destination+"\n\n");
            outputFile.write("-"*100+"\n");
            getRoutes(destination, outputFile);
            outputFile.write("-"*100+"\n\n");

main();
