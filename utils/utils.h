#ifndef UTILS_H
#define UTILS_H

typedef enum Exceptions
{
    AllocationError=1,
    KeyError,
    NotImplementedError,
    IndexError,
    OverflowError
} Exceptions;

int generate_damage_roll();

void exit_with_error(Exceptions ex);

#define len(arr) (sizeof(arr)/sizeof(arr[0]))

#endif