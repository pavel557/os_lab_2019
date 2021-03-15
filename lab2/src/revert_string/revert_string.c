#include "revert_string.h"
#include <stdio.h>
#include <string.h>

void RevertString(char *str)
{
    char buff;
    int lenght = strlen(str);
    for (int i=0; i<lenght/2; i++)
    {
        buff = str[i];
        str[i] = str[lenght-i-1];
        str[lenght-i-1] = buff;
    }
    
}

