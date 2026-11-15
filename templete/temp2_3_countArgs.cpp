#include <iostream>
#include <cstddef>

namespace v1 
{
    template <typename... Args>
    constexpr std::size_t countArgs(Args...) 
    {
        return sizeof...(Args);
    }
}


namespace v2 
{
    
    constexpr std::size_t countArgs() 
    {
        return 0;
    }

    
    template <typename T, typename... Rest>
    constexpr std::size_t countArgs(T, Rest... rest) 
    {
        return 1 + countArgs(rest...);
    }
}

int main() 
{
    std::cout << "sizeof:\n";
    std::cout << v1::countArgs() << "\n";                  
    std::cout << v1::countArgs(10) << "\n";                
    std::cout << v1::countArgs(10, 20, 30) << "\n";        
    std::cout << v1::countArgs(10, 3.14, "Hello") << "\n"; 

    std::cout << "recursive:\n";
    std::cout << v2::countArgs() << "\n";                  
    std::cout << v2::countArgs(10) << "\n";               
    std::cout << v2::countArgs(10, 20, 30) << "\n";        
    std::cout << v2::countArgs(10, 3.14, "Hello") << "\n";

    return 0;
}