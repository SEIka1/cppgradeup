#pragma once
#include <cstddef>
#include <type_traits>
#include <utility>
#include <memory>
#include <optional>
#include <variant>

template<class T, class = void>
struct _is_complete : std::false_type {};
template<class T>
struct _is_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};
template<class T>
inline constexpr bool is_complete_v = _is_complete<T>::value;

template<typename... Ts>
class MyTuple;

template<>
class MyTuple<> {
public:
    static constexpr std::size_t size() noexcept { return 0; }

    friend constexpr bool operator==(const MyTuple&, const MyTuple&) noexcept { return true; }
    friend constexpr bool operator<=(const MyTuple&, const MyTuple&) noexcept { return true; }
    friend constexpr bool operator>=(const MyTuple&, const MyTuple&) noexcept { return true; }
    friend constexpr bool operator!=(const MyTuple&, const MyTuple&) noexcept { return false; }
    friend constexpr bool operator<(const MyTuple&, const MyTuple&) noexcept { return false; }
    friend constexpr bool operator>(const MyTuple&, const MyTuple&) noexcept { return false; }
};

template<typename Head, typename... Tail>
class MyTuple<Head, Tail...> {
    static_assert(!std::is_void_v<Head> && (... && !std::is_void_v<Tail>), "void is not allowed");

    static_assert(is_complete_v<Head> && (is_complete_v<Tail> && ...), "Incomplete types not allowed");

public:
    Head head;
    MyTuple<Tail...> tail;

    constexpr MyTuple() = default;
    constexpr MyTuple(const MyTuple&) = default;
    constexpr MyTuple(MyTuple&&) = default;
    constexpr MyTuple& operator=(const MyTuple&) = default;
    constexpr MyTuple& operator=(MyTuple&&) = default;
    ~MyTuple() = default;

    template <typename H, typename... TT>
    constexpr explicit MyTuple(H&& h, TT&&... tt) : head(std::forward<H>(h)), tail(std::forward<TT>(tt)...) {
        static_assert(sizeof...(TT) == sizeof...(Tail), "wrong number of arguments");
        static_assert(std::is_constructible_v<Head, H&&>, "Head not constructible from H&&");
        static_assert((std::is_constructible_v<Tail, TT&&> && ...), "Tail not constructible from TT&&");
    }

    static constexpr std::size_t size() noexcept { return sizeof...(Tail) + 1; }

    template <std::size_t I>
    constexpr decltype(auto) get() & {
        static_assert(I < size(), "Tuple index out of range");
        if constexpr (I == 0) return (head);
        else return tail.template get<I - 1>();
    }

    template <std::size_t I>
    constexpr decltype(auto) get() const& {
        static_assert(I < size(), "Tuple index out of range");
        if constexpr (I == 0) return (std::as_const(head));
        else return tail.template get<I - 1>();
    }

    template <std::size_t I>
    constexpr decltype(auto) get() && {
        static_assert(I < size(), "Tuple index out of range");
        if constexpr (I == 0) return std::move(head);
        else return std::move(tail).template get<I - 1>();
    }

    template <std::size_t I>
    constexpr decltype(auto) get() const&& {
        static_assert(I < size(), "Tuple index out of range");
        if constexpr (I == 0) return std::move(head); 
        else return std::move(tail).template get<I - 1>();
    }

private:
    template <typename T, typename... Args>
    struct index_of_impl;

    template <typename T, typename H>
    struct index_of_impl<T, H> {
        static constexpr std::size_t value = std::is_same_v<T, H> ? 0 : static_cast<std::size_t>(-1);
    };

    template <typename T, typename H, typename Next, typename... TT>
    struct index_of_impl<T, H, Next, TT...> {
    private:
        static constexpr std::size_t tail_value = index_of_impl<T, Next, TT...>::value;
    public:
        static constexpr std::size_t value = std::is_same_v<T, H> ? 0 : (tail_value == static_cast<std::size_t>(-1) ? static_cast<std::size_t>(-1) : 1 + tail_value);
    };

    template <typename T, typename... XS>
    struct count_type;
    template <typename T>
    struct count_type<T> : std::integral_constant<int, 0> {};
    template <typename T, typename X, typename... XS>
    struct count_type<T, X, XS...> : std::integral_constant<int, (std::is_same_v<T, X> ? 1 : 0) + count_type<T, XS...>::value> {};

public:
    template <typename T>
    static constexpr std::size_t index_of_v = index_of_impl<T, Head, Tail...>::value;

    template <typename T>
    static constexpr int count_type_v = count_type<T, Head, Tail...>::value;

    template <typename T>
    constexpr decltype(auto) get() & {
        static_assert(index_of_v<T> != static_cast<std::size_t>(-1), "type not found");
        return static_cast<MyTuple&>(*this).template get<index_of_v<T>>();
    }
    template <typename T>
    constexpr decltype(auto) get() const& {
        static_assert(index_of_v<T> != static_cast<std::size_t>(-1), "type not found");
        return static_cast<const MyTuple&>(*this).template get<index_of_v<T>>();
    }
    template <typename T>
    constexpr decltype(auto) get() && {
        static_assert(index_of_v<T> != static_cast<std::size_t>(-1), "type not found");
        return static_cast<MyTuple&&>(*this).template get<index_of_v<T>>();
    }
    template <typename T>
    constexpr decltype(auto) get() const&& {
        static_assert(index_of_v<T> != static_cast<std::size_t>(-1), "type not found");
        return static_cast<const MyTuple&&>(*this).template get<index_of_v<T>>();
    }

    friend constexpr bool operator==(const MyTuple& a, const MyTuple& b) {
        if constexpr (sizeof...(Tail) == 0) return a.head == b.head;
        else return (a.head == b.head) && (a.tail == b.tail);
    }
    friend constexpr bool operator!=(const MyTuple& a, const MyTuple& b) { return !(a == b); }

    friend constexpr bool operator<(const MyTuple& a, const MyTuple& b) {
        if (a.head < b.head) return true;
        if (b.head < a.head) return false;
        if constexpr (sizeof...(Tail) == 0) return false;
        else return a.tail < b.tail;
    }

    friend constexpr bool operator<=(const MyTuple& a, const MyTuple& b) { return !(b < a); }
    friend constexpr bool operator>(const MyTuple& a, const MyTuple& b) { return (b < a); }
    friend constexpr bool operator>=(const MyTuple& a, const MyTuple& b) { return !(a < b); }
};

namespace _mytuple_detail {
    template <typename T, typename = void>
    struct has_static_size : std::false_type {};

    template <typename T>
    struct has_static_size<T, std::void_t<decltype(T::size())>> : std::bool_constant<std::is_convertible_v<decltype(T::size()), std::size_t>> {};

    template <typename T, std::size_t I, typename = void>
    struct has_get_impl : std::false_type {};

    template <typename T, std::size_t I>
    struct has_get_impl<T, I, std::void_t<decltype(T::size())>> {
    private:
        template <typename U, std::enable_if_t<(I < static_cast<std::size_t>(U::size())), int> = 0>
        static auto test(int) -> decltype(
            void(std::declval<U&>().template get<I>()),
            void(std::declval<const U&>().template get<I>()),
            std::true_type{}
        );

        template <typename>
        static std::false_type test(...);

    public:
        static constexpr bool value = decltype(test<T>(0))::value;
    };
}

template <typename T, std::size_t I>
constexpr bool has_get_v = _mytuple_detail::has_get_impl<T, I>::value;

template <typename... Ts>
constexpr MyTuple<std::decay_t<Ts>...> make_my_tuple(Ts&&... args) {
    return MyTuple<std::decay_t<Ts>...>(std::forward<Ts>(args)...);
}
