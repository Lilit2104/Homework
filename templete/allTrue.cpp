#include <iostream>

namespace v1
{
    constexpr bool allTrue()
    {
        return true;
    }

    template <typename T, typename ... Rest>
    constexpr bool allTrue(T first, Rest ... rest)
    {
        return static_cast<bool>(first) && allTrue(rest...);
    }
}

namespace v2
{
    template <typename ... Args>
    constexpr bool allTrue(Args ... args)
    {
        return (static_cast<bool>(args) && ...);
    }
}

int main() 
{
    std::cout << std::boolalpha;

    std::cout << "recursive:\n";
    std::cout << v1::allTrue(true, true, true) << "\n";  
    std::cout << v1::allTrue(true, false, true) << "\n"; 
    std::cout << v1::allTrue(1, 2, 3) << "\n";           
    std::cout << v1::allTrue(1, 0, 3) << "\n";           
    std::cout << v1::allTrue() << "\n";                  

    std::cout << "fold:\n";
    std::cout << v2::allTrue(true, true, true) << "\n";  
    std::cout << v2::allTrue(true, false, true) << "\n";
    std::cout << v2::allTrue(1, 2, 3) << "\n";           
    std::cout << v2::allTrue(1, 0, 3) << "\n";           
    std::cout << v2::allTrue() << "\n";                  

    return 0;
}