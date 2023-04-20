#include <iostream>
#include <string>

struct Helper {
    Helper() = default;

    Helper(const Helper& other)
    {
        std::cout << "Copy" << std::endl;
    }

    Helper& operator=(const Helper& other)
    {
        std::cout << "Copy" << std::endl;
        return *this;
    }

    Helper(Helper&& other)
    {
        std::cout << "Move" << std::endl;
    }

    Helper& operator=(Helper&& other)
    {
        std::cout << "Move" << std::endl;
        return *this;
    }
};