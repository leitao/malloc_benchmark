#ifndef   ALLOCS_H
#define   ALLOCS_H
#include <stdlib.h>

typedef struct {
    const char *const name;
    void *    (*const func)(const size_t size);
} alloc_info;

/* Array of known allocators.
 * First entry is a NULL allocator, for overhead measurement.
 * Array is terminated with name == func == NULL.
*/
extern const alloc_info alloc[];

/* Determine the power of two for use with allocations.
*/
#ifndef ALLOC_ALIGN
# if   __BIGGEST_ALIGNMENT__ <= 8
#  define  ALLOC_ALIGN  8
# elif __BIGGEST_ALIGNMENT__ <= 16
#  define  ALLOC_ALIGN 16
# elif __BIGGEST_ALIGNMENT__ <= 32
#  define  ALLOC_ALIGN 32
# elif __BIGGEST_ALIGNMENT__ <= 64
#  define  ALLOC_ALIGN 64
# elif __BIGGEST_ALIGNMENT__ <= 128
#  define  ALLOC_ALIGN 128
# elif __BIGGEST_ALIGNMENT__ <= 256
#  define  ALLOC_ALIGN 256
# else
#  error This arch has insane alignment restrictions.
# endif
#endif

#endif /* ALLOCS_H */
