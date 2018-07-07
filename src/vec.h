#ifndef VEC_H
#define VEC_H

#include <stdlib.h>

/* Default expansion profile: passive */
#ifndef VEC_DEFAULT_EXPANSION_PROFILE
#define VEC_DEFAULT_EXPANSION_PROFILE vec_passive_expansion_profile
#endif

/* Minimum vector capacity */
#ifndef VEC_DEFAULT_MIN_COUNT
#define VEC_DEFAULT_MIN_COUNT 2
#endif

/* Useful construct, ex: vec_new(int); */
#define vec_new(_type_) (vec_empty(sizeof(_type_)))

/*
 * Aggressive expansion profile: cap(old_cap) = old_cap * 2
 */
size_t vec_aggressive_expansion_profile(size_t prev_item_count);

/*
 * Passive expansion profile: cap(old_cap) = old_cap + old_cap / 2
 */
size_t vec_passive_expansion_profile(size_t prev_item_count);

/*
 * Sets expansion profile on vector
 * 
 * vec:
 *  the vector
 * expansion_profile:
 *  the expansion profile callback
 */
void vec_set_expansion_profile(void *vec, size_t (*expansion_profile)(size_t));

/*
 * Creates new vector with `item_count` items of `item_size` bytes each
 * 
 * Obs:
 *  Capacity will be the same as `item_count`, hence, `item_count` cannot
 *  be less than VEC_DEFAULT_MIN_COUNT
 */
void *vec_create(size_t item_count, size_t item_size);

/*
 * Creates new vector with 0 items of `item_size` bytes
 * 
 * Obs:
 *  Capacity will be VEC_DEFAULT_MIN_COUNT
 */
void *vec_empty(size_t item_size);

/*
 * Gets number of items in the vector
 */
size_t vec_count(void *vec);

/*
 * Resizes this vector. New vector count will be equal to `item_count` as well
 * as the vector's capacity. This will effectively erase data if `item_count`
 * is lower than the current vec_count().
 * 
 * Obs:
 *  Capacity will be at least VEC_DEFAULT_MIN_COUNT
 */
void *vec_resize(void *vec, size_t item_count);

/*
 * Adds `n` slots to the end of the vector, allocating heap if needed.
 */
void *vec_add_n(void *vec, size_t n);

/*
 * Equivalent to vec_add_n(vec, 1);
 */
void *vec_add(void *vec);

/*
 * Shifts all elements from index i + 1 (if any) to the right
 * and returns pointer to liberated slot in i.
 * 
 * Important:
 *  This function assumes vec_count() is enough to fit a new
 *  item.
 * 
 * Example:
 *  int *ivec = vec_new(int);
 *  int *inspoint;
 *  ivec = vec_add_n(ivec, 3);
 *  inspoint = vec_insert(ivec, 1);
 *  *inspoint = 42;
 */
void *vec_insert(void *vec, size_t i);

/*
 * Remove item on `idx` position of `vec`
 */
int vec_remove(void *vec, size_t idx);

/*
 * Shrink vector capacity so that it will be equal to vec_count()
 */
void *vec_shrink(void *vec);

/*
 * Get pointer to last available slot.
 * 
 * Obs:
 *  Returns NULL if vec_count() == 0
 */
void *vec_last(void *vec);

/*
 * Destroy the vector
 */
void vec_destroy(void *vec);

#endif /* VEC_H */