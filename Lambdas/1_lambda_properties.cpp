// References

// Lambda Idioms: https://www.youtube.com/watch?v=xBAduq0RGes&t=3401s
// Lightning talk "Lambda calculus" https://www.youtube.com/watch?v=1hwRxW99lg0
// Lightning talk "Back to Basics: Lambdas from Scratch" https://www.youtube.com/watch?v=3jCOwajNch0
// Constexpr function parameters: https://www.elbeno.com/blog/?p=1619
// The `constexpr` Problem That Took Me 5 Years To Fix!: https://www.youtube.com/watch?v=ABg4_EV5L3w
// Cppinsights: https://cppinsights.io/

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#include "helper.h"
// Properties on lambdas
// Find the bug
// const + noexcept
// perfect forwarding
// default construction and unevaluated contexts

void legacy(void (*funcPtr)(int))
{
    funcPtr(10);
}

int main()
{
    // Where is the "bug"?
    auto returnX = [x = Helper {}]() { return std::move(x); };

    // The lambda operator is const therefore the move will make a copy and take not effect.
    // Signature of the call operator is auto operator() const {return}... it is also not noexcept by default

    // C++23 will allow attribute specifiers such as [[nodiscard]]

    // Perfect forwarding
    auto perfectForwardingPreCpp20 = [](auto&&... x) { return std::tuple(std::forward<decltype(x)>(x)...); };
    auto perfectForwardingCpp20 = []<typename... Args>(Args&&... x) { return std::tuple(std::forward<Args>(x)...); };

    // Casting to function pointers: What is the type auf auto in the short version.
    auto shortVersion = +[](int x) { return x + 10; };
    auto longVersion = static_cast<int (*)(int)>([](int x) { return x + 10; });

    // Default construction of captureless lambdas and unevaluated contexts
    using myUselessPointer = std::unique_ptr<int, decltype([](auto*) { std::cout << "Behavior baked into the type" << std::endl; })>;

    // Capturing things, static and constexpr things don't need to be captured.
    static auto thing = 10;
    auto alsoThing = [] { return thing; }();
    std::cout << "Thing: " << thing << std::endl;

    // const int is implicit constexpr, change it to float and it won't compile anymore.
    const int ten = 10;
    auto alsoTen = [] { return ten; }();

    // starting with C++20 you can capture this and [=] capturing this is deprecated.
    struct Sample {
        Sample()
        {
            auto willNotWorkWithCpp20 = [=] { return *this; };
            auto copyOfSample = [*this] { return *this; };
        }
    };

    // If you use a lambda in a loop consider moving things into the capture initializer:
    std::string prefix = "foo";
    std::vector<std::string> samples { "galia", "est", "divisa", "in", "patres", "tres" };
    auto result = std::find_if(samples.begin(), samples.end(), [&prefix](const std::string element) {
        std::string keyword = prefix + "bar";
        return element == keyword;
    });

    auto betterResult = std::find_if(samples.begin(), samples.end(), [keyword = prefix + "bar"](const std::string element) {
        return element == keyword;
    });

    return 0;
}