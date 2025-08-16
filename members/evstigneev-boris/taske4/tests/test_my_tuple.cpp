#include <gtest/gtest.h>
#include <string>
#include <type_traits>
#include <utility>
#include "../include/MyTuple.h"

class MyTupleTest : public ::testing::Test {
protected:
    void SetUp() override {
        intFloatStringTuple = MyTuple<int, float, std::string>(42, 3.14f, "hello");
        stringBoolIntTuple = MyTuple<std::string, bool, int>("world", true, 100);
        emptyTuple = MyTuple<int, float, double>();
    }

    MyTuple<int, float, std::string> intFloatStringTuple;
    MyTuple<std::string, bool, int> stringBoolIntTuple;
    MyTuple<int, float, double> emptyTuple;
};

template <typename T, size_t I, typename = void>
struct has_get : std::false_type {};

template <typename T, size_t I>
struct has_get<T, I, std::void_t<decltype(std::declval<T>().template get<I>())>> 
    : std::true_type {};

template <typename T, size_t I>
constexpr bool has_get_v = has_get<T, I>::value;

TEST_F(MyTupleTest, ConstructionAndTypes) {
    static_assert(std::is_same_v<decltype(intFloatStringTuple.get<0>()), int&>);
    static_assert(std::is_same_v<decltype(intFloatStringTuple.get<1>()), float&>);
    static_assert(std::is_same_v<decltype(intFloatStringTuple.get<2>()), std::string&>);
}

TEST_F(MyTupleTest, GetByIndex) {
    EXPECT_EQ(intFloatStringTuple.get<0>(), 42);
    EXPECT_FLOAT_EQ(intFloatStringTuple.get<1>(), 3.14f);
    EXPECT_EQ(intFloatStringTuple.get<2>(), "hello");

    const auto& constTuple = intFloatStringTuple;
    EXPECT_EQ(constTuple.get<0>(), 42);
    EXPECT_FLOAT_EQ(constTuple.get<1>(), 3.14f);
    EXPECT_EQ(constTuple.get<2>(), "hello");
}

TEST_F(MyTupleTest, ModificationThroughGet) {
    intFloatStringTuple.get<0>() = 100;
    EXPECT_EQ(intFloatStringTuple.get<0>(), 100);

    stringBoolIntTuple.get<1>() = false;
    EXPECT_EQ(stringBoolIntTuple.get<1>(), false);
}

TEST_F(MyTupleTest, DefaultConstruction) {
    EXPECT_EQ(emptyTuple.get<0>(), 0);
    EXPECT_FLOAT_EQ(emptyTuple.get<1>(), 0.0f);
    EXPECT_DOUBLE_EQ(emptyTuple.get<2>(), 0.0);
}

TEST_F(MyTupleTest, CompileTimeChecks) {
    static_assert(has_get_v<decltype(intFloatStringTuple), 0>);
    static_assert(has_get_v<decltype(intFloatStringTuple), 1>);
    static_assert(has_get_v<decltype(intFloatStringTuple), 2>);

    static_assert(!has_get_v<decltype(intFloatStringTuple), 3>);
    static_assert(!has_get_v<decltype(intFloatStringTuple), static_cast<size_t>(-1)>);
}

TEST_F(MyTupleTest, VariousTypeCombinations) {
    MyTuple<std::string, std::string, std::string> stringTuple("a", "b", "c");
    EXPECT_EQ(stringTuple.get<0>(), "a");
    EXPECT_EQ(stringTuple.get<1>(), "b");
    EXPECT_EQ(stringTuple.get<2>(), "c");

    struct Point { int x, y; };
    MyTuple<Point, Point, Point> structTuple(Point{1,2}, Point{3,4}, Point{5,6});
    EXPECT_EQ(structTuple.get<0>().x, 1);
    EXPECT_EQ(structTuple.get<1>().y, 4);
    EXPECT_EQ(structTuple.get<2>().x, 5);
}

TEST_F(MyTupleTest, ConstCorrectness) {
    const auto& constTuple = intFloatStringTuple;
    static_assert(std::is_same_v<decltype(constTuple.get<0>()), const int&>);
    static_assert(std::is_same_v<decltype(constTuple.get<1>()), const float&>);
    static_assert(std::is_same_v<decltype(constTuple.get<2>()), const std::string&>);
}