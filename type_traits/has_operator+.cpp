#include <iostream>
#include <string>

template <typename...>
struct make_void 
{ 
    using type = void; 
};

template <typename... Ts>
using void_t = typename make_void<Ts...>::type;

template <typename T>
T&& declval();

template <typename T, typename = void>
struct has_operator_plus 
{
    static constexpr bool value = false;
};

template <typename T>
struct has_operator_plus<T, void_t<decltype(declval<T>() + declval<T>())>> 
{
    static constexpr bool value = true;
};

struct A {};

int main() 
{
    std::cout << std::boolalpha;
    std::cout << has_operator_plus<int>::value << "\n";          
    std::cout << has_operator_plus<double>::value << "\n";       
    std::cout << has_operator_plus<std::string>::value << "\n";  
    std::cout << has_operator_plus<A>::value << "\n";    
    
    return 0;
}