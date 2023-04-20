#include <functional>
#include <iostream>

// References

// Lambda Idioms: https://www.youtube.com/watch?v=xBAduq0RGes&t=3401s
// Lightning talk "Lambda calculus" https://www.youtube.com/watch?v=1hwRxW99lg0
// Lightning talk "Back to Basics: Lambdas from Scratch" https://www.youtube.com/watch?v=3jCOwajNch0
// Constexpr function parameters: https://www.elbeno.com/blog/?p=1619
// The `constexpr` Problem That Took Me 5 Years To Fix!: https://www.youtube.com/watch?v=ABg4_EV5L3w
// Cppinsights: https://cppinsights.io/

int main()
{
    // Methods to have a recursive lambda:
    std::function<int(int)> factorial;
    factorial = [&](int x) {
        if (x == 0)
            return 1;
        return x * factorial(x - 1);
    };
    std::cout << "With std::function: " << factorial(5) << std::endl;

    // Without std::function:
    auto factorialWithoutStdFunc = [&](int x) {
        auto factorial2 = [](auto factorial, int x) {
            if (x == 0)
                return 1;
            return x * factorial(factorial, x - 1);
        };
        return factorial2(factorial2, x);
    };
    std::cout << "Without std::function: " << factorialWithoutStdFunc(5) << std::endl;

    // In c++23 with deducing this
    // auto factorial2 = [](this auto&& self, int x) {
    //     if (x == 0)
    //         return 1;
    //     return x * self(x - 1);
    // };

    return 0;
}