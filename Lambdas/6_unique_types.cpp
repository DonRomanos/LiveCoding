// References

// Lambda Idioms: https://www.youtube.com/watch?v=xBAduq0RGes&t=3401s
// Lightning talk "Lambda calculus" https://www.youtube.com/watch?v=1hwRxW99lg0
// Lightning talk "Back to Basics: Lambdas from Scratch" https://www.youtube.com/watch?v=3jCOwajNch0
// Constexpr function parameters: https://www.elbeno.com/blog/?p=1619
// IIFE: https://www.cppstories.com/2016/11/iife-for-complex-initialization/
// The `constexpr` Problem That Took Me 5 Years To Fix!: https://www.youtube.com/watch?v=ABg4_EV5L3w
// Cppinsights: https://cppinsights.io/

#include <iostream>

template <auto = [] {}>
struct X { };

int main()
{
    // Is the static assert true?
    {
        auto f1 = [] {};
        auto f2 = [] {};

        static_assert(std::is_same_v<decltype(f1), decltype(f2)>, "Yes or No?");
    }

    {
        auto f1 = [] {};
        auto f2 = f1;

        static_assert(std::is_same_v<decltype(f1), decltype(f2)>, "Yes or No?");
    }

    {
        decltype([] {}) f1;
        decltype([] {}) f2;

        static_assert(std::is_same_v<decltype(f1), decltype(f2)>, "Yes or No?");
    }

    {
        X f1;
        X f2;

        static_assert(std::is_same_v<decltype(f1), decltype(f2)>, "Yes or No?");
    }
}