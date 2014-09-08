#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

// Assumes 0 <= range <= RAND_MAX
// Returns in the half-open interval [0, max]
int random_at_most(long max) {
    unsigned long seed;
    size_t seed_len = 0;
    int f = open("/dev/urandom", O_RDONLY);
    while (seed_len < sizeof seed) {
        size_t result = read(f, ((char*)&seed) + seed_len, (sizeof seed) - seed_len);
        seed_len += result;
    }
    close(f);
    srand(seed);
    unsigned long
        // max <= RAND_MAX < ULONG_MAX, so this is okay.
        num_bins = (unsigned long) max + 1,
        num_rand = (unsigned long) RAND_MAX + 1,
        bin_size = num_rand / num_bins,
        defect   = num_rand % bin_size;

    long x;
    // This is carefully written not to overflow
    while (num_rand - defect <= (unsigned long)(x = random()));

    // Truncated division is intentional
    return (int)x/bin_size;
}

int main(int argc, char **argv) {
    FILE *f = fopen(argv[1], "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    long psize = strtol(argv[2], (char **)NULL, 10);
    fseek(f, random_at_most(fsize - psize), SEEK_SET);
    char *pattern = malloc(psize + 1);
    fread(pattern, psize, 1, f);
    fclose(f);
    printf("%s\n", pattern);
    return 0;
}