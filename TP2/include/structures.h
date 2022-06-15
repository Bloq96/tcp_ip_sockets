#ifndef _GABRIEL_STRUCTURES_H_
#define _GABRIEL_STRUCTURES_H_

#define BUFFER_SIZE 32768

struct Buffer {
    char data[BUFFER_SIZE];
    int length;
};

#endif // _GABRIEL_STRUCTURES_H_
