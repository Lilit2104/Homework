#include <iostream>
#include <type_traits>


namespace v1 
{
   
    template <typename T>
    constexpr T minValue(T a) 
    {
        return a;
    }

    template <typename T, typename U, typename... Rest>
    constexpr std::common_type_t<T, U, Rest...>
    minValue(T a, U b, Rest... rest) 
    {
        using R = std::common_type_t<T, U, Rest...>;
        R smaller = (static_cast<R>(b) < static_cast<R>(a)) ? static_cast<R>(b) : static_cast<R>(a);
        return minValue<R>(smaller, static_cast<R>(rest)...);
    }
}


namespace v2 
{
    template <typename T, typename... Rest>
    constexpr std::common_type_t<T, Rest...> minValue(T first, Rest... rest) 
    {
        using R = std::common_type_t<T, Rest...>;
        R result = static_cast<R>(first);
        ((result = (static_cast<R>(rest) < result) ? static_cast<R>(rest) : result), ...);
        return result;
    }
}

int main() 
{
    std::cout << "recursive:\n";
    std::cout << v1::minValue(10, 5) << "\n";           
    std::cout << v1::minValue(10, 5, 20, 3) << "\n";    
    std::cout << v1::minValue(3.5, 2.1, 7.8) << "\n";    
    std::cout << v1::minValue(10, 3.5, 7) << "\n";      

    std::cout << "fold:\n";
    std::cout << v2::minValue(10, 5) << "\n";           
    std::cout << v2::minValue(10, 5, 20, 3) << "\n";    
    std::cout << v2::minValue(3.5, 2.1, 7.8) << "\n";    
    std::cout << v2::minValue(10, 3.5, 7) << "\n";  
    
    return 0;
}
