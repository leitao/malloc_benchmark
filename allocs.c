#include <stdlib.h>
#ifdef __GLIBC__
#include <malloc.h>
#endif
#include "allocs.h"

void *alloc_none(const size_t const size __attribute__((unused)))
{
    return NULL;
}

void *alloc_malloc(const size_t const size)
{
    void *p = malloc(size);
    if (p) return p;
    exit(1);
}

#ifdef __GLIBC__
void *alloc_memalign(const size_t const size)
{
    void *p = memalign(ALLOC_ALIGN, size);
    if (p) return p;
    exit(1);
}
#endif

#if _POSIX_C_SOURCE >= 200112L || XOPEN_SOURCE >= 600
void *alloc_posix_memalign(const size_t const size)
{
    void *p;
    if (!posix_memalign(&p, ALLOC_ALIGN, size))
        return p;
    exit(1);
}
#endif

#ifdef _ISOC11_SOURCE
void *alloc_aligned_alloc(const size_t const size)
{
    if (size & (ALLOC_ALIGN))
        return aligned_alloc(ALLOC_ALIGN, 1 + (size | (ALLOC_ALIGN - 1)));
    else
        return aligned_alloc(ALLOC_ALIGN, size);
}
#endif

const alloc_info alloc[] = {
    { "Overhead", alloc_none },

    { "malloc", alloc_malloc },

#ifdef __GLIBC__
    { "memalign", alloc_memalign },
#endif

#if _POSIX_C_SOURCE >= 200112L || XOPEN_SOURCE >= 600
    { "posix_memalign", alloc_posix_memalign },
#endif

#ifdef _ISOC11_SOURCE
    { "aligned_alloc", alloc_aligned_alloc },
#endif

    { NULL, NULL }
};
