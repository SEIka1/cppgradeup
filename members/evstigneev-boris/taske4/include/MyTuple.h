#pragma once
#include <cstddef>
#include <type_traits>

template<typename... Ts>
class MyTuple;

template<>
class MyTuple<> {
public:
    static constexpr size_t size() { return 0; }
};

template<typename Head, typename... Tail>
class MyTuple<Head, Tail...> {
public:
    Head head;
    MyTuple<Tail...> tail;

    constexpr MyTuple(const Head& h, const Tail&... t) : head(h), tail(t...) {}

    template <std::size_t I>
    constexpr auto& get() {
        static_assert(I < sizeof...(Tail) + 1, "Tuple index out of range");
        if constexpr (I == 0) return head;
        else return tail.template get<I - 1>();
    }

    template <std::size_t I>
    constexpr const auto& get() const {
        static_assert(I < sizeof...(Tail) + 1, "Tuple index out of range");
        if constexpr (I == 0) return head;
        else return tail.template get<I - 1>();
    }

    static constexpr size_t size() { return sizeof...(Tail) + 1; }
};

template <typename T, size_t I, typename = void>
struct has_get_impl : std::false_type {};

template <typename T, size_t I>
struct has_get_impl<T, I, std::enable_if_t<(I < T::size())>> : std::true_type {};

template <typename T, size_t I>
constexpr bool has_get_v = has_get_impl<T, I>::value;