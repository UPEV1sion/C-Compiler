//
// Created by escha on 02.02.25.
//

#include "helper.h"

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

int atoi_s(const char *restrict str, int *out)
{
    char *end_ptr;
    errno = 0;
    long val = strtol(str, &end_ptr, 10);

    if (errno == ERANGE || val > INT_MAX || val < INT_MIN)
        return -1;

    if (end_ptr == str || *end_ptr != 0)
        return -2;

    *out = (int) val;

    return 0;
}

static int str_to_float(const char *restrict str, float *out)
{
    char *end_ptr;
    errno = 0;
    float val = strtof(str, &end_ptr);

    if (errno == ERANGE)
        return -1;

    if (end_ptr == str || *end_ptr != 0)
        return -2;

    *out = val;

    return 0;
}


static int str_to_double(const char *restrict str, double *out)
{
    char *end_ptr;
    errno = 0;
    double val = strtod(str, &end_ptr);

    if (errno == ERANGE)
        return -1;

    if (end_ptr == str || *end_ptr != 0)
        return -2;

    *out = val;

    return 0;
}

int atof_s(const char *restrict str, void *out)
{
    return strstr(str, "%d") ? str_to_double(str, out) : str_to_float(str, out);
}


