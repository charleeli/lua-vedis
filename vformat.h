#ifndef VFORMAT_H
#define VFORMAT_H

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#include <stdarg.h>

size_t bulklen(size_t len);
int intlen(int i);
int vedisvFormatCommand(char **target, const char *format, va_list ap);
int vedisFormatCommand(char **target, const char *format, ...);

#endif
