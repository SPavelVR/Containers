
#include <stdlib.h>
#include <string.h>

#undef max

#include "../include/string_t.h"

String strvoid(size_t capacity) {
    String str = malloc(sizeof(__STRING_TYPE) * capacity);

    if (str == NULL) return NULL;

    memset(str, '\0', sizeof(__STRING_TYPE) * capacity);

    return str;
}

String strinit(String text) {
    size_t size = strlen(text);
    
    String str = malloc(sizeof(__STRING_TYPE) * (size + 1));

    if (str == NULL) return NULL;

    memcpy(str, text, sizeof(__STRING_TYPE) * (size + 1));

    return str;
};

String strninit(String text, size_t lim) {
    size_t size = strnlen(text, lim);
    
    String str = malloc(sizeof(__STRING_TYPE) * (size + 1));

    if (str == NULL) return NULL;

    memcpy(str, text, sizeof(__STRING_TYPE) * (size + 1));

    return str;
};

String strninitindex(String text, size_t index, size_t lim) {
    if (lim == 0) return NULL;

    size_t size = strnlen(text + index, lim);
    
    String str = malloc(sizeof(__STRING_TYPE) * (size + 1));
    memset(str, '\0', sizeof(__STRING_TYPE) * (size + 1));

    if (str == NULL) return NULL;

    memcpy(str, text + index, sizeof(__STRING_TYPE) * (size));

    return str;
};

void strfree(String str) {
    if (str == NULL) return ;
    free(str);
};

void strarrfree(String* arr) {
    if (arr == NULL) return ;

    String* start = arr;

    while (arr != NULL) {
        strfree(*arr);
        arr++;
    }

    free(start);
};

String strplus(String left, String right) {
    size_t l = strlen(left), r = strlen(right);
    size_t sum = l + r;

    String str = strvoid(sum + 1);

    if (str == NULL) return NULL;

    if (l > 0)
        memcpy(str, left, l * sizeof(__STRING_TYPE));

    if (r > 0)
        memcpy(str + l, right, (r + 1) * sizeof(__STRING_TYPE));

    return str;
};

String* strsplit(String dest, String source) {
    size_t count = strcount(dest, source);
    size_t size_sr = strlen(source);

    String* arr = calloc(count + 2, sizeof(String));

    size_t cutl = 0;
    size_t citr = 0;

    String start = source;
    String start_ds = dest;
    String* start_arr = arr;

    while (*dest != '\0') {

        if (*dest == *source) source++;
        else source = start;

        citr++;
        dest++;

        if (*source == '\0') {
            *arr = strninitindex(start_ds, cutl, citr - size_sr - cutl);
            if (arr != NULL)
                arr++;
            cutl = citr;
            source = start;
        }
    }

    if (cutl != citr) {
        *arr = strninitindex(start_ds, cutl, citr - cutl);
            if (arr != NULL)
                arr++;
    }

    *arr = NULL;
        
    return start_arr;
};

String strjoin(String face, String* source) {
    size_t size_fc = strlen(face);
    size_t count = 0;
    size_t size_arr = 0;

    String* start = source;

    
    while (*source != NULL) {
        size_arr += strlen(*source);
        count++;
        source++;
    }

    source = start;

    String bin = strvoid(1);
    
    String temp = *source;

    while (*source != NULL) {
        String t_bin = strplus(bin, temp);
        strfree(bin);
        bin = t_bin;

        if (temp == face) {
            temp = *source;
        } else {
            temp = face;
            source++;
        }
    };

    return bin;

};

char strfind(String dest, String source) {
    String start = source;

    while (*dest != '\0') {
        if (*dest == *source) source++;
        else source = start;

        if (*source == '\0') {
            return 1;
        }
        dest++;
    }

    return 0;
};

size_t strcount(String dest, String source) {

    size_t count = 0;

    String start = source;

    while (*dest != '\0') {
        if (*dest == *source) source++;
        else source = start;

        if (*source == '\0') {
            count++;
            source = start;
        }

        dest++;
    }

    return count;
};

int streql(String str1, String str2) {
    size_t l = strlen(str1);
    size_t r = strlen(str2);

    size_t m = (l >= r) ? r : l;

    int c = memcmp(str1, str2, m * sizeof(__STRING_TYPE));

    if (c == 0 && l == r) return 0;
    if (c == 0) {
        if (l < r) return 1;
        return -1;
    }
    return c;
};

String strreplace(String dest, String old, String new, int count) {
    if (dest == NULL || old == NULL || new == NULL) return NULL;

    size_t c = strcount(dest, old);
    
    if (c == 0) return strinit(dest);
    if (count == 0) count = (int) c;
    
    size_t osize = strlen(old);
    size_t nsize = strlen(new);
    size_t mx = (osize > nsize) ? osize : nsize;
    size_t mn = (osize > nsize) ? nsize : osize;
    size_t capacity = strlen(dest) + 1;
    size_t r = mx - mn;
    
    if (nsize > osize) {
        capacity += (nsize - osize) * ((count >= 0) ? count : (-count));
    }

    if (count + (int) c < 0) count = (int) ~(c - 1);

    String res = strvoid(capacity);
    String start_rs = res;
    String start_o = old;
    String start_n = new;

    while (*dest != '\0') {

        if (*dest == *old) {
            old++;
        } else {
            old = start_o;
        }

        *res = *dest;
        dest++;
        res++;

        if (*old == '\0' && (count > 0 || (count < 0 && count + (int) c == 0))) {
            res -= osize;

            while (*new != '\0') {
                *res = *new;
                res++;
                new++;
            }
            old = start_o;
            new = start_n;
            c--;
            if (count > 0)
                count--;
            if (count < 0)
                count++;
        } else if (*old == '\0') {
            c--;
        }
    }

    *res = *dest;
    return start_rs;

};

uint64_t fsize(String fname) {

    if (name == NULL) return 0;

    FILE* f = fopen(fname, "rb");
    
    if (f == NULL) return 0;

    uint64_t count = 0;

    while (fgetc(f) != EOF) count++;

    fclose(f);
    return count;
};

uint64_t fmaxline(String fname) {

    if (name == NULL) return 0;

    FILE* f = fopen(fname, "rb");
    
    if (f == NULL) return 0;

    uint64_t count = 0;
    uint64_t temp = 0;

    char c;
    while ((c = fgetc(f)) != EOF) {

        if (c != '\n' || c != '\0') temp++;
        else {
            count = (count > temp) ? count : temp;
            temp = 0;
        }
    }

    fclose(f);
    return count;
};

String freadall(String fname) {

    if (name == NULL) return 0;

    FILE* f = fopen(fname, "rb");
    
    if (f == NULL) return 0;

    String bin = strvoid(2);
    
    uint64_t ml = fmaxline(fname) + 2;

    String buff = strvoid(ml);

    while (fgets(buff, ml, f) != NULL) {
        String temp = strplus(bin, buff);
        strfree(bin);
        bin = temp;
    }

    strfree(buff);

    fclose(f);

    return bin;
    
};