#include <iostream>

// References

// Lambda Idioms: https://www.youtube.com/watch?v=xBAduq0RGes&t=3401s
// Lightning talk "Lambda calculus" https://www.youtube.com/watch?v=1hwRxW99lg0
// Lightning talk "Back to Basics: Lambdas from Scratch" https://www.youtube.com/watch?v=3jCOwajNch0
// Constexpr function parameters: https://www.elbeno.com/blog/?p=1619
// The `constexpr` Problem That Took Me 5 Years To Fix!: https://www.youtube.com/watch?v=ABg4_EV5L3w
// Cppinsights: https://cppinsights.io/

template <size_t N>
constexpr auto assertWithParameter(const char (&param)[N])
{
    static_assert(param[0] == 'H');
    static_assert(param[1] == 'e');
};

// If you use a lambda instead it will work
constexpr auto info = []<typename T>(T string) {
    static_assert(string()[0] == 'H');
    static_assert(string()[1] == 'e');
};

#define constexpr_parameter(X) \
    [] { return X; }

int main()
{
    // This will not compile:
    // assertWithParameter("Hello");

    // But with a lambda it will:
    info(constexpr_parameter("Hello"));

    return 0;
}