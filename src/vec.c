#include "vec.h"
#include <string.h>

#define VEC_CAP(cap) ((cap < VEC_DEFAULT_MIN_COUNT) ? VEC_DEFAULT_MIN_COUNT : cap)

typedef struct vec_meta
{
    size_t item_count;
    size_t item_size;
    size_t capacity;
    size_t (*expansion_profile)(size_t);
} vec_meta_s;

size_t vec_aggressive_expansion_profile(size_t prev_item_count)
{
    return prev_item_count * 2;
}

size_t vec_passive_expansion_profile(size_t prev_item_count)
{
    return prev_item_count + prev_item_count / 2;
}

vec_meta_s *vec_meta(void *vec)
{
    vec_meta_s *meta = vec;
    return meta - 1;
}

void vec_set_expansion_profile(void *vec, size_t (*expansion_profile)(size_t))
{
    if (expansion_profile == NULL)
    {
        return;
    }
    vec_meta(vec)->expansion_profile = expansion_profile;
}

void *vec_create(size_t item_count, size_t item_size)
{
    if (item_size == 0)
    {
        return NULL;
    }
    if (item_count == 0)
    {
        return vec_empty(item_size);
    }
    vec_meta_s *meta = malloc(sizeof(vec_meta_s) + item_count * item_size);
    size_t real_count = VEC_CAP(item_count);
    meta->item_count = real_count;
    meta->item_size = item_size;
    meta->capacity = real_count;
    meta->expansion_profile = VEC_DEFAULT_EXPANSION_PROFILE;
    return meta + 1;
}

void *vec_empty(size_t item_size)
{
    void *vec = vec_create(VEC_DEFAULT_MIN_COUNT, item_size);
    vec_meta(vec)->item_count = 0;
    return vec;
}

size_t vec_count(void *vec)
{
    return vec_meta(vec)->item_count;
}

void *vec_resize(void *vec, size_t item_count)
{
    vec_meta_s *meta = vec_meta(vec);
    size_t real_count = VEC_CAP(item_count);

    meta = realloc(meta, sizeof(vec_meta_s) + meta->item_size * real_count);
    meta->item_count = real_count;
    meta->capacity = real_count;

    return meta + 1;
}

void *vec_add_n(void *vec, size_t n)
{
    /* Save the hassle */
    if (n == 0)
    {
        return vec;
    }
    vec_meta_s *meta = vec_meta(vec);

    if (meta->item_count + n > meta->capacity)
    {
        /* Calculate capacity increase needed to store "n" more items */
        size_t new_capacity = meta->capacity;
        while ((new_capacity = meta->expansion_profile(new_capacity)) < meta->item_count + n)
            ;

        /* Since vec_resize() sets capacity = count, count must be reset after call */
        size_t item_count_bak = meta->item_count;
        vec = vec_resize(vec, new_capacity);
        meta = vec_meta(vec);
        meta->item_count = item_count_bak;
    }
    /* We can safely increase the counter */
    meta->item_count += n;

    return vec;
}

void *vec_add(void *vec)
{
    return vec_add_n(vec, 1);
}

int vec_remove(void *vec, size_t idx)
{
    vec_meta_s *meta = vec_meta(vec);

    /* idx out of range */
    if (idx >= meta->item_count)
    {
        return -1;
    }

    /* if last, easy operation */
    if (idx + 1 == meta->item_count)
    {
        meta->item_count--;
        return 0;
    }

    /* Move everything after removed item one slot behind */
    char *it = vec;
    memmove(it + idx * meta->item_size,
            it + (idx + 1) * meta->item_size,
            (meta->item_count - idx - 1) * meta->item_size);
    meta->item_count--;
    return 0;
}

void *vec_shrink(void *vec)
{
    return vec_resize(vec, vec_meta(vec)->item_count);
}

void *vec_last(void *vec)
{
    vec_meta_s *meta = vec_meta(vec);
    return (meta->item_count == 0) ? NULL : (char *)vec + (meta->item_count - 1) * meta->item_size;
}

void vec_destroy(void *vec)
{
    free(vec_meta(vec));
}