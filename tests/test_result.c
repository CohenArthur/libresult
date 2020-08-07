#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "../src/result.h"

#define TEST(Name, Subname) void test_##Name##_##Subname()
#define RUN_TEST(Name, Subname) test_##Name##_##Subname()

#define ASSERT_TRUE(Exp) assert(Exp)
#define ASSERT_EQ(Lhs, Rhs) assert(Lhs == Rhs)

TEST(result, good)
{
    struct result res = res_good_new(120);

    ASSERT_EQ(res_good(&res), 120);
    ASSERT_TRUE(res_is_good(&res));
}

TEST(result, bad)
{
    struct result res = res_bad_new("Oh fuck");

    ASSERT_EQ(strcmp(res_bad(&res), "Oh fuck"), 0);
    ASSERT_TRUE(res_is_bad(&res));
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

TEST(result, aggregate)
{
    char *err_msg = calloc(1, 256);
    memcpy(err_msg, "Result<", 7);

    struct result res = res_bad_new(err_msg);

    res_bad_aggregate(&res, "Option<T>>", str_concat);

    ASSERT_EQ(strcmp(res_bad(&res), "Result<Option<T>>"), 0);

    free(res_bad(&res));
}

int main(void)
{
    RUN_TEST(result, good);
    RUN_TEST(result, bad);
    RUN_TEST(result, aggregate);
}
