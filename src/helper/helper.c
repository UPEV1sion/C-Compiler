//
// Created by escha on 02.02.25.
//

#include "helper.h"

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

static int str_to_signed_num(const char *restrict str, long long *out, const ssize_t lower_bound, const ssize_t upper_bound)
{
    char *end_ptr;
    errno = 0;
    long long val = strtoll(str, &end_ptr, 10);

    if (errno == ERANGE || val > upper_bound || val < lower_bound)
        return -1;

    if (end_ptr == str || *end_ptr != 0)
        return -2;

    *out = val;

    return 0;
}

int atoi_s(const char *restrict str, int *out)
{
    return str_to_signed_num(str, (long long *) out, INT_MIN, INT_MAX);
}

int atol_s(const char *restrict str, long *out)
{
    return str_to_signed_num(str, (long long *) out, LONG_MIN, LONG_MAX);
}

int atoll_s(const char *restrict str, long long *out)
{
    return str_to_signed_num(str, out, LLONG_MIN, LLONG_MAX);
}

static int str_to_unsigned_num(const char *restrict str, unsigned long long *out, const size_t upper_bound)
{
    char *end_ptr;
    errno = 0;
    unsigned long long val = strtoull(str, &end_ptr, 10);

    if (errno == ERANGE || val > upper_bound)
        return -1;

    if (end_ptr == str || *end_ptr != 0)
        return -2;

    *out = val;

    return 0;
}

int atoui_s(const char *restrict str, unsigned int *out)
{
    return str_to_unsigned_num(str, (unsigned long long *) out, UINT_MAX);
}

int atoul_s(const char *restrict str, unsigned long *out)
{
    return str_to_unsigned_num(str, (unsigned long long *) out, ULONG_MAX);
}

int atoull_s(const char *restrict str, unsigned long long *out)
{
    return str_to_unsigned_num(str, out, ULLONG_MAX);
}

int atof_s(const char *restrict str, float *out)
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

int atod_s(const char *restrict str, double *out)
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

int atold_s(const char *restrict str, long double *out)
{
    char *end_ptr;
    errno = 0;
    long double val = strtold(str, &end_ptr);

    if (errno == ERANGE)
        return -1;

    if (end_ptr == str || *end_ptr != 0)
        return -2;

    *out = val;

    return 0;
}


