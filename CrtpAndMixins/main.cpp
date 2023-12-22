#include <iostream>
#include <type_traits>

template <typename T, typename Tag = decltype([] {})>
struct StrongType {
    using type = T;

    explicit StrongType(T&& value)
        : m_value(std::forward<T>(value))
    {
    }

    StrongType(const StrongType& other) = default;
    StrongType(StrongType&& other) = default;
    StrongType& operator=(const StrongType& other) = default;
    StrongType& operator=(StrongType&& other) = default;

    [[nodiscard]] const T& value() const
    {
        return m_value;
    }
    [[nodiscard]] T& value()
    {
        return m_value;
    }

private:
    T m_value;
};

struct FooTag { };

using Foo = StrongType<int>;
using Bar = StrongType<int>;

static_assert(!std::is_same_v<Foo, Bar>, "Foo and Bar must be distinct types");

// Curiously recurring template pattern
namespace crtp {
template <typename T>
struct Comparable {
    [[nodiscard]] bool operator==(const Comparable<T>& other) const
    {
        return static_cast<const T&>(*this).value() == static_cast<const T&>(other).value();
    }
};

template <typename T>
struct Incrementable {
    Incrementable& operator++()
    {
        ++static_cast<T&>(*this).value();
        return (*this);
    }
};

template <typename T>
struct Printable {
    void print() const
    {
        std::cout << static_cast<const T&>(*this).value() << std::endl;
    }
};

// # Policy based design?
template <typename T, typename Tag, template <typename> typename... Args>
struct StrongType : public Args<StrongType<T, Tag, Args...>>... {
    explicit StrongType(T&& value)
        : m_value(std::forward<T>(value))
    {
    }

    StrongType(const StrongType& other) = default;
    StrongType(StrongType&& other) = default;
    StrongType& operator=(const StrongType& other) = default;
    StrongType& operator=(StrongType&& other) = default;

    [[nodiscard]] const T& value() const
    {
        return m_value;
    }
    [[nodiscard]] T& value()
    {
        return m_value;
    }

private:
    T m_value;
};

struct FooTag {
};
struct BarTag {
};

using Foo = StrongType<int, FooTag, Printable, Comparable, Incrementable>;
using Bar = StrongType<int, BarTag, Printable, Comparable, Incrementable>;

static_assert(!std::is_same_v<Foo, Bar>, "Foo and Bar are not distinct types");
} // namespace crtp

// From wikipedia
/*
In object-oriented programming languages, a mixin (or mix-in)[1][2][3][4] is a class that contains methods for use by other classes without having to be the parent class of those other classes.
How those other classes gain access to the mixin's methods depends on the language. Mixins are sometimes described as being "included" rather than "inherited".
*/

namespace mixins {

// Here we can avoid touching the class itself, but we pay for it by having to use our own type instead (Mixin)
template <typename T>
struct Printable {
    Printable(T& underlying)
        : m_underlying(underlying)
    {
    }

    void print() const
    {
        std::cout << "mixin:" << m_underlying.value() << std::endl;
    }

private:
    T& m_underlying;
};

template <typename T>
struct Comparable {
    Comparable(T& underlying)
        : m_underlying(underlying)
    {
    }

    [[nodiscard]] bool operator==(const Comparable<T>& other) const
    {
        return m_underlying.value() == m_underlying.value();
    }

private:
    T& m_underlying;
};

// But we have to specicy the constructor
template <typename StrongType, template <typename> typename... Args>
struct Mixin : public StrongType, Args<StrongType>... {
    Mixin(typename StrongType::type&& value)
        : StrongType(std::forward<typename StrongType::type>(value))
        , Args<StrongType>(static_cast<StrongType&>(*this))...
    {
    }
};

using FooOnSteroids = Mixin<Foo, Printable, Comparable>;
} // namespace mixins

// The fundamental theorem of software engineering (FTSE)
// "We can solve any problem by introducing an extra level of indirection."
// by Andrew Koenig to describe a remark by Butler Lampson.

namespace mixins_and_crtp {
// Solving the size and constructor problems
template <template <typename T> typename... Args>
struct Mix {
    template <typename T>
    struct into : public T, Args<Mix<Args...>::into<T>>... {
        using T::T;
    };
};

using FooOnSteroids = Mix<crtp::Printable, crtp::Comparable>::into<Foo>;
}

int main()
{
    auto foo = Foo { 10 };
    auto bar = Bar { 42 };

    auto printableFoo = crtp::Foo { 10 };
    auto printableFoo2 = crtp::Foo { 11 };

    printableFoo.print();
    printableFoo2.print();

    if (printableFoo == printableFoo2) {
        std::cout << "We have equality!" << std::endl;
    }

    ++printableFoo;
    printableFoo.print();
    ++printableFoo;
    printableFoo.print();
    ++printableFoo;
    printableFoo.print();

    auto mixinFoo = mixins::FooOnSteroids { 100 };

    mixinFoo.print();

    std::cout << "Sizeof mixins printable: " << sizeof(mixins::Printable<Foo>) << std::endl;
    std::cout << "Sizeof mixins comparable: " << sizeof(mixins::Comparable<Foo>) << std::endl;
    std::cout << "Sizeof FooOnSteroids: " << sizeof(mixins::FooOnSteroids) << std::endl;
    std::cout << "Sizeof crtp::Foo: " << sizeof(crtp::Foo) << std::endl;
    std::cout << "Sizeof Foo: " << sizeof(Foo) << std::endl;
    std::cout << "Sizeof FooOnSteroids: " << sizeof(mixins::FooOnSteroids) << std::endl;
    std::cout << "Alignment of pointers: " << std::alignment_of_v<Foo*> << std::endl;
    std::cout << "Alignment of references: " << std::alignment_of_v<Foo&> << std::endl;

    if (mixinFoo == mixinFoo) {
        std::cout << "Is comparable." << std::endl;
    }

    mixins_and_crtp::FooOnSteroids { 33 }.print();
    std::cout << "Sizeof mixins_and_crtp::FooOnSteroids: " << sizeof(mixins_and_crtp::FooOnSteroids) << std::endl;

    std::cout << "Hello" << std::endl;
}
