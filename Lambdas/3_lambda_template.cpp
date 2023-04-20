// References

// Lambda Idioms: https://www.youtube.com/watch?v=xBAduq0RGes&t=3401s
// Lightning talk "Lambda calculus" https://www.youtube.com/watch?v=1hwRxW99lg0
// Lightning talk "Back to Basics: Lambdas from Scratch" https://www.youtube.com/watch?v=3jCOwajNch0
// Constexpr function parameters: https://www.elbeno.com/blog/?p=1619
// IIFE: https://www.cppstories.com/2016/11/iife-for-complex-initialization/
// The `constexpr` Problem That Took Me 5 Years To Fix!: https://www.youtube.com/watch?v=ABg4_EV5L3w
// Cppinsights: https://cppinsights.io/

#include <iostream>

// You can template lambdas not only on the call operator
template <typename T>
constexpr auto typedFunctor = []<typename U>(U secondType) {
    std::cout << "T is: " << typeid(T).name() << std::endl;
    std::cout << "U is: " << typeid(U).name() << std::endl;
    return static_cast<T>(secondType);
};

int main()
{
    // This is useful if you want to provide a customizable lambda for example a converter:
    auto converter = typedFunctor<int>;

    auto x = converter(10.0);
    auto x2 = converter(10.f);

    return 0;
}