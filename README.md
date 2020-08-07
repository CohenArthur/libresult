# libresult

"Generic" Result type in C

# Usage

The library is header only. You can simply add the following code to define a result type

* in your header:
```c
#include "result.h"

RES_DECLARE(result_ptr, void *, char *);
```

* in your source file:
```c
#include "option.h"

RES_DEFINE(result_ptr, void *, char *);
```

This declares and defines a `result` with a `void *` as a `Good` data, and a `char *` as
a `Bad` data.

You can thus write a safer malloc for example !

```c
#include <stdlib.h>

struct result_ptr r_malloc(size_t size)
{
    void *ptr = malloc(size);

    if (!ptr)
        return result_ptr_bad_new("allocation failed");

    return result_ptr_good_new(ptr);
}
```

To access the allocated pointers, your users have to check the result or they risk a
program crash

```c
struct result_ptr *r_ptr = r_malloc(1);

void *ptr = result_ptr_good(r_ptr); // Will crash if the allocation failed !

if (result_ptr_is_good(r_ptr))
    /* Allocation was successful */
else
    /* Something went wrong */
```

`result`s are also used to chain errors and bring them back up easily

```c

char *err_aggregate(char *og, char *new)
{
    size_t og_l = strlen(og);
    size_t new_l = strlen(new);

    char *og = realloc(og, og_l + new_l + 1);

    memcpy(og + og_l, new, new_l);

    og[og_l + new_l] = '\0';

    return og;
}

struct result_ptr r_read_file(char *path)
{
    struct result_ptr r_ptr = r_malloc(sizeof(struct some_file_helper));

    if (result_ptr_is_bad(r_ptr))
    {
        /* Return the error which is now "allocation failed: couldn't open file" */
        result_ptr_bad_aggregate(r_ptr, ": couldn't open file", err_aggregate))

        return r_ptr;
    }

    /* Allocation was fine */
}

```

The resulting struct exposes the following API

|function's name|
|---|
|`<name>_bad`|
|`<name>_is_bad`|
|`<name>_bad_new`|
|`<name>_bad_aggregate`|
|`<name>_good`|
|`<name>_is_good`|
|`<name>_good_new`|

# Limitations
