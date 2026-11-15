#include <iostream>
#include <type_traits>

namespace v1 
{
    constexpr int sum() 
    {
        return 0;
    }

    template <typename T, typename... Rest>
    constexpr auto sum(T first, Rest... rest) 
    {
        if constexpr (sizeof...(Rest) == 0)
            return first;
        else
            return first + sum(rest...);
    }
}

namespace v2 
{
    constexpr int sum() 
    {
        return 0;
    }

    template <typename... Args>
    constexpr auto sum(Args... args) 
    {
        return (args + ...);   
    }
}

int main() 
{
    std::cout << "recursive:\n";
    std::cout << v1::sum(1, 2, 3) << "\n";        
    std::cout << v1::sum(1, 2.5, 3) << "\n";      
    std::cout << v1::sum(10, 20, 30, 40) << "\n"; 
    std::cout << v1::sum() << "\n";               

    std::cout << "fold:\n";
    std::cout << v2::sum(1, 2, 3) << "\n";        
    std::cout << v2::sum(1, 2.5, 3) << "\n";      
    std::cout << v2::sum(10, 20, 30, 40) << "\n"; 
    std::cout << v2::sum() << "\n";    
    
    return 0;
}