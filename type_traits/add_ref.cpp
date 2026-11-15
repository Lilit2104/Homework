#include <iostream>

template <typename T, typename U>
struct is_same 
{ 
    static constexpr bool value = false; 
};

template <typename T>
struct is_same<T, T> 
{ 
    static constexpr bool value = true; 
};

template <typename...>
struct make_void 
{ 
    using type = void; 
};

template <typename... Ts>
using void_t = typename make_void<Ts...>::type;


template <typename T, typename = void>
struct add_reference 
{
    using type = T;
};

template <typename T>
struct add_reference<T, void_t<T&>> 
{
    using type = T&;
};

int main() 
{
    std::cout << std::boolalpha;
    std::cout << is_same<add_reference<int>::type, int&>::value << "\n";    
    std::cout << is_same<add_reference<int&>::type, int&>::value << "\n";   
    std::cout << is_same<add_reference<int&&>::type, int&>::value << "\n";  
    std::cout << is_same<add_reference<void>::type, void>::value << "\n";   
    
    return 0;
}