//
// Created by escha on 02.02.25.
//

#pragma once

int atoi_s(const char *restrict str, int *out);
int atol_s(const char *restrict str, long *out);
int atoll_s(const char *restrict str, long long *out);
int atoui_s(const char *restrict str, unsigned int *out);
int atoul_s(const char *restrict str, unsigned long *out);
int atoull_s(const char *restrict str, unsigned long long *out);
int atof_s(const char *restrict str, float *out);
int atod_s(const char *restrict str, double *out);
int atold_s(const char *restrict str, long double *out);
