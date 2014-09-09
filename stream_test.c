#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef PARAMETERISED
#include "parameterised_matching/m_match.h"
#elif defined(KMP_TEST)
#include "parameterised_matching/kmp.h"
#else
#include "exact_matching/exact_matching.h"
#endif

int main(int argc, char **argv) {
    FILE *f = fopen(argv[2], "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *pattern = malloc(fsize);
    fread(pattern, fsize, 1, f);
    fclose(f);

    f = fopen(argv[1], "r");
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);

    clock_t start = clock();

    #ifdef PARAMETERISED
    mmatch_state state = mmatch_build(pattern, fsize - 1);
    #elif defined(KMP_TEST)
    kmp_state state = kmp_build(pattern, fsize - 1);
    #else
    char *sigma = malloc(256 * sizeof(char));
    int cnt;
    for (cnt = 0; cnt < 256; cnt++) sigma[cnt] = cnt - 128;
    exactmatch_state state = exactmatch_build(pattern, fsize - 1, sigma, 256, n, 0);
    #endif
    double build_time = (double)(clock() - start) / (CLOCKS_PER_SEC/1000);

    int i = 0;
    char T_i;
    int result;
    T_i = fgetc(f);
    start = clock();
    while (T_i != EOF) {
        #ifdef PARAMETERISED
        result = mmatch_stream(state, T_i, i);
        #elif defined(KMP_TEST)
        result = kmp_stream(state, T_i, i);
        #else
        result = exactmatch_stream(&state, T_i);
        #endif
        if (result != -1) printf("%d, ", result);
        i++;
        T_i = fgetc(f);
    }
    double time = (double)(clock() - start) / (CLOCKS_PER_SEC/1000000);
    fclose(f);
    printf("\nBuild Time: %f\nAverage Time per Character: %f\nTotal Time: %f\n", build_time, time/i, time);
    FILE *process_file = fopen("/proc/self/statm", "r");
    unsigned long size, resident, share, text, lib, data, dt;
    if (fscanf(process_file, "%lu %lu %lu %lu %lu %lu %lu", &size, &resident, &share, &text, &lib, &data, &dt) == 7) {
        printf("Size: %lu\nData: %lu\n", size, data);
    }
    fclose(process_file);

    #ifdef PARAMETERISED
    mmatch_free(state);
    #elif defined(KMP_TEST)
    kmp_free(state);
    #else
    exactmatch_free(&state);
    #endif
    return 0;
}