#include <iostream>

template <typename...>
struct make_void 
{ 
    using type = void; 
};

template <typename... Ts>
using void_t = typename make_void<Ts...>::type;


template <typename T, typename = void>
struct is_default_constructible 
{
    static constexpr bool value = false;
};

template <typename T>
struct is_default_constructible<T, void_t<decltype(new T)>> 
{
    static constexpr bool value = true;
};

struct A {};
struct B { B() = delete; };
struct C { private: C(); };
struct Abstract { virtual void f() = 0; };

int main() 
{
    std::cout << std::boolalpha;
    std::cout << is_default_constructible<int>::value << "\n";       
    std::cout << is_default_constructible<A>::value << "\n";         
    std::cout << is_default_constructible<B>::value << "\n";         
    std::cout << is_default_constructible<C>::value << "\n";         
    std::cout << is_default_constructible<Abstract>::value << "\n";  
    std::cout << is_default_constructible<void>::value << "\n";      
    
    return 0;
}