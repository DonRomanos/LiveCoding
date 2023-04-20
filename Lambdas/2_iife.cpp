// References

// Lambda Idioms: https://www.youtube.com/watch?v=xBAduq0RGes&t=3401s
// Lightning talk "Lambda calculus" https://www.youtube.com/watch?v=1hwRxW99lg0
// Lightning talk "Back to Basics: Lambdas from Scratch" https://www.youtube.com/watch?v=3jCOwajNch0
// Constexpr function parameters: https://www.elbeno.com/blog/?p=1619
// IIFE: https://www.cppstories.com/2016/11/iife-for-complex-initialization/
// The `constexpr` Problem That Took Me 5 Years To Fix!: https://www.youtube.com/watch?v=ABg4_EV5L3w
// Cppinsights: https://cppinsights.io/

#include <iostream>

int main()
{
    // If you want something to happen exactly once you can either use std::call_once or
    static auto _ = [] { std::cout << "this will happen exactly once and threadsafe." << std::endl; return 0; }();

    // This is really useful for const objects, or if you want to scope something or have something non default constructable.
    const auto somethingWithComplexInitLogic = [] {
        if (1) {
            return 5;
        } else {
            return 10;
        }
    }();
}