#include <iostream>


template <unsigned int N>
struct factorial \
{
    static constexpr unsigned int value = N * factorial<N - 1>::value;
};


template <>
struct factorial<0> 
{
    static constexpr unsigned int value = 1;
};

int main() 
{
    std::cout << factorial<0>::value << "\n";  
    std::cout << factorial<1>::value << "\n"; 
    std::cout << factorial<5>::value << "\n";  
    std::cout << factorial<6>::value << "\n";  

    return 0;
}