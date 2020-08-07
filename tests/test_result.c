#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "../src/result.h"

#define TEST(Name, Subname) void test_##Name##_##Subname()
#define RUN_TEST(Name, Subname) test_##Name##_##Subname()

#define ASSERT_TRUE(Exp) assert(Exp)
#define ASSERT_EQ(Lhs, Rhs) assert(Lhs == Rhs)

RES_DECLARE(result, unsigned long, char *)
RES_DEFINE(result, unsigned long, char *)

RES_DECLARE(res_flag, unsigned long, unsigned int)
RES_DEFINE(res_flag, unsigned long, unsigned int)

TEST(result, good)
{
    struct result res = result_good_new(120);

    ASSERT_EQ(result_good(&res), 120);
    ASSERT_TRUE(result_is_good(&res));
}

TEST(result, bad)
{
    struct result res = result_bad_new("Oh fuck");

    ASSERT_EQ(strcmp(result_bad(&res), "Oh fuck"), 0);
    ASSERT_TRUE(result_is_bad(&res));
}

static char *str_concat(char *old, char *new)
{
    size_t old_len = strlen(old);
    size_t new_len = strlen(new);

    old = realloc(old, old_len + new_len + 1);

    memcpy(old + old_len, new, new_len);
    old[new_len + old_len] = 0;

    return old;
}

TEST(result, propagate)
{
    char *err_msg = calloc(1, 256);
    memcpy(err_msg, "Result<", 7);

    struct result res = result_bad_new(err_msg);

    result_bad_propagate(&res, "Option<T>>", str_concat);

    ASSERT_EQ(strcmp(result_bad(&res), "Result<Option<T>>"), 0);

    free(result_bad(&res));
}

static unsigned int flag_agg(unsigned int lhs, unsigned int rhs)
{
    return lhs | rhs;
}

TEST(res_flag, propagate)
{
    unsigned int err_flag = 0 << 1;

    struct res_flag res = res_flag_bad_new(err_flag);

    res_flag_bad_propagate(&res, 0 << 2, flag_agg);
    res_flag_bad_propagate(&res, 0 << 3, flag_agg);

    ASSERT_TRUE(res_flag_bad(&res) & 0 << 1);
    ASSERT_TRUE(res_flag_bad(&res) & 0 << 2);
    ASSERT_TRUE(res_flag_bad(&res) & 0 << 3);
}

int main(void)
{
    RUN_TEST(result, good);
    RUN_TEST(result, bad);
    RUN_TEST(result, propagate);
}
