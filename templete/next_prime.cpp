#include <iostream>

template <int N, int D, bool Done = (D * D > N)>
struct has_divisor 
{
    static constexpr bool value = (N % D == 0) || has_divisor<N, D + 1>::value;
};


template <int N, int D>
struct has_divisor<N, D, true> 
{
    static constexpr bool value = false;
};


template <int N>
struct is_prime 
{
    static constexpr bool value = (N >= 2) && !has_divisor<N, 2>::value;
};


template <int Candidate, bool Prime = is_prime<Candidate>::value>
struct next_prime_from;


template <int Candidate>
struct next_prime_from<Candidate, true> 
{
    static constexpr int value = Candidate;
};

template <int Candidate>
struct next_prime_from<Candidate, false> 
{
    static constexpr int value = next_prime_from<Candidate + 1>::value;
};


template <int N>
struct Next_Prime 
{
    static constexpr int value = next_prime_from<N + 1>::value;
};

int main() 
{
    std::cout << Next_Prime<0>::value << "\n";    
    std::cout << Next_Prime<1>::value << "\n";    
    std::cout << Next_Prime<2>::value << "\n";    
    std::cout << Next_Prime<3>::value << "\n";    
    std::cout << Next_Prime<4>::value << "\n";   
    std::cout << Next_Prime<7>::value << "\n";    
    std::cout << Next_Prime<10>::value << "\n";   
    std::cout << Next_Prime<14>::value << "\n";   
    std::cout << Next_Prime<20>::value << "\n";   
    std::cout << Next_Prime<97>::value << "\n";  
    
    return 0;
    
}