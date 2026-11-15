#include <iostream>

namespace v1 
{
   
    constexpr bool anyTrue() 
    {
        return false;
    }

    template <typename T, typename... Rest>
    constexpr bool anyTrue(T first, Rest... rest) 
    {
        return static_cast<bool>(first) || anyTrue(rest...);
    }
}


namespace v2 {
    template <typename... Args>
    constexpr bool anyTrue(Args... args) 
    {
        return (static_cast<bool>(args) || ...); 
    }
}

int main() 
{
    std::cout << std::boolalpha;

    std::cout << "recursive:\n";
    std::cout << v1::anyTrue(true, false, false) << "\n"; 
    std::cout << v1::anyTrue(false, false, false) << "\n"; 
    std::cout << v1::anyTrue(0, 0, 1) << "\n";            
    std::cout << v1::anyTrue(0, 0, 0) << "\n";            
    std::cout << v1::anyTrue() << "\n";                   

    std::cout << "fold:\n";
    std::cout << v2::anyTrue(true, false, false) << "\n"; 
    std::cout << v2::anyTrue(false, false, false) << "\n"; 
    std::cout << v2::anyTrue(0, 0, 1) << "\n";            
    std::cout << v2::anyTrue(0, 0, 0) << "\n";            
    std::cout << v2::anyTrue() << "\n";                   

    return 0;
}