#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "parameterised_matching/m_match.h"

int main(int argc, char **argv) {
    FILE *f = fopen(argv[2], "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *pattern = malloc(fsize);
    fread(pattern, fsize, 1, f);
    fclose(f);

    clock_t start = clock();
    mmatch_state state = mmatch_build(pattern, fsize - 1);
    double build_time = (double)(clock() - start) / (CLOCKS_PER_SEC/1000);

    f = fopen(argv[1], "r");
    int i = 0;
    char T_i;
    int result;
    T_i = fgetc(f);
    start = clock();
    while (T_i != EOF) {
        result = mmatch_stream(state, T_i, i);
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

    mmatch_free(state);
    return 0;
}