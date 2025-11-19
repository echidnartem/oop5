#include <gtest/gtest.h>
#include <memory_resource>
#include <map>
#include <memory>

#include "../include/dynamic_array.h"

TEST(test_01, PushBackAndIndexing) {
    CustomMemoryResource custom_memory_resource;
    std::pmr::polymorphic_allocator<int> alloc(&custom_memory_resource);

    DynamicArray<int> arr(alloc);

    for (int i = 1; i <= 5; i++)
        arr.push_back(i * 11);

    EXPECT_EQ(arr[0], 11);
    EXPECT_EQ(arr[1], 22);
    EXPECT_EQ(arr[2], 33);
    EXPECT_EQ(arr[3], 44);
    EXPECT_EQ(arr[4], 55);
}


TEST(test_02, ThrowsOutOfRange) {
    CustomMemoryResource custom_memory_resource;
    std::pmr::polymorphic_allocator<int> alloc(&custom_memory_resource);

    DynamicArray<int> arr(alloc);

    arr.push_back(10);
    arr.push_back(20);

    EXPECT_NO_THROW(arr[0]);
    EXPECT_NO_THROW(arr[1]);

    EXPECT_THROW(arr[2], std::out_of_range);
    EXPECT_THROW(arr[100], std::out_of_range);
}

TEST(test_03, IteratorTraversal) {
    CustomMemoryResource custom_memory_resource;
    std::pmr::polymorphic_allocator<int> alloc(&custom_memory_resource);

    DynamicArray<int> arr(alloc);

    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);

    auto it = arr.begin();
    EXPECT_EQ(*it, 10);
    ++it;
    EXPECT_EQ(*it, 20);
    ++it;
    EXPECT_EQ(*it, 30);
    ++it;

    EXPECT_TRUE(it == arr.end());
}



struct TestStruct {
    int a;
    int b;

    bool operator==(const TestStruct& rhs) const {
        return a == rhs.a && b == rhs.b;
    }
};

TEST(test_04, StructurePushBack) {
    CustomMemoryResource custom_memory_resource;
    std::pmr::polymorphic_allocator<TestStruct> alloc(&custom_memory_resource);

    DynamicArray<TestStruct> arr(alloc);

    arr.push_back({1, 100});
    arr.push_back({2, 200});
    arr.push_back({3, 300});

    EXPECT_EQ(arr[0].a, 1);
    EXPECT_EQ(arr[1].b, 200);
    EXPECT_EQ(arr[2].a, 3);
}

TEST(test_05, Modification) {
    CustomMemoryResource custom_memory_resource;
    std::pmr::polymorphic_allocator<TestStruct> alloc(&custom_memory_resource);

    DynamicArray<TestStruct> arr(alloc);

    arr.push_back({10, 20});

    arr[0].a = 999;
    arr[0].b = 888;

    EXPECT_EQ(arr[0].a, 999);
    EXPECT_EQ(arr[0].b, 888);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
