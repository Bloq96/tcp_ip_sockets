#include <stdio.h>

int compareStrings(char *string1, char *string2, int length) {
    for(int it=0;it<length;++it) {
        if((int)string1[it]<(int)string2[it]) {
            return -1;
        } else if((int)string1[it]>(int)string2[it]) {
            return 1;
        } else if(string1[it]=='\0') {
            break;
        }
    }
    return 0; 
}

int readLine(char *buffer, int max_length) {
    int it;
    for(it=0;it<max_length-1;++it) {
        if(scanf("%c",&buffer[it])==EOF) {
            buffer[it] = '\0';
            return it?it:-1;
        } else if(buffer[it]=='\n') {
            break;
        }
    }
    buffer[it] = '\0';
    return it;
}

int stringLength(char *string) {
    if(string==NULL) {
        return 0;
    }

    int length = 0;

    while(string[length]!='\0') {
        ++length;
    }

    return length;
}

int stringToInteger(char *string, int length) {
    if(string==NULL||length==0) {
        return 0;
    }

    int it = 0;
    int signal = 1;

    if(string[it]=='-') {
        signal *= -1;
        ++it; 
    } else if(string[it]=='+') {
        ++it;
    }

    int result = 0;
    int value;

    while(it<length) {
        value = (int) string[it];
        result *= 10;
        if(value>=48&&value<=57) {
            result += value - 48;
        } else {
            return 0;
        }
        ++it;
    }

    return signal*result;
}

int validIPv4(char *IPv4,int length) {
    int digit = 0;
    int digits = 0;
    int fields = 0;
    int value = 0;
    for(int it=0;it<length;++it) {
        if(IPv4[it]=='.') {
            if(digits==0) {
                return 0;
            } else if(value<0||value>255) {
                return 0;
            } else {
                ++fields;
                digits = 0;
                value = 0;
            }
        } else {
            value *= 10;
            digit = (int) IPv4[it];
            if(digit<48||digit>57) {
                return 0;
            } else {
                value += digit - 48;
                ++digits; 
            }
        }
    }
    if(digits==0) {
        return 0;
    } else if(value<0||value>255) {
        return 0;
    } else {
        return fields==3?1:0;
    }
}
