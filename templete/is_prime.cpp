#include <iostream>

template <int N, int D>
struct is_divisible 
{
    static constexpr bool value = (N % D == 0);
};


template <int N, int D, bool Done = (D * D > N), bool Div  = is_divisible<N, D>::value>
struct has_divisor;

template <int N, int D, bool Div>
struct has_divisor<N, D, true, Div> 
{
    static constexpr bool value = false;
};

template <int N, int D>
struct has_divisor<N, D, false, true>
{ 
    static constexpr bool value = true;
};

template <int N, int D>
struct has_divisor<N, D, false, false> 
{
    static constexpr bool value = has_divisor<N, D + 1>::value;
};


template <int N>
struct Is_Prime 
{
    static constexpr bool value = !has_divisor<N, 2>::value;
};


template <>
struct Is_Prime<0> 
{
    static constexpr bool value = false;
};

template <>
struct Is_Prime<1> 
{
    static constexpr bool value = false;
};

int main() 
{
    std::cout << std::boolalpha;
    std::cout << Is_Prime<0>::value << "\n";    
    std::cout << Is_Prime<1>::value << "\n";    
    std::cout << Is_Prime<2>::value << "\n";    
    std::cout << Is_Prime<3>::value << "\n";    
    std::cout << Is_Prime<4>::value << "\n";    
    std::cout << Is_Prime<7>::value << "\n";    
    std::cout << Is_Prime<13>::value << "\n";   
    std::cout << Is_Prime<25>::value << "\n";   
    std::cout << Is_Prime<97>::value << "\n";   
    std::cout << Is_Prime<100>::value << "\n";

    return 0;
}