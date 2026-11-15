#include <iostream>
#include <string>

template <typename...>
struct make_void 
{ 
    using type = void; 
};

template <typename... Ts>
using void_t = typename make_void<Ts...>::type;

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

template <typename T>
struct is_void 
{ 
    static constexpr bool value = is_same<T, void>::value || is_same<T, const void>::value || is_same<T, volatile void>::value ||  is_same<T, const volatile void>::value;
};

template <typename T, typename = void>
struct add_rvalue_ref 
{
     using type = T; 
};

template <typename T>
struct add_rvalue_ref<T, void_t<T&&>> 
{
     using type = T&&; 
};

template <typename T>
typename add_rvalue_ref<T>::type declval();

template <typename To>
void test_convert(To);

template <typename From, typename To, typename = void>
struct is_convertible_impl 
{
    static constexpr bool value = false;
};

template <typename From, typename To>
struct is_convertible_impl<From, To,void_t<decltype(test_convert<To>(declval<From>()))>>
{
    static constexpr bool value = true;
};

template <typename From, typename To>
struct is_convertible 
{
    static constexpr bool value = (is_void<From>::value && is_void<To>::value) || (!is_void<From>::value && !is_void<To>::value &&  is_convertible_impl<From, To>::value);
};

int main() 
{
    std::cout << std::boolalpha;
    std::cout << is_convertible<int, double>::value << "\n";       
    std::cout << is_convertible<double, int>::value << "\n";      
    std::cout << is_convertible<int*, void*>::value << "\n";       
    std::cout << is_convertible<std::string, int>::value << "\n";  
    std::cout << is_convertible<void, void>::value << "\n";        
    std::cout << is_convertible<int, void>::value << "\n";         
    
    return 0;
}