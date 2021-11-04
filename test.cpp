#include <gtest/gtest.h>

#include <exception>
#include <map>

#include "BucketAllocator.h"
namespace {
    TEST(BucketAllocatorSuite, vector_test) {
        std::map<int, int, std::less<int>, BucketAllocator<std::pair<const int, int>, 10>> v;
        for (int i = 0; i < 10; i++) {
            v.emplace_hint(v.end(), i, i);
        }

        for (int i = 0; i < 10; i++) {
            EXPECT_EQ(v[i], i);
        }
        EXPECT_THROW(v.emplace_hint(v.end(), 10, 10), std::bad_alloc);

    }
}