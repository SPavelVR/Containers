#ifndef __STRING_T_H
#define __STRING_T_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __STRING_TYPE
#define __STRING_TYPE char
#endif

typedef char* String;

#include <stddef.h>
#include <string.h>


// ОБЪЯВЛЕНИЯ ФУНКЦИЙ

String strinit(String text);
String strninit(String text, size_t size);
String strninitindex(String text, size_t index, size_t size);

void strfree(String str);
void strarrfree(String* arr);

String strplus(String left, String right);

String* strsplit(String dest, String source);

String strjoin(String face, String* source);

char strfind(String find, String source);

size_t strcount(String dest, String source);

int streql(String str1, String str2);

String strreplace(String dest, String old, String new, int count);


#ifdef __cplusplus
}
#endif

#endif