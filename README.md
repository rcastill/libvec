# libvec
Super simple C vector library

**Example usage**
```c
#include <stdio.h>
#include <stdlib.h>
#include <vec.h>

int main()
{
    int *numbers = vec_new(int);
    int *last;
    
    for (int i = 1; i <= 10; i++)
    {
        numbers = vec_add(numbers);
        if ((last = vec_last(numbers)) != NULL)
        {
            *last = i;
        }
    }

    for (size_t i = 0; i < vec_count(numbers); i++)
    {
        printf("%d\n", numbers[i]);
    }

    vec_destroy(numbers);
}
```

**Build**
```bash
git clone https://github.com/rcastill/libvec.git
cd libvec && make install
```
This will install the static library under `/usr/local/lib` and the header under `/usr/local/include`. You are free to modify the `Makefile` to your convinience.
