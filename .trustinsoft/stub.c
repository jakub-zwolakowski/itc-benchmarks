#include <unistd.h>
#include <stdio.h>

unsigned int sleep(unsigned int seconds)
{
    printf("CALL sleep(%u)\n", seconds);
    return 0;
}
