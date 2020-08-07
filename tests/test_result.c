#include <assert.h>
#include <string.h>
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

int main(void)
{
    RUN_TEST(result, good);
    RUN_TEST(result, bad);
    // RUN_TEST(result, bad_get);
}
