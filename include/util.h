#ifndef __UTIL_H
#define __UTIL_H

#include <stdio.h>
#include <string.h>

#define check_ptr(ptr__) \
if ((ptr__) == NULL) {  \
    printf("\nError in file %s, on line %d, NO MEMERY!!!\n", __FILE__, __LINE__),\
    exit(1);    \
}

#define swap_arg(arg1__, arg2__) {\
    if(sizeof(arg1__) == sizeof(arg2__)) {\
        char tmp[sizeof(arg1__)]; \
        memcpy(tmp, &(arg1__), sizeof(arg1__)); \
        memcpy(&(arg1__), &(arg2__), sizeof(arg1__)); \
        memcpy(&(arg2__), tmp, sizeof(arg2__));  \
    }\
}

#define AssertExit(__comparison)    \
if (!(__comparison)) {   \
    printf("\nAssert [%s/%d]: not true comparison\n%s\n", __FILE__, __LINE__, #__comparison);   \
    exit(1);    \
}



#define exit_error(massage__) \
do {    \
    printf("\nError in file %s, on line %d, massage: %s;\n", __FILE__, __LINE__, massage__),\
    exit(1);    \
}   while(0);

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#endif