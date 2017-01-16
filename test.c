#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>

#include "vformat.h"

int main(int argc, char **argv) {
    char *cmd;
    int len;

    len = vedisFormatCommand(&cmd,"SET %b bar" , "foo", 3);
    printf("%s\n", cmd);

    free(cmd);
    return 0;
}
