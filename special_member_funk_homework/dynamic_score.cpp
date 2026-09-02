#include <iostream>
#include <utility>

class Score {
private:
    int* score;
    int* bonus;
    int* penalty;

public:
    int getFinalScore() const
    {
        int finalScore = *score + *bonus - *penalty;
        if (finalScore < 0)
            finalScore = 0;
        return finalScore;
    }

    
    Score(int s = 0, int b = 0, int p = 0)
        : score(new int(s)), bonus(new int(b)), penalty(new int(p))
    {
    }

    
    Score(const Score& other)
        : score(new int(*other.score)),
          bonus(new int(*other.bonus)),
          penalty(new int(*other.penalty))
    {
        std::cout << "copy ctor" << std::endl;
    }

   
    Score(Score&& other) noexcept
        : score(other.score),
          bonus(other.bonus),
          penalty(other.penalty)
    {
        std::cout << "move ctor" << std::endl;
        other.score = nullptr;
        other.bonus = nullptr;
        other.penalty = nullptr;
    }

    
    Score& operator=(const Score& other)
    {
        std::cout << "copy assign" << std::endl;
        if (this == &other)
            return *this;

        *score = *other.score;
        *bonus = *other.bonus;
        *penalty = *other.penalty;

        return *this;
    }

    
    Score& operator=(Score&& other) noexcept
    {
        std::cout << "move assign" << std::endl;
        if (this == &other)
            return *this;

        delete score;
        delete bonus;
        delete penalty;

        score = other.score;
        bonus = other.bonus;
        penalty = other.penalty;

        other.score = nullptr;
        other.bonus = nullptr;
        other.penalty = nullptr;

        return *this;
    }

    ~Score()
    {
        delete score;
        delete bonus;
        delete penalty;
    }


    Score& operator++()
    {
        ++(*score);
        return *this;
    }

    
    Score operator++(int)
    {
        Score tmp(*this);
        ++(*score);
        return tmp;
    }

   
    Score& operator--()
    {
        --(*score);
        if (*score < 0)
            *score = 0;
        return *this;
    }

   
    Score operator--(int)
    {
        Score tmp(*this);
        --(*score);
        if (*score < 0)
            *score = 0;
        return tmp;
    }

    Score operator+(const Score& other) const
    {
        int newScore   = *score + *other.score;
        int newBonus   = *bonus + *other.bonus;
        int newPenalty = *penalty + *other.penalty;
        return Score(newScore, newBonus, newPenalty);
    }

    int operator-(const Score& other) const
    {
        return getFinalScore() - other.getFinalScore();
    }

    bool operator==(const Score& other) const
    {
        return getFinalScore() == other.getFinalScore();
    }

    bool operator>(const Score& other) const
    {
        return getFinalScore() > other.getFinalScore();
    }

    friend std::ostream& operator<<(std::ostream& os, const Score& obj)
    {
        os << "Score: "       << *obj.score       << "\n"
           << "Bonus: "       << *obj.bonus       << "\n"
           << "Penalty: "     << *obj.penalty     << "\n"
           << "Final Score: " << obj.getFinalScore();
        return os;
    }
};

int main()
{

    Score a(100, 20, 10);
    std::cout << "Score a(100,20,10):\n" << a << "\n\n";

    std::cout << "Score b = a;\n";
    Score b = a;                   
    std::cout << "b:\n" << b << "\n\n";

    std::cout << " Score c = std::move(a);\n";
    Score c = std::move(a);          
    std::cout << "c:\n" << c << "\n";


    std::cout << " b = c;\n";
    b = c;                          
    std::cout << "b:\n" << b << "\n\n";

    std::cout << " c = std::move(b);\n";
    c = std::move(b);               
    std::cout << "c:\n" << c << "\n";


    Score s1;
    Score s2(23, 50, 0);
    std::cout << "s1:\n" << s1 << "\n\n";
    std::cout << "s2:\n" << s2 << "\n\n";

    ++s2;
    std::cout << "after ++s2:\n" << s2 << "\n\n";

    s2++;
    std::cout << "after s2++:\n" << s2 << "\n\n";

    --s2;
    std::cout << "after --s2:\n" << s2 << "\n\n";

    s2--;
    std::cout << "after s2--:\n" << s2 << "\n\n";

    Score s3(10, 30, 0);
    Score s4(5, 40, 0);
    Score s5 = s3 + s4;
    std::cout << "s5 = s3 + s4:\n" << s5 << "\n\n";

    std::cout << "s3 - s4 = " << (s3 - s4) << "\n";
    std::cout << "s3 == s4: " << (s3 == s4 ? "true" : "false") << "\n";
    std::cout << "s3 > s4: "  << (s3 > s4  ? "true" : "false") << "\n";

    return 0;
}