#include <iostream>
#include <vector>


class NumberValidator {
public:
    virtual bool isValid(double number) const = 0;
    virtual ~NumberValidator() = default;
};

class PositiveNumberValidator : public NumberValidator {
public:
    bool isValid(double number) const override 
    {
        return number > 0;
    }
};

class RestrictedNumberValidator : public NumberValidator {
public:
    bool isValid(double number) const override 
    {
        return number < 200;
    }
};



int main() {
    PositiveNumberValidator positive;
    RestrictedNumberValidator restricted;

    std::vector<NumberValidator*> validators = { &positive,&restricted };

    double numbers[] = {10, -5, 250, 199};

    for (const auto* validator : validators) 
    {
        for (double number : numbers) {
            std::cout << number << " -> "
                      << validator->isValid(number)
                      << '\n';
        }
       
    }

    return 0;
}