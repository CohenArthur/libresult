#pragma once

#include <err.h>
#include <stdbool.h>

/**
 * Different types of result
 */
enum res_kind { BAD = 0, GOOD };

#define RES_DECLARE(Name, Good, Bad)                                    \
    struct Name##_bad {                                                 \
        Bad error;                                                      \
    };                                                                  \
                                                                        \
    struct Name##_good {                                                \
        Good data;                                                      \
    };                                                                  \
                                                                        \
    typedef Bad (*Name##_fn_aggregate)(Bad old_bad, Bad new_bad);       \
                                                                        \
    /**                                                                 \
     * Simple result struct containing a `Good` or a `Bad` type         \
     */                                                                 \
    struct Name;                                                        \
                                                                        \
    /**                                                                 \
     * Returns the data associated with a bad result                    \
     */                                                                 \
    Bad Name##_bad(struct Name *res);                                   \
                                                                        \
    /**                                                                 \
     * Returns the data associated with a good result                   \
     */                                                                 \
    Good Name##_good(struct Name *res);                                 \
                                                                        \
    /**                                                                 \
     * Create a new, good result                                        \
     */                                                                 \
    struct Name Name##_good_new(Good data);                             \
                                                                        \
    /**                                                                 \
     * Create a new, bad result                                         \
     */                                                                 \
    struct Name Name##_bad_new(Bad error);                              \
                                                                        \
    /**                                                                 \
     * Is the result positive                                           \
     */                                                                 \
    bool Name##_is_good(struct Name *res);                              \
                                                                        \
    /**                                                                 \
     * Is the result negative                                           \
     */                                                                 \
    bool Name##_is_bad(struct Name *res);                               \
                                                                        \
    /**                                                                 \
     * Append new information to the existing `Bad` result. `fn` is the \
     * function used to append new_err to the existing error            \
     */                                                                 \
    void Name##_propagate(struct Name *res, Bad new_err,                \
                          Name##_fn_aggregate fn);

#define RES_DEFINE(Name, Good, Bad)                                     \
    struct Name {                                                       \
        /* The kind of result contained */                              \
        enum res_kind kind;                                             \
                                                                        \
        /* The different types contained inside the Result */           \
        union {                                                         \
            struct Name##_bad bad;                                      \
            struct Name##_good good;                                    \
        } data;                                                         \
    };                                                                  \
                                                                        \
    Bad Name##_bad(struct Name *res) {                                  \
        if (res->kind == GOOD)                                          \
            errx(1, "%s", "trying to access error on `Good` result");   \
                                                                        \
        return res->data.bad.error;                                     \
    }                                                                   \
                                                                        \
    Good Name##_good(struct Name *res) {                                \
        if (res->kind == BAD)                                           \
            errx(1, "%s", "trying to access data on `Bad` result");     \
                                                                        \
        return res->data.good.data;                                     \
    }                                                                   \
                                                                        \
    struct Name Name##_good_new(Good data) {                            \
        struct Name res = {                                             \
            .kind = GOOD,                                               \
            .data.good.data = data,                                     \
        };                                                              \
                                                                        \
        return res;                                                     \
    }                                                                   \
                                                                        \
    struct Name Name##_bad_new(Bad error) {                             \
        struct Name res = {                                             \
            .kind = BAD,                                                \
            .data.bad.error = error,                                    \
        };                                                              \
                                                                        \
        return res;                                                     \
    }                                                                   \
                                                                        \
    bool Name##_is_good(struct Name *res) { return res->kind == GOOD; } \
                                                                        \
    bool Name##_is_bad(struct Name *res) { return res->kind == BAD; }   \
                                                                        \
    void Name##_bad_propagate(struct Name *res, Bad new_err,            \
                              Name##_fn_aggregate fn) {                 \
        if (res->kind == GOOD)                                          \
            errx(1, "%s", "trying to access error on `Good` result");   \
                                                                        \
        res->data.bad.error = fn(res->data.bad.error, new_err);         \
    }
