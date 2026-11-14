#include <iostream>
#include <string>

class Discount {
public:
    virtual double calculate(double price) const = 0;
    virtual ~Discount() = default;
};

class RegularDiscount : public Discount {
public:
    double calculate(double price) const override 
    {
        return price;
    }
};

class LoyalDiscount : public Discount {
public:
    double calculate(double price) const override 
    {
        return price * 0.9;
    }
};

class SpecialDiscount : public Discount {
public:
    double calculate(double price) const override
    {
        return price * 0.8;
    }
};

class StudentDiscount : public Discount {
public:
    double calculate(double price) const override
    {
        return price * 0.85;
    }
};

class DiscountCalculator {
public:
    double applyDiscount(double price, const Discount& discount ) const 
    {
        return discount.calculate(price);
    }
};

int main() {
    double price = 100.0;

    RegularDiscount regularDiscount;
    LoyalDiscount loyalDiscount;
    SpecialDiscount specialDiscount;
    StudentDiscount studentDiscount;

    DiscountCalculator discountCalculator;

    std::cout << "Original Price: $" << price << std::endl;
    std::cout << "Regular Discount Price: $" 
              << discountCalculator.applyDiscount(price, regularDiscount) << std::endl;
    std::cout << "Loyal Discount Price: $" 
              << discountCalculator.applyDiscount(price, loyalDiscount) << std::endl;
    std::cout << "Special Discount Price: $" 
              << discountCalculator.applyDiscount(price, specialDiscount) << std::endl;
    std::cout << "Student Discount Price: $" 
              << discountCalculator.applyDiscount(price, studentDiscount) << std::endl;

    return 0;
}