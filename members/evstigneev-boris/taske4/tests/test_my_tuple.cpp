#include <gtest/gtest.h>
#include <type_traits>
#include <utility>
#include <memory>
#include <optional>
#include <variant>
#include <chrono>
#include <vector>
#include <array>
#include <string>
#include <limits>
#include <stdexcept>
#include "../include/MyTuple.h"

TEST(MyTupleTest, ConstructionAndTypes) {
    MyTuple<int, float, double> t(42, 3.14f, 2.718);
    static_assert(std::is_same<decltype(t.get<0>()), int&>::value);
    static_assert(std::is_same<decltype(t.get<1>()), float&>::value);
    static_assert(std::is_same<decltype(t.get<2>()), double&>::value);
}

TEST(MyTupleTest, GetByIndexAndConst) {
    const MyTuple<int, float, double> t(42, 3.14f, 2.718);
    EXPECT_EQ(t.get<0>(), 42);
    EXPECT_FLOAT_EQ(t.get<1>(), 3.14f);
    EXPECT_DOUBLE_EQ(t.get<2>(), 2.718);
}

TEST(MyTupleTest, GetByType) {
    MyTuple<int, float, double> t(42, 3.14f, 2.718);
    EXPECT_EQ(t.get<int>(), 42);
    EXPECT_FLOAT_EQ(t.get<float>(), 3.14f);
    EXPECT_DOUBLE_EQ(t.get<double>(), 2.718);
}

TEST(MyTupleTest, ModificationThroughGet) {
    MyTuple<int, bool, double> t(1, true, 3.14);
    t.get<0>() = 10;
    t.get<bool>() = false;
    t.get<double>() = 2.718;
    EXPECT_EQ(t.get<0>(), 10);
    EXPECT_FALSE(t.get<1>());
    EXPECT_DOUBLE_EQ(t.get<2>(), 2.718);
}

TEST(MyTupleTest, EmptyTuple) {
    MyTuple<> t;
    EXPECT_EQ(t.size(), 0u);
    static_assert(!has_get_v<decltype(t), 0>);
}

TEST(MyTupleTest, SingleElementTuple) {
    MyTuple<int> t(42);
    EXPECT_EQ(t.get<0>(), 42);
    t.get<int>() = 99;
    EXPECT_EQ(t.get<0>(), 99);
    static_assert(has_get_v<decltype(t), 0>);
    static_assert(!has_get_v<decltype(t), 1>);
}

TEST(MyTupleTest, TwoElementsTuple) {
    MyTuple<int, bool> t(7, true);
    EXPECT_EQ(t.get<0>(), 7);
    EXPECT_TRUE(t.get<1>());
    t.get<int>() = 55;
    t.get<bool>() = false;
    EXPECT_EQ(t.get<0>(), 55);
    EXPECT_FALSE(t.get<1>());
}

TEST(MyTupleTest, ManyElementsTuple) {
    MyTuple<int, float, char, bool, double> t(1, 2.5f, 'x', true, 9.99);
    EXPECT_EQ(t.get<int>(), 1);
    EXPECT_FLOAT_EQ(t.get<float>(), 2.5f);
    EXPECT_EQ(t.get<char>(), 'x');
    EXPECT_TRUE(t.get<bool>());
    EXPECT_DOUBLE_EQ(t.get<double>(), 9.99);

    t.get<int>() = 10;
    t.get<char>() = 'y';
    EXPECT_EQ(t.get<0>(), 10);
    EXPECT_EQ(t.get<2>(), 'y');
}

TEST(MyTupleTest, InvalidIndices) {
    using Tuple3 = MyTuple<int, float, double>;
    static_assert(!has_get_v<Tuple3, 3>);
    static_assert(!has_get_v<Tuple3, static_cast<size_t>(-1)>);
}

TEST(MyTupleTest, StructElements) {
    struct Point { int x, y; };
    MyTuple<Point, Point, Point> t(Point{1,2}, Point{3,4}, Point{5,6});
    EXPECT_EQ(t.get<0>().x, 1);
    EXPECT_EQ(t.get<1>().y, 4);
    EXPECT_EQ(t.get<2>().x, 5);
}

TEST(MyTupleTest, ConstCorrectness) {
    const MyTuple<int, float, double> t(1, 2.5f, 3.14);
    static_assert(std::is_same<decltype(t.get<0>()), const int&>::value);
    static_assert(std::is_same<decltype(t.get<1>()), const float&>::value);
    static_assert(std::is_same<decltype(t.get<2>()), const double&>::value);
}

TEST(MyTupleBasicTests, ConstSize) {
    const MyTuple<int, double> t(1, 2.0);
    static_assert(noexcept(t.size()));
    EXPECT_EQ(t.size(), 2);
    
    const MyTuple<> empty;
    static_assert(noexcept(empty.size()));
    EXPECT_EQ(empty.size(), 0);
}

TEST(MyTupleTest, ComparisonOperators) {
    MyTuple<int, char> t1(1, 'a');
    MyTuple<int, char> t2(1, 'a');
    MyTuple<int, char> t3(2, 'b');

    EXPECT_TRUE(t1 == t2);
    EXPECT_FALSE(t1 == t3);
    EXPECT_TRUE(t1 < t3);
    EXPECT_FALSE(t3 < t1);
    EXPECT_TRUE(t1 <= t2);
    EXPECT_TRUE(t1 != t3);
    EXPECT_TRUE(t3 >= t1);
}

TEST(MyTupleTest, MoveSemantics) {
    auto uptr = std::make_unique<int>(42);
    MyTuple<std::unique_ptr<int>> a(std::move(uptr));
    EXPECT_TRUE(a.get<0>() != nullptr);
    EXPECT_EQ(*a.get<std::unique_ptr<int>>(), 42);

    auto b = std::move(a);
    EXPECT_TRUE(b.get<0>() != nullptr);
    EXPECT_EQ(*b.get<0>(), 42);
    EXPECT_EQ(a.get<0>(), nullptr);
}

TEST(MyTupleTest, MakeMyTupleFactory) {
    auto t = make_my_tuple(1, 2.5f, std::string("hi"));
    EXPECT_EQ(t.get<int>(), 1);
    EXPECT_FLOAT_EQ(t.get<float>(), 2.5f);
    EXPECT_EQ(t.get<std::string>(), "hi");
}

TEST(MyTupleTest, NestedTuple) {
    MyTuple<int, MyTuple<double, std::string>> t(1, MyTuple<double, std::string>(3.14, "pi"));
    EXPECT_EQ(t.get<int>(), 1);
    EXPECT_DOUBLE_EQ((t.get<MyTuple<double, std::string>>().get<0>()), 3.14);
    EXPECT_EQ((t.get<MyTuple<double, std::string>>().get<1>()), "pi");
}

TEST(MyTupleTest, WithOptionalAndVariant) {
    MyTuple<std::optional<int>, std::variant<int, std::string>> t(std::optional<int>(5), std::variant<int, std::string>("hello"));
    EXPECT_EQ(t.get<std::optional<int>>().value(), 5);
    EXPECT_EQ(std::get<std::string>(t.get<std::variant<int, std::string>>()), "hello");
}

struct VectorLike {
    static constexpr size_t size() { return 10; }
};

TEST(MyTupleTest, HasGetEdgeCases) {
    static_assert(!has_get_v<VectorLike, 2>);
    using T = MyTuple<int, double>;
    static_assert(has_get_v<T, 0>);
    static_assert(has_get_v<T, 1>);
    static_assert(!has_get_v<T, 2>);
}

TEST(MyTupleStressTest, ExtremeIndexValues) {
    MyTuple<int, double> t(1, 2.0);
    
    EXPECT_NO_THROW(t.get<1>());
    
    static_assert(!has_get_v<decltype(t), 2>);
    static_assert(!has_get_v<decltype(t), static_cast<size_t>(-1)>);
    
    static_assert(!has_get_v<decltype(t), std::numeric_limits<size_t>::max()>);
}

TEST(MyTupleStressTest, ExceptionSafety) {
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        ThrowOnCopy(const ThrowOnCopy&) { throw std::runtime_error("copy failed"); }
        ThrowOnCopy(ThrowOnCopy&&) = default;
    };
    
    MyTuple<ThrowOnCopy, int> t1(ThrowOnCopy{}, 42);
    
    EXPECT_THROW({ auto t2 = t1; }, std::runtime_error);
    
    EXPECT_NO_THROW({ auto t3 = std::move(t1); });
}

TEST(MyTupleStressTest, Performance) {
    constexpr size_t N = 10000;
    
    auto make_large_tuple = [N] {
        return make_my_tuple(std::vector<int>(N), std::array<double, N>{}, std::make_shared<std::string>(std::string(N, 'a')));
    };
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        auto t = make_large_tuple();
        auto t2 = std::move(t);
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    EXPECT_LT((end - start).count(), 1000000) << "too slow";
}

TEST(MyTupleStressTest, DeepMoveSemantics) {
    auto ptr1 = std::make_unique<int>(1);
    auto ptr2 = std::make_unique<int>(2);
    
    MyTuple<std::unique_ptr<int>, MyTuple<std::unique_ptr<int>, std::string>> t1{std::move(ptr1), MyTuple<std::unique_ptr<int>, std::string>{std::move(ptr2), "test"}};
    
    auto t2 = std::move(t1);
    
    EXPECT_EQ(t1.get<0>(), nullptr);
    EXPECT_EQ(*t2.get<0>(), 1);
    EXPECT_EQ(*t2.get<1>().get<0>(), 2);
    EXPECT_EQ(t2.get<1>().get<1>(), "test");
}

TEST(MyTupleExtendedTypeTests, VariantAndOptional) {
    MyTuple<std::variant<int, std::string>, std::optional<std::string>> t(std::variant<int, std::string>("test"), std::nullopt);
    
    EXPECT_EQ(std::get<std::string>(t.get<0>()), "test");
    EXPECT_FALSE(t.get<1>().has_value());
}

TEST(MyTupleExceptionTests, ExceptionSafety) {
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        ThrowOnCopy(const ThrowOnCopy&) { throw std::runtime_error("copy failed"); }
        ThrowOnCopy(ThrowOnCopy&&) = default;
    };

    auto ptr = std::make_shared<int>(0);
    {
        MyTuple<ThrowOnCopy, std::shared_ptr<int>> t1(ThrowOnCopy{}, ptr);
        EXPECT_EQ(ptr.use_count(), 2);

        try {
            auto t2 = t1;
            FAIL() << "Expected exception on copy";
        } 
        catch (const std::runtime_error& e) {
            EXPECT_STREQ(e.what(), "copy failed");
        }
        EXPECT_EQ(ptr.use_count(), 2);
    }
    EXPECT_EQ(ptr.use_count(), 1);
}

TEST(MyTupleEmptyTests, EmptyTupleOperations) {
    MyTuple<> t1;
    MyTuple<> t2;
    
    EXPECT_TRUE(t1 == t2);
    EXPECT_FALSE(t1 != t2);
    EXPECT_FALSE(t1 < t2);
    
    auto t3 = std::move(t1);
    static_assert(std::is_same_v<decltype(t3), MyTuple<>>);
}