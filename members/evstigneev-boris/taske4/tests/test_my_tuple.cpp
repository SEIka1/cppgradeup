#include <gtest/gtest.h>
#include <type_traits>
#include <utility>
#include "../include/MyTuple.h"

TEST(MyTupleTest, ConstructionAndTypes) {
    MyTuple<int, float, double> t(42, 3.14f, 2.718);
    static_assert(std::is_same_v<decltype(t.get<0>()), int&>);
    static_assert(std::is_same_v<decltype(t.get<1>()), float&>);
    static_assert(std::is_same_v<decltype(t.get<2>()), double&>);
}

TEST(MyTupleTest, GetByIndexAndConst) {
    const MyTuple<int, float, double> t(42, 3.14f, 2.718);
    EXPECT_EQ(t.get<0>(), 42);
    EXPECT_FLOAT_EQ(t.get<1>(), 3.14f);
    EXPECT_DOUBLE_EQ(t.get<2>(), 2.718);
}

TEST(MyTupleTest, ModificationThroughGet) {
    MyTuple<int, bool, double> t(1, true, 3.14);
    t.get<0>() = 10;
    t.get<1>() = false;
    t.get<2>() = 2.718;
    EXPECT_EQ(t.get<0>(), 10);
    EXPECT_EQ(t.get<1>(), false);
    EXPECT_DOUBLE_EQ(t.get<2>(), 2.718);
}

TEST(MyTupleTest, EmptyTuple) {
    MyTuple<> t;
    static_assert(!has_get_v<decltype(t), 0>);
}

TEST(MyTupleTest, SingleElementTuple) {
    MyTuple<int> t(42);
    EXPECT_EQ(t.get<0>(), 42);
    t.get<0>() = 99;
    EXPECT_EQ(t.get<0>(), 99);
    static_assert(has_get_v<decltype(t), 0>);
    static_assert(!has_get_v<decltype(t), 1>);
}

TEST(MyTupleTest, TwoElementsTuple) {
    MyTuple<int, bool> t(7, true);
    EXPECT_EQ(t.get<0>(), 7);
    EXPECT_EQ(t.get<1>(), true);
    t.get<0>() = 55;
    t.get<1>() = false;
    EXPECT_EQ(t.get<0>(), 55);
    EXPECT_EQ(t.get<1>(), false);
}

TEST(MyTupleTest, ManyElementsTuple) {
    MyTuple<int, float, char, bool, double> t(1, 2.5f, 'x', true, 9.99);
    EXPECT_EQ(t.get<0>(), 1);
    EXPECT_FLOAT_EQ(t.get<1>(), 2.5f);
    EXPECT_EQ(t.get<2>(), 'x');
    EXPECT_EQ(t.get<3>(), true);
    EXPECT_DOUBLE_EQ(t.get<4>(), 9.99);

    t.get<0>() = 10;
    t.get<2>() = 'z';
    EXPECT_EQ(t.get<0>(), 10);
    EXPECT_EQ(t.get<2>(), 'z');

    static_assert(has_get_v<decltype(t), 0>);
    static_assert(has_get_v<decltype(t), 4>);
    static_assert(!has_get_v<decltype(t), 5>);
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
    static_assert(std::is_same_v<decltype(t.get<0>()), const int&>);
    static_assert(std::is_same_v<decltype(t.get<1>()), const float&>);
    static_assert(std::is_same_v<decltype(t.get<2>()), const double&>);
}
