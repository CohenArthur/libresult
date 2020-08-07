#pragma once

#include <err.h>
#include <stdbool.h>

#define GOOD_TYPE unsigned long
#define BAD_TYPE char *

struct res_bad {
    BAD_TYPE error;
};

struct res_good {
    GOOD_TYPE data;
};

typedef BAD_TYPE (*res_fn_aggregate)(BAD_TYPE old_bad, BAD_TYPE new_bad);

/**
 * Different types of result
 */
enum res_kind {
    BAD = 0,
    GOOD
};

/**
 * Simple result struct containing a `Good` or a `Bad` type
 */
struct result {
    /* The kind of result contained */
    enum res_kind kind;

    /* The different types contained inside the Result */
    union {
        struct res_bad bad;
        struct res_good good;
    } data;
};

/**
 * Returns the data associated with a bad result
 */
BAD_TYPE res_bad(struct result *res);

/**
 * Returns the data associated with a good result
 */
GOOD_TYPE res_good(struct result *res);

/**
 * Create a new, good result
 */
struct result res_good_new(GOOD_TYPE data);

/**
 * Create a new, bad result
 */
struct result res_bad_new(BAD_TYPE error);

/**
 * Is the result positive
 */
bool res_is_good(struct result *res);

/**
 * Is the result negative
 */
bool res_is_bad(struct result *res);

/**
 * Append new information to the existing `Bad` result. `fn` is the function used
 * to append new_err to the existing error
 */
void res_bad_aggregate(struct result *res, BAD_TYPE new_err, res_fn_aggregate fn);

BAD_TYPE res_bad(struct result *res)
{
    if (res->kind == GOOD)
        errx(1, "%s", "trying to access error on `Good` result");

    return res->data.bad.error;
}

GOOD_TYPE res_good(struct result *res)
{
    if (res->kind == BAD)
        errx(1, "%s", "trying to access data on `Bad` result");

    return res->data.good.data;
}

struct result res_good_new(GOOD_TYPE data)
{
    struct result res = {
        .kind = GOOD,
        .data.good.data = data,
    };

    return res;
}

struct result res_bad_new(BAD_TYPE error)
{
    struct result res = {
        .kind = BAD,
        .data.bad.error = error,
    };

    return res;
}

bool res_is_good(struct result *res)
{
    return res->kind == GOOD;
}

bool res_is_bad(struct result *res)
{
    return res->kind == BAD;
}

void res_bad_aggregate(struct result *res, BAD_TYPE new_err, res_fn_aggregate fn)
{
    if (res->kind == GOOD)
        errx(1, "%s", "trying to access error on `Good` result");

    res->data.bad.error = fn(res->data.bad.error, new_err);
}
