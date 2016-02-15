#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h>

#include "util.h"

main(){
char szLine[1024] = "Array";
char* szTarget = "Array";
printf("what's inside: %i\n", strcmp(szLine, szTarget));
return 0;
}
