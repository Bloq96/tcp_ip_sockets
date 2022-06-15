#ifndef _GABRIEL_FUNCTIONS_H_
#define _GABRIEL_FUNCTIONS_H_

int charFill(char *array, int positions, char character);

int compareStrings(char *string1, char *string2, int length);

int readLine(char *buffer, int max_length);

int stringLength(char *string);
    
int stringToInteger(char *string, int length);

int validIPv4(char *IPv4, int length);

int zeroFill(char *array, int positions);

#endif // _GABRIEL_FUNCTIONS_H_
