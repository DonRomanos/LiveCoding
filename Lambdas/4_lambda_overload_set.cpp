// References

// Lambda Idioms: https://www.youtube.com/watch?v=xBAduq0RGes&t=3401s
// Lightning talk "Lambda calculus" https://www.youtube.com/watch?v=1hwRxW99lg0
// Lightning talk "Back to Basics: Lambdas from Scratch" https://www.youtube.com/watch?v=3jCOwajNch0
// Constexpr function parameters: https://www.elbeno.com/blog/?p=1619
// IIFE: https://www.cppstories.com/2016/11/iife-for-complex-initialization/
// The `constexpr` Problem That Took Me 5 Years To Fix!: https://www.youtube.com/watch?v=ABg4_EV5L3w
// Cppinsights: https://cppinsights.io/

#include <iostream>
#include <variant>

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

// Pre C++20 (CTAD) you also needed:
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

int main()
{
    std::variant<int, double, float> x { 10 };

    std::visit(overloaded {
                   [](int) { std::cout << "int" << std::endl; },
                   [](double) { std::cout << "double" << std::endl; },
                   [](float) { std::cout << "float" << std::endl; } },
        x);
}