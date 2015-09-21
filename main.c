#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "timing.h"
#include "allocs.h"

static uint32_t xorshift_state[4] = {
    123456789U,
    362436069U,
    521288629U,
    88675123U
};

static void xorshift_reset(void)
{
    xorshift_state[0] = 123456789U;
    xorshift_state[1] = 362436069U;
    xorshift_state[2] = 521288629U;
    xorshift_state[3] = 88675123U;
}

static uint32_t xorshift(void)
{
    const uint32_t temp = xorshift_state[0] ^ (xorshift_state[0] << 11U);
    xorshift_state[0] = xorshift_state[1];
    xorshift_state[1] = xorshift_state[2];
    xorshift_state[2] = xorshift_state[3];
    return xorshift_state[3] ^= (temp >> 8U) ^ temp ^ (xorshift_state[3] >> 19U);
}

/* Inclusive pseudorandom number.
 * Assumes the range is less than 2**32.
*/
static size_t xorshift_range(const size_t minimum, const size_t maximum)
{
    if (maximum > minimum) {
        const uint32_t limit = (uint32_t)(maximum - minimum + (size_t)1);
        return minimum + (size_t)(xorshift() % limit);
    } else
        return minimum;
}

void run(const size_t index, const size_t rounds,
         void **const ptr, const size_t count,
         const size_t minsize, const size_t maxsize)
{
    size_t r, i;

    for (i = 0; i < count; i++)
        ptr[i] = alloc[index].func(xorshift_range(minsize,maxsize));

    for (r = 1; r < rounds; r++) {
        for (i = 0; i < count; i++) {
            free(ptr[i]);
            ptr[i] = alloc[index].func(xorshift_range(minsize,maxsize));
        }
    }

    for (i = 0; i < count; i++) {
        free(ptr[i]);
        ptr[i] = NULL;
    }
}

int main(int argc, char *argv[])
{
    unsigned long count, rounds, minsize, maxsize;
    unsigned long i, a;
    double  seconds, overhead, calls;
    void  **ptr;
    char   dummy;

    if (argc < 5 || argc > 6 || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
        fprintf(stderr, "\n");
        fprintf(stderr, "Usage: %s [ -h | --help ]\n", argv[0]);
        fprintf(stderr, "       %s FUNCTION COUNT ROUNDS SIZE\n", argv[0]);
        fprintf(stderr, "       %s FUNCTION COUNT ROUNDS MINSIZE MAXSIZE\n", argv[0]);
        fprintf(stderr, "\n");
        fprintf(stderr, "Memory allocaton functions available:\n");
        for (i = 1; alloc[i].name; i++)
            fprintf(stderr, "       %s\n", alloc[i].name);
        fprintf(stderr, "\n");
        return 0;
    }

    for (a = 0UL; alloc[a].name; a++)
        if (!strcmp(alloc[a].name, argv[1]))
            break;
    if (!alloc[a].name || !alloc[a].func) {
        fprintf(stderr, "%s: Not supported. Supported functions are:\n", argv[1]);
        for (i = 1; alloc[i].name; i++)
            fprintf(stderr, "       %s\n", alloc[i].name);
        fprintf(stderr, "\n");
        return 0;
    }

    if (sscanf(argv[2], " %lu %c", &count, &dummy) != 1 || count < 1UL) {
        fprintf(stderr, "%s: Count is too small.\n", argv[2]);
        return 1;
    }
    if (sscanf(argv[3], " %lu %c", &rounds, &dummy) != 1 || count < 1UL) {
        fprintf(stderr, "%s: Number of rounds is too small.\n", argv[3]);
        return 1;
    }        
    if (sscanf(argv[4], " %lu %c", &minsize, &dummy) != 1 || minsize < 1UL) {
        fprintf(stderr, "%s: Size is too small.\n", argv[4]);
        return 1;
    }
    if (argc > 5) {
        if (sscanf(argv[5], " %lu %c", &maxsize, &dummy) != 1 || maxsize < minsize) {
            fprintf(stderr, "%s: Maximum size is too small.\n", argv[5]);
            return 1;
        }
    } else
        maxsize = minsize;

    ptr = malloc((size_t)count * sizeof (*ptr));
    if (!ptr) {
        fprintf(stderr, "%s: Count is too large; out of memory.\n", argv[1]);
        return 1;
    }

    /* Overheads using alloc[0]. */

    xorshift_reset();
    start_timing();
    run(0, rounds, ptr, count, minsize, maxsize);
    stop_timing();

    calls = (double)rounds * (double)count;

    /* Only CPU time spent in userspace is measured. */
    overhead = user_cpu_seconds();

    if (maxsize > minsize)
        fprintf(stdout, "%16s: %lu to %lu bytes: ",
                alloc[a].name, (unsigned long)minsize, (unsigned long)maxsize);
    else
        fprintf(stdout, "%16s: %lu bytes: ",
                alloc[a].name, (unsigned long)minsize);
    fflush(stdout);

    /* Reset PRNG to a known state, to ensure allocation size sequence
     * matches from run to run. */

    xorshift_reset();
    start_timing();
    run(a, rounds, ptr, count, minsize, maxsize);
    stop_timing();

    seconds = user_cpu_seconds();

    fprintf(stdout, "%.3f ns per allocation (+ %.3f ns; %.3f s + %.3f s total)\n",
            1000000000.0 * (seconds - overhead) / calls,
            1000000000.0 * overhead / calls,
            seconds, overhead);

    fflush(stdout);

    return 0;
}
